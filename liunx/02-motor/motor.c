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

#define motor_CNT			1		  	/* 设备号个数 */
#define motor_NAME		"motor"	/* 名字 */
#define GPIO_NUM            2           /*控制引脚总数*/
#define CMD_TRIG            3

/* 环形缓冲区 */
#define BUF_LEN 128
static u64 g_keys[BUF_LEN];
static int r, w;
#define NEXT_POS(x) ((x+1) % BUF_LEN)

static int is_key_buf_empty(void)
{
	return (r == w);
}

static int is_key_buf_full(void)
{
	return (NEXT_POS(w) == r);
}

static void put_key(u64 time)
{
	if (!is_key_buf_full())
	{
		g_keys[w] = time;
		w = NEXT_POS(w);
	}
}

static u64 get_key(void)
{
	u64 key = 0;
	if (!is_key_buf_empty())
	{
		key = g_keys[r];
		r = NEXT_POS(r);
	}
	return key;
}

static DECLARE_WAIT_QUEUE_HEAD(gpio_wait);

/* gpio_info 结构体 */
struct gpio_info {
    int gpio; 
    char name[NAME_MAX];      /* GPIO 名称 */
	int irqnum;				/* 中断号     */
	int active_low;			/* 是否低电平有效 */
};

/* gpio设备结构体 */
struct motor_dev{
	dev_t devid;			/* 设备号 	 */
	struct cdev cdev;		/* cdev 	*/
	struct class *class;	/* 类 		*/
	struct device *device;	/* 设备 	 */
	int major;				/* 主设备号	  */
	int minor;				/* 次设备号   */  
	struct device_node	*nd; /* 设备节点 */
	struct gpio_info gpios[GPIO_NUM];
	u64 last_rising_time;	/* 记录最后一次上升沿时间 */
};

struct motor_dev motor;	/* motor设备 */

/*
 * @description		: 打开设备
 */
static int motor_open(struct inode *inode, struct file *filp)
{
	filp->private_data = &motor;
	motor.last_rising_time = 0;  // 重置时间
	return 0;
}

/*
 * @description		: 向设备写数据 
 */
static ssize_t motor_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
	return 0;
}

/*
 * @description		: 关闭/释放设备
 */
static int motor_release(struct inode *inode, struct file *filp)
{
	struct motor_dev *dev = filp->private_data;
	dev->last_rising_time = 0;
	return 0;
}

/*
 * @description		: ioctl控制函数
 */
static long motor_ioctl(struct file *filp, unsigned int command, unsigned long arg)
{
	struct motor_dev *dev = filp->private_data;
	
	switch (command)
	{
	case CMD_TRIG:
		{
			// 发送10μs的高电平脉冲
			gpio_set_value(dev->gpios[0].gpio, 1);
			udelay(20);
			gpio_set_value(dev->gpios[0].gpio, 0);
			printk(KERN_DEBUG "Trigger sent\n");
			break;
		}
	default:
		return -ENOTTY;
	}
	return 0;
}

/* 设备操作函数 */
static struct file_operations motor_fops= {
	.owner = THIS_MODULE,
	.open = motor_open,
	.write = motor_write,
	.release = motor_release,
	.unlocked_ioctl = motor_ioctl,
};

/*
 * @description	: 驱动入口函数
 */
static int __init motor_init(void)
{
	int ret = 0;
	unsigned char i = 0;
	enum of_gpio_flags flags;

	/* 初始化环形缓冲区 */
	r = w = 0;
	motor.last_rising_time = 0;

	/* 1、获取设备节点 */
	motor.nd = of_find_node_by_path("/gpioled");
	if(motor.nd == NULL) {
		motor.nd = of_find_compatible_node(NULL, NULL, "alientek,gpioled");
	} 
	if(motor.nd == NULL) {
		printk(KERN_ERR "motor node not find! Trying to search by name...\r\n");
		// 尝试通过名称查找
		motor.nd = of_find_node_by_name(NULL, "gpioled");
		if(motor.nd == NULL) {
			printk(KERN_ERR "motor node still not found!\r\n");
			return -EINVAL;
		}
	}
	printk(KERN_INFO "motor node found: %s\r\n", motor.nd->name);

	/* 2、获取设备树中的gpio属性 */
	for(i = 0; i < GPIO_NUM; i++){
		motor.gpios[i].gpio = of_get_named_gpio_flags(motor.nd, "led-gpios", i, &flags);
		if(motor.gpios[i].gpio < 0) {
			printk(KERN_ERR "can't get gpios[%d], error code: %d\n", i, motor.gpios[i].gpio);
			ret = -EINVAL;
			goto fail_get_gpio;
		}
		motor.gpios[i].active_low = (flags & OF_GPIO_ACTIVE_LOW) ? 1 : 0;
		printk(KERN_INFO "gpio num[%d] = %d, active_%s\n", 
		       i, motor.gpios[i].gpio, 
		       motor.gpios[i].active_low ? "low" : "high");
	}

	/* 3、申请GPIO */
	for(i = 0; i < GPIO_NUM; i++){
		memset(motor.gpios[i].name, 0, sizeof(motor.gpios[i].name));
		if(i == 0)
			sprintf(motor.gpios[i].name, "motor_trig");
		else
			sprintf(motor.gpios[i].name, "motor_echo");
		
		ret = gpio_request(motor.gpios[i].gpio, motor.gpios[i].name);
		if(ret < 0) {
			printk(KERN_ERR "failed to request gpio[%d]\r\n", i);
			goto fail_gpio_request;
		}
	}

	/* 4、设置GPIO方向 */
	ret = gpio_direction_output(motor.gpios[0].gpio, 0); // 初始化为低电平
	if(ret < 0) {
		printk(KERN_ERR "can't set trig gpio as output!\r\n");
		goto fail_gpio_dir;
	}
	

	/* 6、注册字符设备驱动 */
	if (motor.major) {		/* 定义了设备号 */
		motor.devid = MKDEV(motor.major, 0);
		ret = register_chrdev_region(motor.devid, motor_CNT, motor_NAME);
	} else {				/* 没有定义设备号 */
		ret = alloc_chrdev_region(&motor.devid, 0, motor_CNT, motor_NAME);
		motor.major = MAJOR(motor.devid);
		motor.minor = MINOR(motor.devid);
	}
	
	if(ret < 0) {
		printk(KERN_ERR "register chrdev region failed!\r\n");
		goto fail_irq;
	}
	
	printk(KERN_INFO "motor major=%d,minor=%d\r\n", motor.major, motor.minor);	

	/* 初始化cdev */
	cdev_init(&motor.cdev, &motor_fops);
	motor.cdev.owner = THIS_MODULE;
	
	/* 添加cdev */
	ret = cdev_add(&motor.cdev, motor.devid, motor_CNT);
	if(ret < 0) {
		printk(KERN_ERR "cdev_add failed!\r\n");
		goto fail_cdev;
	}

	/* 创建类 */
	motor.class = class_create(THIS_MODULE, motor_NAME);
	if (IS_ERR(motor.class)) {
		ret = PTR_ERR(motor.class);
		goto fail_cdev;
	}

	/* 创建设备 */
	motor.device = device_create(motor.class, NULL, motor.devid, NULL, motor_NAME);
	if (IS_ERR(motor.device)) {
		ret = PTR_ERR(motor.device);
		goto fail_class;
	}

	printk(KERN_INFO "motor driver initialized successfully\n");
	return 0;

fail_class:
	class_destroy(motor.class);
fail_cdev:
	cdev_del(&motor.cdev);
	unregister_chrdev_region(motor.devid, motor_CNT);
fail_irq:
	free_irq(motor.gpios[1].irqnum, &motor);
fail_gpio_dir:
fail_gpio_request:
	for(i = 0; i < GPIO_NUM; i++){
		if(motor.gpios[i].gpio >= 0)
			gpio_free(motor.gpios[i].gpio);
	}
fail_get_gpio:
	return ret;
}

/*
 * @description	: 驱动出口函数
 */
static void __exit motor_exit(void)
{
	unsigned char i = 0;
	
	/* 注销字符设备驱动 */
	device_destroy(motor.class, motor.devid);
	class_destroy(motor.class);
	cdev_del(&motor.cdev);
	unregister_chrdev_region(motor.devid, motor_CNT);
	
	/* 释放GPIO */
	for(i = 0; i < GPIO_NUM; i++){
		gpio_free(motor.gpios[i].gpio);
	}
	
	printk(KERN_INFO "motor driver exit\n");
}

module_init(motor_init);
module_exit(motor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JiangGuangBo");
MODULE_DESCRIPTION("motor Ultrasonic Sensor Driver");
MODULE_VERSION("V1.0");