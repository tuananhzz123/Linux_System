#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>


#define DRIVER_AUTHOR "Tuan Anh dep trai"
#define DRIVER_DESC "Communication module control"

struct m_foo_dev{
    struct kobject *kobj_ref;
}mdev;

static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff);
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count);

struct kobj_attribute m_arr = __ATTR(direction, 0660, sysfs_show, sysfs_store);

static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff){
    pr_info("Sysfs read...\n");
    return sprintf(buff, "Hello from sysfs!\n");
}   

static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count){
    pr_info("Sysfs write...\n");
    return count;
}

static int __init hello_world_init(void){
    mdev.kobj_ref = kobject_create_and_add("bbb-gpio", NULL);
    if(sysfs_create_file(mdev.kobj_ref, &m_arr.attr)){
        pr_err("Cannot create sysfs file...\n");
        goto rm_kobj;
    }
    pr_info("Hello world initizliaze successfully\n");
    return 0;

    rm_kobj:
    kobject_put(mdev.kobj_ref);
    return -1;
}

static void __exit hello_world_exit(void){
    sysfs_remove_file(mdev.kobj_ref, &m_arr.attr);
    kobject_put(mdev.kobj_ref);

    pr_info("Good bye my fen!\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");
