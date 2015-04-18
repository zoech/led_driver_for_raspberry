#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

struct led_dev{
	struct cdev cdev;
	unsigned char value;
}

struct led_dev *led;


MODULE_AUTHOR("Zoey Zhang");
MODULE_LICENSE("Dual BSD/GPL");


int led_open(struct inode
