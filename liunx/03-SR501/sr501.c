#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/of_irq.h>
#include <linux/irq.h>
#include <linux/ioctl.h>

#define SR501_CNT			1		  	/* 设备号个数 */
#define SR501_NAME		"sr501"	/* 名字 */
#define GPIO_NUM            1           /*SR501只需要一个输出引脚*/
#define CMD_TRIG            3

/* 简单的状态变量 */
static int sr501_state = 0;  /* 当前GPIO电平状态 */
static DECLARE_WAIT_QUEUE_HEAD(sr501_wait);

/* gpio_info 结构体 */
struct gpio_info {
    int gpio; 
    char name[NAME_MAX];      /* GPIO 名称 */
	int irqnum;				/* 中断号     */
	irqreturn_t (*handler)(int, void *);	/* 中断服务函数 */
	int active_low;			/* 是否低电平有效 */
};

/* gpio设备结构体 */
struct sr501_dev{
	dev_t devid;			/* 设备号 	 */
	struct cdev cdev;		/* cdev 	*/
	struct class *class;	/* 类 		*/
	struct device *device;	/* 设备 	 */
	int major;				/* 主设备号	  */
	int minor;				/* 次设备号   */  
	struct device_node	*nd; /* 设备节点 */
	struct gpio_info gpios[GPIO_NUM];
	int current_state;		/* 当前GPIO电平状态 */
};

struct sr501_dev sr501;	/* sr501设备 */

/*
 * @description		: 打开设备
 */
static int sr501_open(struct inode *inode, struct file *filp)
{
	filp->private_data = &sr501;
	return 0;
}

/*
 * @description		: 中断服务函数
 */
static irqreturn_t sr501_handler(int irq, void *dev_id)
{
	struct sr501_dev *dev = (struct sr501_dev *)dev_id;
	
	// 直接读取当前GPIO电平
	dev->current_state = gpio_get_value(dev->gpios[1].gpio);
	
	// 唤醒等待队列
	wake_up_interruptible(&gpio_wait);
	
	return IRQ_HANDLED;
}

/*
 * @description		: 从设备读取数据 
 */
static ssize_t sr501_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
	int err;
	int state;
	
	if(cnt < sizeof(int))
		return -EINVAL;
	
	// 直接读取当前GPIO电平状态
	state = gpio_get_value(sr501.gpios[0].gpio);
	
	err = copy_to_user(buf, &state, sizeof(int));
	
	if(err)
		return -EFAULT;
	
	return sizeof(int);
}

/*
 * @description		: 向设备写数据 
 */
static ssize_t sr501_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
	return 0;
}

/*
 * @description		: 关闭/释放设备
 */
static int sr501_release(struct inode *inode, struct file *filp)
{
	return 0;
}



/* 设备操作函数 */
static struct file_operations sr501_fops = {
	.owner = THIS_MODULE,
	.open = sr501_open,
	.read = sr501_read,
	.write = sr501_write,
	.release = sr501_release,
};

/*
 * @description	: 驱动入口函数
 */
static int __init sr501_init(void)
{
	int ret = 0;
	unsigned char i = 0;
	enum of_gpio_flags flags;

	/* 初始化环形缓冲区 */
	r = w = 0;
	sr501.last_rising_time = 0;

	/* 1、获取设备节点 */
	sr501.nd = of_find_node_by_path("/gpioled");
	if(sr501.nd == NULL) {
		sr501.nd = of_find_compatible_node(NULL, NULL, "alientek,gpioled");
	} 
	if(sr501.nd == NULL) {
		printk(KERN_ERR "sr501 node not find! Trying to search by name...\r\n");
		// 尝试通过名称查找
		sr501.nd = of_find_node_by_name(NULL, "gpioled");
		if(sr501.nd == NULL) {
			printk(KERN_ERR "sr501 node still not found!\r\n");
			return -EINVAL;
		}
	}
	printk(KERN_INFO "sr501 node found: %s\r\n", sr501.nd->name);

	/* 2、获取设备树中的gpio属性 */
	for(i = 0; i < GPIO_NUM; i++){
		sr501.gpios[i].gpio = of_get_named_gpio_flags(sr501.nd, "led-gpios", i, &flags);
		if(sr501.gpios[i].gpio < 0) {
			printk(KERN_ERR "can't get gpios[%d], error code: %d\n", i, sr501.gpios[i].gpio);
			ret = -EINVAL;
			goto fail_get_gpio;
		}
		sr501.gpios[i].active_low = (flags & OF_GPIO_ACTIVE_LOW) ? 1 : 0;
		printk(KERN_INFO "gpio num[%d] = %d, active_%s\n", 
		       i, sr501.gpios[i].gpio, 
		       sr501.gpios[i].active_low ? "low" : "high");
	}

	/* 3、申请GPIO */
	for(i = 0; i < GPIO_NUM; i++){
		memset(sr501.gpios[i].name, 0, sizeof(sr501.gpios[i].name));
		if(i == 0)
			sprintf(sr501.gpios[i].name, "sr501_trig");
		else
			sprintf(sr501.gpios[i].name, "sr501_echo");
		
		ret = gpio_request(sr501.gpios[i].gpio, sr501.gpios[i].name);
		if(ret < 0) {
			printk(KERN_ERR "failed to request gpio[%d]\r\n", i);
			goto fail_gpio_request;
		}
	}

	/* 4、设置GPIO方向 */
	ret = gpio_direction_output(sr501.gpios[0].gpio, 0); // 初始化为低电平
	if(ret < 0) {
		printk(KERN_ERR "can't set trig gpio as output!\r\n");
		goto fail_gpio_dir;
	}
	
	ret = gpio_direction_input(sr501.gpios[1].gpio);
	if(ret < 0) {
		printk(KERN_ERR "can't set echo gpio as input!\r\n");
		goto fail_gpio_dir;
	}

	/* 5、获取并申请中断 */
	sr501.gpios[1].irqnum = gpio_to_irq(sr501.gpios[1].gpio);
	if(sr501.gpios[1].irqnum < 0) {
		printk(KERN_ERR "can't get irq number for echo gpio\r\n");
		ret = -EINVAL;
		goto fail_gpio_dir;
	}
	
	sr501.gpios[1].handler = sr501_handler;
	// 使用双边沿触发
	ret = request_irq(sr501.gpios[1].irqnum, sr501.gpios[1].handler, 
					 IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, 
					 sr501.gpios[1].name, &sr501);
	if(ret < 0){
		printk(KERN_ERR "irq %d request failed!\r\n", sr501.gpios[1].irqnum);
		goto fail_gpio_dir;
	}
	
	printk(KERN_INFO "gpio=%d, irqnum=%d\r\n", sr501.gpios[1].gpio, sr501.gpios[1].irqnum);

	/* 6、注册字符设备驱动 */
	if (sr501.major) {		/* 定义了设备号 */
		sr501.devid = MKDEV(sr501.major, 0);
		ret = register_chrdev_region(sr501.devid, SR501_CNT, SR501_NAME);
	} else {				/* 没有定义设备号 */
		ret = alloc_chrdev_region(&sr501.devid, 0, SR501_CNT, SR501_NAME);
		sr501.major = MAJOR(sr501.devid);
		sr501.minor = MINOR(sr501.devid);
	}
	
	if(ret < 0) {
		printk(KERN_ERR "register chrdev region failed!\r\n");
		goto fail_irq;
	}
	
	printk(KERN_INFO "sr501 major=%d,minor=%d\r\n", sr501.major, sr501.minor);	

	/* 初始化cdev */
	cdev_init(&sr501.cdev, &sr501_fops);
	sr501.cdev.owner = THIS_MODULE;
	
	/* 添加cdev */
	ret = cdev_add(&sr501.cdev, sr501.devid, SR501_CNT);
	if(ret < 0) {
		printk(KERN_ERR "cdev_add failed!\r\n");
		goto fail_cdev;
	}

	/* 创建类 */
	sr501.class = class_create(THIS_MODULE, SR501_NAME);
	if (IS_ERR(sr501.class)) {
		ret = PTR_ERR(sr501.class);
		goto fail_cdev;
	}

	/* 创建设备 */
	sr501.device = device_create(sr501.class, NULL, sr501.devid, NULL, SR501_NAME);
	if (IS_ERR(sr501.device)) {
		ret = PTR_ERR(sr501.device);
		goto fail_class;
	}

	printk(KERN_INFO "SR501 driver initialized successfully\n");
	return 0;

fail_class:
	class_destroy(sr501.class);
fail_cdev:
	cdev_del(&sr501.cdev);
	unregister_chrdev_region(sr501.devid, SR501_CNT);
fail_irq:
	free_irq(sr501.gpios[1].irqnum, &sr501);
fail_gpio_dir:
fail_gpio_request:
	for(i = 0; i < GPIO_NUM; i++){
		if(sr501.gpios[i].gpio >= 0)
			gpio_free(sr501.gpios[i].gpio);
	}
fail_get_gpio:
	return ret;
}

/*
 * @description	: 驱动出口函数
 */
static void __exit sr501_exit(void)
{
	unsigned char i = 0;
	
	/* 注销字符设备驱动 */
	device_destroy(sr501.class, sr501.devid);
	class_destroy(sr501.class);
	cdev_del(&sr501.cdev);
	unregister_chrdev_region(sr501.devid, SR501_CNT);

	/* 释放中断 */
	free_irq(sr501.gpios[1].irqnum, &sr501);
	
	/* 释放GPIO */
	for(i = 0; i < GPIO_NUM; i++){
		gpio_free(sr501.gpios[i].gpio);
	}
	
	printk(KERN_INFO "sr501 driver exit\n");
}

module_init(sr501_init);
module_exit(sr501_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JiangGuangBo");
MODULE_DESCRIPTION("SR501 Ultrasonic Sensor Driver");
MODULE_VERSION("V1.0");