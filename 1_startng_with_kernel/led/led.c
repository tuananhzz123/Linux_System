#include <linux/module.h>
#include <linux/io.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include "led.h"

#define DRIVER_AUTHOR "Tuan Anh dep trai"
#define DRIVER_DESC "LED blinking"

uint32_t __iomem *base_addr;
static struct task_struct *led_thread;

/* Hàm chạy trong kernel thread riêng, KHÔNG chạy trong context của insmod */
static int led_blink_fn(void *data)
{
	while (!kthread_should_stop()) {
		*(base_addr + GPIO_SETDATAOUT_OFFSET / 4) |= LED;
		msleep(500);   /* nhường CPU, để hệ thống không bị treo */

		if (kthread_should_stop())
			break;

		*(base_addr + GPIO_CLEARDATAOUT_OFFSET / 4) |= LED;
		msleep(500);
	}
	return 0;
}

/* Constructor */
static int __init led_init(void)
{
	base_addr = ioremap(GPIO_0_ADDR_BASE, GPIO_0_ADDR_SIZE);
	if (!base_addr)
		return -ENOMEM;

	*(base_addr + GPIO_OE_OFFSET / 4) &= ~LED;

	led_thread = kthread_run(led_blink_fn, NULL, "led_blink_thread");
	if (IS_ERR(led_thread)) {
		iounmap(base_addr);
		return PTR_ERR(led_thread);
	}

	pr_info("Hello! Initizliaze successfully!\n");
	return 0;
}

/* Destructor */
static void __exit led_exit(void)
{
	if (led_thread)
		kthread_stop(led_thread);   /* dừng thread an toàn */

	*(base_addr + GPIO_CLEARDATAOUT_OFFSET / 4) |= LED;
	iounmap(base_addr);

	pr_info("Good bye my fen !!!\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");
