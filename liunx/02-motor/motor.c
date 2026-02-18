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
#define GPIO_NUM            4           /*控制引脚总数*/
#define CMD_TRIG            3


/* gpio_info 结构体 */
struct gpio_info {
    int gpio; 
    char name[NAME_MAX];      /* GPIO 名称 */
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

/*马达引脚设置数组*/
static int motor_pin_ctr[8]= {0x2,0x3,0x1,0x9,0x8,0xc,0x4,0x6};
static int motor_index = 0;

struct motor_dev motor;	/* motor设备 */

void set_pins_for_motor(int index)
{
	int i;
	for (i = 0; i < 4; i++)
	{
		gpio_set_value(motor.gpios[i].gpio,motor_pin_ctr[index]&(1<<i) ? 1 : 0);
	}
}

/*
 * @description		: 打开设备
 */
static int motor_open(struct inode *inode, struct file *filp)
{
	filp->private_data = &motor;
	return 0;
}

/*
 * @description		: 向设备写数据 
 */
/*
Int buf[2]
buf[0] = 步进的次数 > 0 ：逆时针；<0:顺时针步进
Buf[1] = medlay；
*/
static ssize_t motor_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
	int ker_buf[2];
	int err;
	int step;
	if(cnt !=8)
		return -EINVAL;
	err =copy_from_user(ker_buf,buf,cnt);
	if(ker_buf[0]>0){
		/*ni shi zhen*/
		for (step =0;step <ker_buf[0];step++)
		{
			set_pins_for_motor(motor_index);
			mdelay(ker_buf[1]);
			motor_index--;
			if(motor_index == -1)
				motor_index = 7;
		}
	}
	else
	{
		/*shun shi zhen*/
		ker_buf[0] = 0 -ker_buf[0];
		for (step =0;step <ker_buf[0];step++)
		{
			set_pins_for_motor(motor_index);
			mdelay(ker_buf[1]);
			motor_index++;
			if(motor_index == 8)
				motor_index = 0;
		}
	}
	return 0;
}

/*
 * @description		: 关闭/释放设备
 */
static int motor_release(struct inode *inode, struct file *filp)
{
	return 0;
}


/* 设备操作函数 */
static struct file_operations motor_fops= {
	.owner = THIS_MODULE,
	.open = motor_open,
	.write = motor_write,
	.release = motor_release,
};

/*
 * @description	: 驱动入口函数
 */
static int __init motor_init(void)
{
	int ret = 0;
	unsigned char i = 0;

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
		motor.gpios[i].gpio = of_get_named_gpio(motor.nd, "led-gpios", i);
		if(motor.gpios[i].gpio < 0) {
			printk(KERN_ERR "can't get gpios[%d], error code: %d\n", i, motor.gpios[i].gpio);
			ret = -EINVAL;
			goto fail_get_gpio;
		}
	}

	/* 3、申请GPIO */
	for(i = 0; i < GPIO_NUM; i++){
		memset(motor.gpios[i].name, 0, sizeof(motor.gpios[i].name));
		sprintf(motor.gpios[i].name, "motor_GPIO%d", i);		/* 组合名字 */
		ret = gpio_request(motor.gpios[i].gpio, motor.gpios[i].name);
		if(ret < 0) {
			printk(KERN_ERR "failed to request gpio[%d]\r\n", i);
			goto fail_gpio_request;
		}
	}

	/* 4、设置GPIO方向 */
	for(i = 0; i < GPIO_NUM; i++){
		ret = gpio_direction_output(motor.gpios[i].gpio, 0); // 初始化为低电平
		if(ret < 0) {
			printk(KERN_ERR "can't set gpio[%d] as output!\r\n", i);
			goto fail_gpio_dir;
		}
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