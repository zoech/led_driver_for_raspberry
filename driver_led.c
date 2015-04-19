#include <linux/kernel.h>  
#include <linux/module.h>
#include <linux/device.h> 
#include <mach/platform.h>       
#include <linux/platform_device.h>
#include <linux/types.h>  
#include <linux/fs.h>   
#include <linux/ioctl.h>  
#include <linux/cdev.h>  
#include <linux/delay.h>  
#include <linux/uaccess.h>
#include <linux/init.h> 
#include <linux/gpio.h>

#define DEVICE_NAME "Pi_Led"
#define DRIVER_NAME "pi_led"

//class声明内核模块驱动信息,是UDEV能够自动生成/dev下相应文件
static dev_t pi_led_devno; //设备号
static struct class *pi_led_class;
static struct cdev pi_led_class_dev;
unsigned long interval;

struct gpio_chip *gpiochip;

#define led_pin 17  //gpio 4

//这部分函数为内核调用后open的设备IO操作,和裸板程序一样
int open_flag=0;
static int pi_led_open(struct inode *inode, struct file *filp)  
{   
    printk("Open led ing!\n");  
    if(open_flag ==0){
       open_flag =1;
       printk("Open led success!\n");
       return 0;
    }
    else{
       printk("Led has opened!\n");     
    }
    return 0;  
} 

size_t pi_led_write(struct file *filp,const char __user *buf,size_t count,loff_t *f_pos){
	
    unsigned long times;
    copy_from_user(&times,(unsigned long*)buf,sizeof(unsigned long));
    
    unsigned long blinks = 0;
    for(;blinks < times;++blinks){
    	gpiochip->set(gpiochip,led_pin,1);
    	msleep(interval);
    	gpiochip->set(gpiochip,led_pin,0);
    	msleep(interval);
    }
    
    return 1;
}

//这部分函数为内核调用后ioctl的设备IO操作,和裸板程序一样
static long pi_led_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) 
{  
    switch(cmd){
      case 0: 
          gpiochip->set(gpiochip, led_pin, 0);
            printk("Led  up!\n");
           break;
      case 1:
            gpiochip->set(gpiochip, led_pin, 1);
            printk("Led  down!\n");
           break;
      case 3:
      	    interval = arg;
      	    break;
	  default:
	  		break;
    }
   
   
    return 0;
} 

static int pi_led_release(struct inode *inode,struct file *file){
     printk("Led has release!\n");
     return 0;
}

//file_operations使系统的open,ioctl等函数指针指向我们所写的led_open等函数,
//这样系统才能够调用
static struct file_operations pi_led_dev_fops = {  
   .owner          =THIS_MODULE,  
   .open	   =pi_led_open, 
   .write	   =pi_led_write,
   .unlocked_ioctl = pi_led_ioctl, 
   .release       = pi_led_release,
}; 

static int is_right_chip(struct gpio_chip *chip, void *data)
{

  if (strcmp(data, chip->label) == 0)
    return 1;
  return 0;
}

//内核加载后的初始化函数.
static int __init pi_led_init(void)
{
   struct device *dev;
   int major; //自动分配主设备号
   major = alloc_chrdev_region(&pi_led_devno,0,1,DRIVER_NAME);
   //register_chrdev 注册字符设备使系统知道有LED这个模块在.
   
   cdev_init(&pi_led_class_dev, &pi_led_dev_fops);
   major = cdev_add(&pi_led_class_dev,pi_led_devno,1);
   //注册class
   pi_led_class = class_create(THIS_MODULE,DRIVER_NAME);
   
   dev = device_create(pi_led_class ,NULL,pi_led_devno,NULL,DRIVER_NAME);
   
   gpiochip = gpiochip_find("bcm2708_gpio", is_right_chip);
   gpiochip->direction_output(gpiochip, led_pin, 1);
   gpiochip->set(gpiochip, led_pin, 1);
   msleep(500);
   gpiochip->set(gpiochip, led_pin, 0);
   interval = 500;
   printk("pi led init ok!\n");
   return 0;
}
//内核卸载后的销毁函数.
void pi_led_exit(void)
{
   gpiochip->set(gpiochip, led_pin, 0);
   gpio_free(led_pin);
   device_destroy(pi_led_class,pi_led_devno);
   class_destroy(pi_led_class);
   cdev_del(&pi_led_class_dev);
   unregister_chrdev_region(pi_led_devno, 1);
   printk("pi led exit ok!\n");
   
}

module_init(pi_led_init);
module_exit(pi_led_exit);

MODULE_DESCRIPTION("Rasp Led Driver");
MODULE_AUTHOR("Embbnux Ji < http://blog.csdn.net/embbnux >");
MODULE_LICENSE("GPL");
