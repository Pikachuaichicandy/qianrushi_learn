#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/gfp.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include <asm/io.h>
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: dht11.c
作者	  	: 蒋光波
版本	   	: V1.0
描述	   	: dht11驱动
其他	   	: 无
GitHub	   	: https://github.com/Pikachuaichicandy/qianrushi_learn.git
日志	   	: 初版V1.0 2026/2/18 蒋光波创建
***************************************************************/
#define DHT11_CNT		1			/* 设备号个数 	*/
#define DHT11_NAME		"dht11"	/* 名字 		*/
#define KEY_NUM				1			/* 按键数量 	*/

// /* 中断IO描述结构体 */
// struct irq_keydesc {
// 	int gpio;								/* gpio */
// 	int irqnum;								/* 中断号     */
// 	unsigned char value;					/* 按键对应的键值 */
// 	char name[10];							/* 名字 */
// 	irqreturn_t (*handler)(int, void *);	/* 中断服务函数 */
// };

struct gpio_desc{
	int gpio;
	int irq;
    char *name;
    int key;
	struct timer_list key_timer;
    irqreturn_t (*handler)(int, void *);	/* 中断服务函数 */
} ;

/* dht11设备结构体 */
struct dht11_dev{
	dev_t devid;			/* 设备号 	 */
	struct cdev cdev;		/* cdev 	*/
	struct class *class;	/* 类 		*/
	struct device *device;	/* 设备 	 */
	int major;				/* 主设备号	  */
	int minor;				/* 次设备号   */
	struct device_node	*nd; /* 设备节点 */
	atomic_t keyvalue;		/* 有效的按键键值 */
	atomic_t releasekey;	/* 标记是否完成一次完成的按键，包括按下和释放 */
	struct timer_list timer;/* 定义一个定时器*/
	// struct irq_keydesc irqkeydesc[KEY_NUM];	/* 按键描述数组 */
	// unsigned char curkeynum;				/* 当前的按键号 */
    struct gpio_desc gpios[1];
};

struct dht11_dev dht11;	/* irq设备 */

static u64 g_dht11_irq_time[84];
static int g_dht11_irq_cnt = 0;

/* 环形缓冲区 */
#define BUF_LEN 128
static char g_keys[BUF_LEN];
static int r, w;

struct fasync_struct *button_fasync;

static irqreturn_t dht11_isr(int irq, void *dev_id);
static void parse_dht11_datas(void);

#define NEXT_POS(x) ((x+1) % BUF_LEN)

static int is_key_buf_empty(void)
{
	return (r == w);
}

static int is_key_buf_full(void)
{
	return (r == NEXT_POS(w));
}

static void put_key(char key)
{
	if (!is_key_buf_full())
	{
		g_keys[w] = key;
		w = NEXT_POS(w);
	}
}

static char get_key(void)
{
	char key = 0;
	if (!is_key_buf_empty())
	{
		key = g_keys[r];
		r = NEXT_POS(r);
	}
	return key;
}


static DECLARE_WAIT_QUEUE_HEAD(gpio_wait);

// static void key_timer_expire(struct timer_list *t)
static void key_timer_expire(unsigned long data)
{
	struct gpio_desc *desc = (struct gpio_desc *)data;
	// 定时器超时：数据读取失败
	printk("DHT11 read timeout\n");
	put_key(-1);
	put_key(-1);
	wake_up_interruptible(&gpio_wait);
}




/* @description		: 中断服务函数，开启定时器，延时10ms，
 *				  	  定时器用于按键消抖。
 * @param - irq 	: 中断号 
 * @param - dev_id	: 设备结构。
 * @return 			: 中断执行结果
 */
static irqreturn_t dht11_isr(int irq, void *dev_id)
{
	struct gpio_desc *gpio_desc = dev_id;
	u64 time;
	
	/* 1. 记录中断发生的时间 */
	time = ktime_get_ns();
	g_dht11_irq_time[g_dht11_irq_cnt] = time;
	//printk("pinval = %d\n", gpio_get_value(115));

	/* 2. 累计次数 */
	g_dht11_irq_cnt++;

	/* 3. 次数足够: 解析数据, 放入环形buffer, 唤醒APP */
	if (g_dht11_irq_cnt == 84)
	{
		del_timer(&gpio_desc->key_timer);
		parse_dht11_datas();
	}
	else if (g_dht11_irq_cnt > 84)
	{
		/* 数据过多，出错 */
		printk("DHT11 data overflow\n");
		del_timer(&gpio_desc->key_timer);
		put_key(-1);
		put_key(-1);
		wake_up_interruptible(&gpio_wait);
	}

	return IRQ_HANDLED;
}

/* @description	: 定时器服务函数，用于按键消抖，定时器到了以后
 *				  再次读取按键值，如果按键还是处于按下状态就表示按键有效。
 * @param - arg	: 设备结构变量
 * @return 		: 无
 */
// void timer_function(unsigned long arg)
// {
	
// }

/*
 * @description	: 按键IO初始化
 * @param 		: 无
 * @return 		: 无
 */
static int gpios_init(void)
{
	unsigned char i = 0;
	int ret = 0;
    int err = 0;
    int count = sizeof(dht11.gpios)/sizeof(dht11.gpios[0]);
	
	dht11.nd = of_find_node_by_path("/gpioled");
	if (dht11.nd== NULL){
		printk("gpioled node not find!\r\n");
		return -EINVAL;
	} 

	/* 提取GPIO */
	for (i = 0; i < KEY_NUM; i++) {
		dht11.gpios[i].gpio = of_get_named_gpio(dht11.nd ,"led-gpios", i);
		if (dht11.gpios[i].gpio < 0) {
			printk("can't get gpio%d\r\n", i);
		}
	}



	for (i = 0; i < count; i++)
	{	
        /* 为name字段分配内存 */
        dht11.gpios[i].name = kzalloc(10, GFP_KERNEL);
        if (!dht11.gpios[i].name) {
            printk("Failed to allocate memory for name\r\n");
            return -ENOMEM;
        }
		sprintf(dht11.gpios[i].name, "dht11%d", i);		/* 组合名字 */
		dht11.gpios[i].irq  = gpio_to_irq(dht11.gpios[i].gpio);
        printk("key%d:gpio=%d, irqnum=%d\r\n",i, dht11.gpios[i].gpio, 
                                         dht11.gpios[i].irq);

		/* 设置DHT11 GPIO引脚的初始状态: output 1 */
		err = gpio_request(dht11.gpios[i].gpio, dht11.gpios[i].name);
		gpio_direction_output(dht11.gpios[i].gpio, 1);
		gpio_free(dht11.gpios[i].gpio);

		/* 初始化定时器 */
		init_timer(&dht11.gpios[i].key_timer);
		dht11.gpios[i].key_timer.function = key_timer_expire;
		dht11.gpios[i].key_timer.data = (unsigned long)&dht11.gpios[i];
		/* 激活定时器，但设置一个很远的过期时间 */
		dht11.gpios[i].key_timer.expires = ~0;
		add_timer(&dht11.gpios[i].key_timer);

		// setup_timer(&dht11.gpios[i].key_timer, key_timer_expire, (unsigned long)&dht11.gpios[i]);
	 	//timer_setup(&dht11.gpios[i].key_timer, key_timer_expire, 0);
		//dht11.gpios[i].key_timer.expires = ~0;
		//add_timer(&dht11.gpios[i].key_timer);
		//err = request_irq(dht11.gpios[i].irq, dht11_isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "100ask_gpio_key", &dht11.gpios[i]);
    }
	// /* 创建定时器 */
	// init_timer(&dht11.timer);
	// dht11.timer.function = timer_function;
	return 0;
}

/*
 * @description		: 打开设备
 * @param - inode 	: 传递给驱动的inode
 * @param - filp 	: 设备文件，file结构体有个叫做private_data的成员变量
 * 					  一般在open的时候将private_data指向设备结构体。
 * @return 			: 0 成功;其他 失败
 */
static int dht11_open(struct inode *inode, struct file *filp)
{
	filp->private_data = &dht11;	/* 设置私有数据 */
	return 0;
}

 /*
  * @description     : 从设备读取数据 
  * @param - filp    : 要打开的设备文件(文件描述符)
  * @param - buf     : 返回给用户空间的数据缓冲区
  * @param - cnt     : 要读取的数据长度
  * @param - offt    : 相对于文件首地址的偏移
  * @return          : 读取的字节数，如果为负值，表示读取失败
  */
static ssize_t dht11_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{

	int err;
	char kern_buf[2];
	struct gpio_desc *desc;

	if (cnt != 2)
		return -EINVAL;

	g_dht11_irq_cnt = 0;

	/* 1. 发送18ms的低脉冲 */
	err = gpio_request(dht11.gpios[0].gpio, dht11.gpios[0].name);
	gpio_direction_output(dht11.gpios[0].gpio, 0);
	gpio_free(dht11.gpios[0].gpio);

	mdelay(18);
	gpio_direction_input(dht11.gpios[0].gpio);  /* 引脚变为输入方向, 由上拉电阻拉为1 */

	/* 2. 注册中断 */
	err = request_irq(dht11.gpios[0].irq, dht11_isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, dht11.gpios[0].name, &dht11.gpios[0]);
	if (err) {
		printk("Failed to request IRQ\n");
		return -EIO;
	}
	
	/* 设置定时器超时时间为100ms */
	mod_timer(&dht11.gpios[0].key_timer, jiffies + msecs_to_jiffies(100));	

	/* 3. 休眠等待数据 */
	wait_event_interruptible(gpio_wait, !is_key_buf_empty());

	free_irq(dht11.gpios[0].irq, &dht11.gpios[0]);

	desc = gpio_to_desc(dht11.gpios[0].gpio);

	//printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

	/* 设置DHT11 GPIO引脚的初始状态: output 1 */
	err = gpio_request(dht11.gpios[0].gpio, dht11.gpios[0].name);
	if (err)
	{
		printk("%s %s %d, gpio_request err\n", __FILE__, __FUNCTION__, __LINE__);
	}
	gpio_direction_output(dht11.gpios[0].gpio, 1);
	gpio_free(dht11.gpios[0].gpio);


	/* 4. copy_to_user */
	kern_buf[0] = get_key();
	kern_buf[1] = get_key();

	//printk("get val : 0x%x, 0x%x\n", kern_buf[0], kern_buf[1]);
	if ((kern_buf[0] == (char)-1) && (kern_buf[1] == (char)-1))
	{
		//printk("get err val\n");
		return -EIO;
	}

	err = copy_to_user(buf, kern_buf, 2);
	
	return 2;
	

}

static int dht11_release (struct inode *inode, struct file *filp)
{
	return 0;
}


/* 设备操作函数 */
static struct file_operations dht11_fops = {
	.owner = THIS_MODULE,
	.open = dht11_open,
	.read = dht11_read,
    .release = dht11_release,
};


static void parse_dht11_datas(void)
{
	int i;
	u64 high_time;
	unsigned char data = 0;
	int bits = 0;
	unsigned char datas[5];
	int byte = 0;
	unsigned char crc;

	//printk("g_dht11_irq_cnt = %d\n", g_dht11_irq_cnt);
	/* 数据个数: 可能是81、82、83、84 */
	if (g_dht11_irq_cnt < 81)
	{
		/* 出错 */
		put_key(-1);
		put_key(-1);

		// 唤醒APP
		wake_up_interruptible(&gpio_wait);
		g_dht11_irq_cnt = 0;
		return;
	}

	// 解析数据
	for (i = g_dht11_irq_cnt - 80; i < g_dht11_irq_cnt; i+=2)
	{
		high_time = g_dht11_irq_time[i] - g_dht11_irq_time[i-1];

		data <<= 1;

		if (high_time > 50000) /* data 1 */
		{
			data |= 1;
		}

		bits++;

		if (bits == 8)
		{
			datas[byte] = data;
			data = 0;
			bits = 0;
			byte++;
		}
	}

	// 放入环形buffer
	crc = datas[0] + datas[1] + datas[2] + datas[3];
	if (crc == datas[4])
	{
		put_key(datas[0]);
		put_key(datas[2]);
	}
	else
	{
		//printk("dht11 crc err\n");
		put_key(-1);
		put_key(-1);
	}

	g_dht11_irq_cnt = 0;
	// 唤醒APP
	wake_up_interruptible(&gpio_wait);
}



/*
 * @description	: 驱动入口函数
 * @param 		: 无
 * @return 		: 无
 */
static int __init dht11_init(void)
{
	/* 1、构建设备号 */
	if (dht11.major) {
		dht11.devid = MKDEV(dht11.major, 0);
		register_chrdev_region(dht11.devid, DHT11_CNT, DHT11_NAME);
	} else {
		alloc_chrdev_region(&dht11.devid, 0, DHT11_CNT, DHT11_NAME);
		dht11.major = MAJOR(dht11.devid);
		dht11.minor = MINOR(dht11.devid);
	}

	/* 2、注册字符设备 */
	cdev_init(&dht11.cdev, &dht11_fops);
	cdev_add(&dht11.cdev, dht11.devid, DHT11_CNT);

	/* 3、创建类 */
	dht11.class = class_create(THIS_MODULE, DHT11_NAME);
	if (IS_ERR(dht11.class)) {
		return PTR_ERR(dht11.class);
	}

	/* 4、创建设备 */
	dht11.device = device_create(dht11.class, NULL, dht11.devid, NULL, DHT11_NAME);
	if (IS_ERR(dht11.device)) {
		return PTR_ERR(dht11.device);
	}
	
	/* 5、初始化按键 */
	gpios_init();
	return 0;
}

/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */
static void __exit dht11_exit(void)
{
	unsigned int i = 0;
    int count = sizeof(dht11.gpios)/sizeof(dht11.gpios[0]);
	/* 删除定时器 */
	del_timer_sync(&dht11.timer);	/* 删除定时器 */
		
	/* 释放中断和内存 */
	for (i = 0; i < count; i++) {
		free_irq(dht11.gpios[i].irq, &dht11);
		gpio_free(dht11.gpios[i].gpio);
		kfree(dht11.gpios[i].name);	/* 释放name字段内存 */
	}
	cdev_del(&dht11.cdev);
	unregister_chrdev_region(dht11.devid, DHT11_CNT);
	device_destroy(dht11.class, dht11.devid);
	class_destroy(dht11.class);
}

module_init(dht11_init);
module_exit(dht11_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("jiangguangbo");