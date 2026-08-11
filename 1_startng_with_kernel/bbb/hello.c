#include <linux/module.h>

#define DRIVER_AUTHOR "TUANANH"
#define DRIVER_DESC "Tuan Anh xin chao"
#define DRIVER_VERS "1.0"

// Constructor

static int __init hello_world_init(void){
	printk(KERN_INFO "Chao may nha, tao la Tuan Anh\n");
	return 0;
	
}

// Destructor
static void __exit hello_world_exit(void){
	printk(KERN_INFO "GoodBye\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERS);
