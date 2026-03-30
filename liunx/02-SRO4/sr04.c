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

#define SR04_CNT			1		  	/* 设备号个数 */
#define SR04_NAME		"sr04"	/* 名字 */
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
	irqreturn_t (*handler)(int, void *);	/* 中断服务函数 */
	int active_low;			/* 是否低电平有效 */
};

/* gpio设备结构体 */
struct sr04_dev{
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

struct sr04_dev sr04;	/* sr04设备 */

/*
 * @description		: 打开设备
 */
static int sr04_open(struct inode *inode, struct file *filp)
{
	filp->private_data = &sr04;
	sr04.last_rising_time = 0;  // 重置时间
	return 0;
}

/*
 * @description		: 中断服务函数
 */
static irqreturn_t sr04_handler(int irq, void *dev_id)
{
	struct sr04_dev *dev = (struct sr04_dev *)dev_id;
	int val;
	u64 current_time, time_diff;
	
	current_time = ktime_get_ns();
	val = gpio_get_value(dev->gpios[1].gpio);
	
	// 简化逻辑：直接根据实际电平判断
	// 超声波传感器ECHO引脚：高电平表示回波信号
	if(val) {
		// ECHO引脚变高（回波开始）
		dev->last_rising_time = current_time;
		printk(KERN_DEBUG "ECHO rising at %llu ns\n", current_time);
	} else {
		// ECHO引脚变低（回波结束）
		if(dev->last_rising_time == 0) {
			printk(KERN_WARNING "Missing rising edge, ignoring falling edge\n");
			return IRQ_HANDLED;
		}
		time_diff = current_time - dev->last_rising_time;
		printk(KERN_DEBUG "ECHO falling at %llu ns, time_diff=%llu ns (%.3f ms)\n", 
		       current_time, time_diff, (double)time_diff / 1000000.0);
		
		// 记录有效的时间差（超声波往返时间）
		// 最小有效时间：约58μs（1cm距离）
		// 最大有效时间：约60ms（10m距离）
		if(time_diff > 58000 && time_diff < 60000000) {
			put_key(time_diff);
			wake_up_interruptible(&gpio_wait);
			printk(KERN_DEBUG "Valid measurement: %llu ns recorded\n", time_diff);
		} else {
			printk(KERN_WARNING "Invalid measurement: %llu ns (ignored)\n", time_diff);
		}
		dev->last_rising_time = 0;
	}
	
	return IRQ_HANDLED;
}

/*
 * @description		: 从设备读取数据 
 */
static ssize_t sr04_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
	int err;
	u64 time_ns;
	
	if(cnt < sizeof(u64))
		return -EINVAL;
	
	if(is_key_buf_empty()) {
		if(filp->f_flags & O_NONBLOCK)
			return -EAGAIN;
		
		// 添加超时机制，避免永久阻塞
		if(wait_event_interruptible_timeout(gpio_wait, !is_key_buf_empty(), HZ/2) == 0) {
			// 超时返回0，表示没有数据
			return 0;
		}
	}
	
	time_ns = get_key();
	err = copy_to_user(buf, &time_ns, sizeof(u64));
	
	if(err)
		return -EFAULT;
	
	return sizeof(u64);
}

/*
 * @description		: 向设备写数据 
 */
static ssize_t sr04_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
	return 0;
}

/*
 * @description		: 关闭/释放设备
 */
static int sr04_release(struct inode *inode, struct file *filp)
{
	struct sr04_dev *dev = filp->private_data;
	dev->last_rising_time = 0;
	return 0;
}

/*
 * @description		: ioctl控制函数
 */
static long sr04_ioctl(struct file *filp, unsigned int command, unsigned long arg)
{
	struct sr04_dev *dev = filp->private_data;
	
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
static struct file_operations sr04_fops = {
	.owner = THIS_MODULE,
	.open = sr04_open,
	.read = sr04_read,
	.write = sr04_write,
	.release = sr04_release,
	.unlocked_ioctl = sr04_ioctl,
};

/*
 * @description	: 驱动入口函数
 */
static int __init sr04_init(void)
{
	int ret = 0;
	unsigned char i = 0;
	enum of_gpio_flags flags;

	/* 初始化环形缓冲区 */
	r = w = 0;
	sr04.last_rising_time = 0;

	/* 1、获取设备节点 */
	sr04.nd = of_find_node_by_path("/gpioled");
	if(sr04.nd == NULL) {
		sr04.nd = of_find_compatible_node(NULL, NULL, "alientek,gpioled");
	} 
	if(sr04.nd == NULL) {
		printk(KERN_ERR "sr04 node not find! Trying to search by name...\r\n");
		// 尝试通过名称查找
		sr04.nd = of_find_node_by_name(NULL, "gpioled");
		if(sr04.nd == NULL) {
			printk(KERN_ERR "sr04 node still not found!\r\n");
			return -EINVAL;
		}
	}
	printk(KERN_INFO "sr04 node found: %s\r\n", sr04.nd->name);

	/* 2、获取设备树中的gpio属性 */
	for(i = 0; i < GPIO_NUM; i++){
		sr04.gpios[i].gpio = of_get_named_gpio_flags(sr04.nd, "led-gpios", i, &flags);
		if(sr04.gpios[i].gpio < 0) {
			printk(KERN_ERR "can't get gpios[%d], error code: %d\n", i, sr04.gpios[i].gpio);
			ret = -EINVAL;
			goto fail_get_gpio;
		}
		sr04.gpios[i].active_low = (flags & OF_GPIO_ACTIVE_LOW) ? 1 : 0;
		printk(KERN_INFO "gpio num[%d] = %d, active_%s\n", 
		       i, sr04.gpios[i].gpio, 
		       sr04.gpios[i].active_low ? "low" : "high");
	}

	/* 3、申请GPIO */
	for(i = 0; i < GPIO_NUM; i++){
		memset(sr04.gpios[i].name, 0, sizeof(sr04.gpios[i].name));
		if(i == 0)
			sprintf(sr04.gpios[i].name, "sr04_trig");
		else
			sprintf(sr04.gpios[i].name, "sr04_echo");
		
		ret = gpio_request(sr04.gpios[i].gpio, sr04.gpios[i].name);
		if(ret < 0) {
			printk(KERN_ERR "failed to request gpio[%d]\r\n", i);
			goto fail_gpio_request;
		}
	}

	/* 4、设置GPIO方向 */
	ret = gpio_direction_output(sr04.gpios[0].gpio, 0); // 初始化为低电平
	if(ret < 0) {
		printk(KERN_ERR "can't set trig gpio as output!\r\n");
		goto fail_gpio_dir;
	}
	
	ret = gpio_direction_input(sr04.gpios[1].gpio);
	if(ret < 0) {
		printk(KERN_ERR "can't set echo gpio as input!\r\n");
		goto fail_gpio_dir;
	}

	/* 5、获取并申请中断 */
	sr04.gpios[1].irqnum = gpio_to_irq(sr04.gpios[1].gpio);
	if(sr04.gpios[1].irqnum < 0) {
		printk(KERN_ERR "can't get irq number for echo gpio\r\n");
		ret = -EINVAL;
		goto fail_gpio_dir;
	}
	
	sr04.gpios[1].handler = sr04_handler;
	// 使用双边沿触发
	ret = request_irq(sr04.gpios[1].irqnum, sr04.gpios[1].handler, 
					 IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, 
					 sr04.gpios[1].name, &sr04);
	if(ret < 0){
		printk(KERN_ERR "irq %d request failed!\r\n", sr04.gpios[1].irqnum);
		goto fail_gpio_dir;
	}
	
	printk(KERN_INFO "gpio=%d, irqnum=%d\r\n", sr04.gpios[1].gpio, sr04.gpios[1].irqnum);

	/* 6、注册字符设备驱动 */
	if (sr04.major) {		/* 定义了设备号 */
		sr04.devid = MKDEV(sr04.major, 0);
		ret = register_chrdev_region(sr04.devid, SR04_CNT, SR04_NAME);
	} else {				/* 没有定义设备号 */
		ret = alloc_chrdev_region(&sr04.devid, 0, SR04_CNT, SR04_NAME);
		sr04.major = MAJOR(sr04.devid);
		sr04.minor = MINOR(sr04.devid);
	}
	
	if(ret < 0) {
		printk(KERN_ERR "register chrdev region failed!\r\n");
		goto fail_irq;
	}
	
	printk(KERN_INFO "sr04 major=%d,minor=%d\r\n", sr04.major, sr04.minor);	

	/* 初始化cdev */
	cdev_init(&sr04.cdev, &sr04_fops);
	sr04.cdev.owner = THIS_MODULE;
	
	/* 添加cdev */
	ret = cdev_add(&sr04.cdev, sr04.devid, SR04_CNT);
	if(ret < 0) {
		printk(KERN_ERR "cdev_add failed!\r\n");
		goto fail_cdev;
	}

	/* 创建类 */
	sr04.class = class_create(THIS_MODULE, SR04_NAME);
	if (IS_ERR(sr04.class)) {
		ret = PTR_ERR(sr04.class);
		goto fail_cdev;
	}

	/* 创建设备 */
	sr04.device = device_create(sr04.class, NULL, sr04.devid, NULL, SR04_NAME);
	if (IS_ERR(sr04.device)) {
		ret = PTR_ERR(sr04.device);
		goto fail_class;
	}

	printk(KERN_INFO "SR04 driver initialized successfully\n");
	return 0;

fail_class:
	class_destroy(sr04.class);
fail_cdev:
	cdev_del(&sr04.cdev);
	unregister_chrdev_region(sr04.devid, SR04_CNT);
fail_irq:
	free_irq(sr04.gpios[1].irqnum, &sr04);
fail_gpio_dir:
fail_gpio_request:
	for(i = 0; i < GPIO_NUM; i++){
		if(sr04.gpios[i].gpio >= 0)
			gpio_free(sr04.gpios[i].gpio);
	}
fail_get_gpio:
	return ret;
}

/*
 * @description	: 驱动出口函数
 */
static void __exit sr04_exit(void)
{
	unsigned char i = 0;
	
	/* 注销字符设备驱动 */
	device_destroy(sr04.class, sr04.devid);
	class_destroy(sr04.class);
	cdev_del(&sr04.cdev);
	unregister_chrdev_region(sr04.devid, SR04_CNT);

	/* 释放中断 */
	free_irq(sr04.gpios[1].irqnum, &sr04);
	
	/* 释放GPIO */
	for(i = 0; i < GPIO_NUM; i++){
		gpio_free(sr04.gpios[i].gpio);
	}
	
	printk(KERN_INFO "sr04 driver exit\n");
}

module_init(sr04_init);
module_exit(sr04_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JiangGuangBo");
MODULE_DESCRIPTION("SR04 Ultrasonic Sensor Driver");
MODULE_VERSION("V1.0");