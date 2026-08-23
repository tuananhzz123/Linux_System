#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>


#define DRIVER_AUTHOR "Tuan Anh dep trai"
#define DRIVER_DESC "Communication module control"

struct m_foo_dev{
    struct kobject *kobj_ref;
}mdev;

static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff);
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count);
static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff);
static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count);

struct kobj_attribute value =  __ATTR(value, 0660, value_show, value_store);
struct kobj_attribute direction = __ATTR(direction, 0660, direction_show, direction_store);

static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff){
    pr_info("value read...\n");
    return sprintf(buff, "Hello from value!\n");
}   

static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count){
    pr_info("value write...\n");
    return count;
}

static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff){
    pr_info("Direction read...\n");
    return sprintf(buff, "Hello from direction!\n");
}   

static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count){
    pr_info("Direction write...\n");
    return count;
}

static struct attribute *attrs[] = {
    &direction.attr,
    &value.attr,
    NULL,
};

static struct attribute_group attr_group = {
    .attrs = attrs,
};

static int __init hello_world_init(void){

    mdev.kobj_ref = kobject_create_and_add("bbb-gpio", NULL);
    if(sysfs_create_group(mdev.kobj_ref, &attr_group)){
        pr_err("Cannot create sysfs group...\n");
        goto rm_kobj;
    }
    pr_info("Hello world initizliaze successfully\n");
    return 0;

    rm_kobj:
    kobject_put(mdev.kobj_ref);
    return -1;
}

static void __exit hello_world_exit(void){
    sysfs_remove_group(mdev.kobj_ref, &attr_group);
    kobject_put(mdev.kobj_ref);

    pr_info("Good bye my fen!\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");
