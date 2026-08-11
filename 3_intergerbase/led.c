#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>

#define DRIVER_AUTHOR "Tuan Anh dep trai"
#define DRIVER_DESC "Led module control"

#define GPIO_LED 30

static int __init led_init(void){
    int retval;

    gpio_request(GPIO_LED, "led");
    gpio_direction_output(GPIO_LED, 0);
    gpio_set_value(GPIO_LED, 1);

    pr_info("Hello! Initizliaze successfully\n");
    return 0;
}

static void __exit led_exit(void){
    gpio_set_value(GPIO_LED, 0);
    gpio_free(GPIO_LED);

    pr_info("Good bye my fen!\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");

