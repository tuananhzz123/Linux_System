#include <linux/module.h>  
#include <linux/fs.h>      
#include <linux/device.h>  
#include <linux/cdev.h>    
#include <linux/slab.h>     
#include <linux/uaccess.h>  
#include <linux/io.h>
#include "led.h"

#define DRIVER_AUTHOR "Tuan Anh dep trai"
#define DRIVER_DESC   "LED Control Character Driver"

#define NPAGES  1

#define WR_VALUE _IOW('a', '1', int32_t *)
#define RD_VALUE _IOR('a', '2', int32_t *)

static int32_t led_status = 0;
uint32_t __iomem *base_addr;

struct m_foo_dev {
    int size;
    char *kmalloc_ptr;
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
} mdev;

/*  Function Prototypes */
static int      __init hello_world_init(void);
static void     __exit hello_world_exit(void);
static int      m_open(struct inode *inode, struct file *file);
static int      m_release(struct inode *inode, struct file *file);
static ssize_t  m_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset);
static ssize_t  m_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset);
static long     m_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops =
{
    .owner          = THIS_MODULE,
    .read           = m_read,
    .write          = m_write,
    .open           = m_open,
    .release        = m_release,
    .unlocked_ioctl = m_ioctl,
};

/* Hàm bật/tắt LED trực tiếp trên phần cứng */
static void set_led_state(int state)
{
    if (base_addr == NULL)
        return;

    if (state) {
        /* Bật LED */
        *(base_addr + GPIO_SETDATAOUT_OFFSET / 4) |= LED;
        led_status = 1;
        pr_info("LED Turned ON\n");
    } else {
        /* Tắt LED */
        *(base_addr + GPIO_CLEARDATAOUT_OFFSET / 4) |= LED;
        led_status = 0;
        pr_info("LED Turned OFF\n");
    }
}

static int m_open(struct inode *inode, struct file *file)
{
    pr_info("System call open() called...!!!\n");
    return 0;
}

static int m_release(struct inode *inode, struct file *file)
{
    pr_info("System call close() called...!!!\n");
    return 0;
}

static ssize_t m_read(struct file *filp, char __user *user_buffer, size_t size, loff_t *offset)
{
    char buf[16];
    int len = snprintf(buf, sizeof(buf), "%d\n", led_status);

    if (*offset >= len)
        return 0;

    if (copy_to_user(user_buffer, buf, len))
        return -EFAULT;

    *offset += len;
    return len;
}

static ssize_t m_write(struct file *filp, const char __user *user_buffer, size_t size, loff_t *offset)
{
    char kbuf[10];
    size_t to_copy = min(size, sizeof(kbuf) - 1);

    if (copy_from_user(kbuf, user_buffer, to_copy))
        return -EFAULT;

    kbuf[to_copy] = '\0';

    /* Nhận giá trị '1' hoặc '0' từ userspace */
    if (kbuf[0] == '1') {
        set_led_state(1);
    } else if (kbuf[0] == '0') {
        set_led_state(0);
    } else {
        pr_err("Invalid command! Use '1' to ON, '0' to OFF.\n");
    }

    return size;
}

static long m_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int32_t val = 0;

    switch(cmd) {
        case WR_VALUE:
            if (copy_from_user(&val, (int32_t*)arg, sizeof(val))) {
                pr_err("Data Write : Err!\n");
                return -EFAULT;
            }
            set_led_state(val ? 1 : 0);
            break;

        case RD_VALUE:
            if (copy_to_user((int32_t*)arg, &led_status, sizeof(led_status))) {
                pr_err("Data Read : Err!\n");
                return -EFAULT;
            }
            break;

        default:
            pr_info("Default ioctl option\n");
            break;
    }

    return 0;
}

static int __init hello_world_init(void)   /* Constructor */
{   
    /* 1. Mapping IO memory */
    base_addr = ioremap(GPIO_0_ADDR_BASE, GPIO_0_ADDR_SIZE);
    if (!base_addr) {
        pr_err("Failed to ioremap GPIO memory\n");
        return -ENOMEM;
    }

    /* Cấu hình chân GPIO làm OUTPUT */
    *(base_addr + GPIO_OE_OFFSET / 4) &= ~LED;

    /* 2. Allocating device number */
    if (alloc_chrdev_region(&mdev.dev_num, 0, 1, "m-cdev") < 0) {
	    pr_err("Failed to alloc chrdev region\n");
        iounmap(base_addr);
	    return -1;
    }

    /* 3. Creating cdev structure */
    cdev_init(&mdev.m_cdev, &fops);

    /* 4. Adding character device to system */
    if ((cdev_add(&mdev.m_cdev, mdev.dev_num, 1)) < 0) {
        pr_err("Cannot add the device to the system\n");
        goto rm_device_numb;
    }

    /* 5. Creating struct class */
    if ((mdev.m_class = class_create("m_class")) == NULL) {
        pr_err("Cannot create the struct class for my device\n");
        goto rm_device_numb;
    }

    /* 6. Creating device node /dev/m_device */
    if ((device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "m_device")) == NULL) {
        pr_err("Cannot create my device\n");
        goto rm_class;
    }

    pr_info("LED Driver Initizliazed successfully!\n");
    return 0;

rm_class:
    class_destroy(mdev.m_class);
rm_device_numb:
    unregister_chrdev_region(mdev.dev_num, 1);
    iounmap(base_addr);
    return -1;
}

static void __exit hello_world_exit(void)   /* Destructor */
{
    /* Tắt LED trước khi gỡ module */
    set_led_state(0);

    /* Giải phóng tài nguyên */
    device_destroy(mdev.m_class, mdev.dev_num);
    class_destroy(mdev.m_class);
    cdev_del(&mdev.m_cdev);
    unregister_chrdev_region(mdev.dev_num, 1);
    iounmap(base_addr);

    pr_info("LED Driver Exited Successfully!\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION("1.0");