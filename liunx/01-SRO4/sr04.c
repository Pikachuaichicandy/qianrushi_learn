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
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: gpio.c
作者	  	: jgb
版本	   	: V1.0
描述	   	: 采用pinctrl和gpio子系统驱动LED灯。
其他	   	: 无                                                                                                                                                                                                                                                                                                                                             
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2019/7/13 左忠凯创建
***************************************************************/
#define SR04_CNT			1		  	/* 设备号个数 */
#define SR04_NAME		"sr04"	/* 名字 */
#define LEDOFF 				0			/* 关灯 */
#define LEDON 				1			/* 开灯 */
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
	return (r == NEXT_POS(w));
}

static void put_key(u64 time)
{
	if (!is_key_buf_full())
	{
		g_keys[w] = time;
		w = NEXT_POS(w);
	}
}

static char get_key(void)
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
	int key;                 /* GPIO 编号 */
    char name[NAME_MAX];      /* GPIO 名称 */
	int irqnum;								/* 中断号     */
	unsigned char value;					/* 按键对应的键值 */
	irqreturn_t (*handler)(int, void *);	/* 中断服务函数 */
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
	//int gpio;			/* led所使用的GPIO编号		*/
	struct gpio_info gpios[GPIO_NUM];
};

struct sr04_dev sr04;	/* led设备 */

/*
 * @description		: 打开设备
 * @param - inode 	: 传递给驱动的inode
 * @param - filp 	: 设备文件，file结构体有个叫做private_data的成员变量
 * 					  一般在open的时候将private_data指向设备结构体。
 * @return 			: 0 成功;其他 失败
 */

static irqreturn_t sr04_handler(int irq, void *dev_id)
{
	struct sr04_dev *dev = (struct sr04_dev *)dev_id;
	int val;
	static u64 rising_time = 0;
	u64 time;
	val = gpio_get_value(sr04.gpios[1].gpio);
	if(val){
		rising_time = ktime_get_ns();
	}
	else{
		if(rising_time == 0){
			printk("missing rising interrupt\n");
			return IRQ_HANDLED;	
		}
		time = ktime_get_ns()-rising_time;
		put_key(time);
		wake_up_interruptible(&gpio_wait);


	}


	// struct sr04_dev *dev = (struct sr04_dev *)dev_id;

	// dev->curkeynum = 0;
	// dev->timer.data = (volatile long)dev_id;
	return IRQ_RETVAL(IRQ_HANDLED);
	// mod_timer(&dev->timer, jiffies + msecs_to_jiffies(10));	/* 10ms定时 */
}

static int sr04_open(struct inode *inode, struct file *filp)
{
	filp->private_data = &sr04; /* 设置私有数据 */
	return 0;
}

/*
 * @description		: 从设备读取数据 
 * @param - filp 	: 要打开的设备文件(文件描述符)
 * @param - buf 	: 返回给用户空间的数据缓冲区
 * @param - cnt 	: 要读取的数据长度
 * @param - offt 	: 相对于文件首地址的偏移
 * @return 			: 读取的字节数，如果为负值，表示读取失败
 */
static ssize_t sr04_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
	int err;
	int key;
	if(is_key_buf_empty() && (filp->f_flags & O_NONBLOCK))
		return -EAGAIN;
	wait_event_interruptible(gpio_wait,!is_key_buf_empty());
	key = get_key();
	err = copy_to_user(buf,&key,4);
	return 4;
}

/*
 * @description		: 向设备写数据 
 * @param - filp 	: 设备文件，表示打开的文件描述符
 * @param - buf 	: 要写给设备写入的数据
 * @param - cnt 	: 要写入的数据长度
 * @param - offt 	: 相对于文件首地址的偏移
 * @return 			: 写入的字节数，如果为负值，表示写入失败
 */
static ssize_t sr04_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
	
	return 0;
}

/*
 * @description		: 关闭/释放设备
 * @param - filp 	: 要关闭的设备文件(文件描述符)
 * @return 			: 0 成功;其他 失败
 */
static int sr04_release(struct inode *inode, struct file *filp)
{
	return 0;
}
//ioctl(fd,CMD,ARG)
static long sr04_ioctl(struct file *filp, unsigned int command, unsigned long arg)
{
	/*send triger*/
	switch (command)
	{
	case CMD_TRIG:
		{
			gpio_set_value(sr04.gpios[0].gpio,1);
			udelay(20);
			gpio_set_value(sr04.gpios[0].gpio,0);
		}
	
	}
	return 0;
}

/* 设备操作函数 */
static struct file_operations sr04_fops = {
	.owner = THIS_MODULE,
	.open = sr04_open,
	.read = sr04_read,
	.write = sr04_write,
	.release = 	sr04_release,
	.unlocked_ioctl = sr04_ioctl,
};

/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */
static int __init sr04_init(void)
{
	int ret = 0;
	unsigned char i = 0;
	/* 设置LED所使用的GPIO */
	/* 1、获取设备节点：sr04 */
	sr04.nd = of_find_node_by_path("/gpioled");
	if(sr04.nd == NULL) {
		sr04.nd = of_find_compatible_node(NULL, NULL, "alientek,gpioled");
	} 
	if(sr04.nd == NULL) {
		printk("sr04 node not find!\r\n");
		return -EINVAL;
	} else {
		printk("sr04 node find!\r\n");
	}

	/* 2、 获取设备树中的gpio属性 */
	/* 获取trig引脚 */
	sr04.gpios[0].gpio = of_get_named_gpio(sr04.nd, "trig-gpios", 0);
	if(sr04.gpios[0].gpio < 0) {
		printk("can't get trig-gpios, error code: %d\n", sr04.gpios[0].gpio);
		ret = -EINVAL;
		goto fail_findnode;	
	}
	printk("trig gpio = %d\r\n", sr04.gpios[0].gpio); 

	/* 获取echo引脚 */
	sr04.gpios[1].gpio = of_get_named_gpio(sr04.nd, "echo-gpios", 0);
	if(sr04.gpios[1].gpio < 0) {
		printk("can't get echo-gpios, error code: %d\n", sr04.gpios[1].gpio);
		ret = -EINVAL;
		goto fail_findnode;	
	}
	printk("echo gpio = %d\r\n", sr04.gpios[1].gpio);
	
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
		}
	}

	/* 3、设置GPIO1_IO03为输出，并且输出low*/
	ret = gpio_direction_output(sr04.gpios[0].gpio, 0);
	if(ret < 0) {
		printk("can't set gpio!\r\n");
	}
	ret = gpio_direction_input(sr04.gpios[1].gpio);
	if(ret < 0) {
		printk(KERN_ERR "can't set echo gpio as input!\r\n");
	}

	sr04.gpios[1].irqnum = irq_of_parse_and_map(sr04.nd, 1); 
	/* 申请中断 */
	sr04.gpios[1].handler = sr04_handler;
	ret = request_irq(sr04.gpios[1].irqnum, sr04.gpios[1].handler, 
						 IRQF_TRIGGER_RISING, sr04.gpios[1].name, &sr04);
	if(ret < 0){
		printk("irq %d request failed!\r\n", sr04.gpios[1].irqnum);
		return -EFAULT;
	}
	printk("gpio=%d, irqnum=%d\r\n",sr04.gpios[1].gpio, 
                                         sr04.gpios[1].irqnum);


	/* 注册字符设备驱动 */
	/* 1、创建设备号 */
	if (sr04.major) {		/*  定义了设备号 */
		sr04.devid = MKDEV(sr04.major, 0);
		register_chrdev_region(sr04.devid, SR04_CNT, SR04_NAME);
	} 
	else {						/* 没有定义设备号 */
		alloc_chrdev_region(&sr04.devid, 0, SR04_CNT, SR04_NAME);	/* 申请设备号 */
		sr04.major = MAJOR(sr04.devid);	/* 获取分配号的主设备号 */
		sr04.minor = MINOR(sr04.devid);	/* 获取分配号的次设备号 */
	}
	printk("sr04 major=%d,minor=%d\r\n",sr04.major, sr04.minor);	
	/* 2、初始化cdev */
	sr04.cdev.owner = THIS_MODULE;
	cdev_init(&sr04.cdev, &sr04_fops);
	
	/* 3、添加一个cdev */
	cdev_add(&sr04.cdev, sr04.devid, SR04_CNT);

	/* 4、创建类 */
	sr04.class = class_create(THIS_MODULE, SR04_NAME);
	if (IS_ERR(sr04.class)) {
		return PTR_ERR(sr04.class);
	}

	/* 5、创建设备 */
	sr04.device = device_create(sr04.class, NULL, sr04.devid, NULL, SR04_NAME);
	if (IS_ERR(sr04.device)) {
		return PTR_ERR(sr04.device);
	}
	return 0;
	
fail_findnode:
     return ret;
}

/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */
static void __exit sr04_exit(void)
{
	unsigned char i = 0;
	/* 注销字符设备驱动 */
	cdev_del(&sr04.cdev);/*  删除cdev */
	unregister_chrdev_region(sr04.devid, SR04_CNT); /* 注销设备号 */

	device_destroy(sr04.class,sr04.devid);
	class_destroy(sr04.class);
	for(i=0;i<GPIO_NUM;i++){
		gpio_free(sr04.gpios[i].gpio);
	}
	free_irq(sr04.gpios[1].irqnum, &sr04);
	
}

module_init(sr04_init);
module_exit(sr04_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("JiangGuangBo");