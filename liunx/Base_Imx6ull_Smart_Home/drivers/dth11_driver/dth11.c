#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
//#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/semaphore.h>
#include <asm/mach/map.h>
#include <linux/timer.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/fs.h>

#define DTH11_CNT           1
#define DTH11_NAME          "dth11"

#define DTH11_IO_OUT        gpio_direction_output(dth11.gpio_dth11, 1)
#define DTH11_IO_IN         gpio_direction_input(dth11.gpio_dth11)
#define DTH11_IO_GET        gpio_get_value(dth11.gpio_dth11)
#define DTH11_IO_SET(bit)   gpio_set_value(dth11.gpio_dth11, bit)

struct dth11_dev{
    dev_t devid;                /* 设备号 */
    struct cdev cdev;           /* 字符设备 */
    struct class *class;        /* 类 */
    struct device *device;      /* 设备 */
    struct device_node *nd;     /* 设备树节点 */
    int major;                  /* 主设备号 */
    int minor;                  /* 次设备号 */
    int gpio_dth11;             /* DTH11 GPIO引脚 */
    u16 humidity;               /* 湿度 */
    u16 temperature;            /* 温度 */
};

struct dth11_dev dth11; /* DTH11设备结构体 */

/* 等待dth11的响应信号 */
/* 收到主机信号后dth11会拉低电平80us，在拉高电平80us */
static int dth11_wait_response(void)
{
    int timeout = 500; /* 超时时间500us，若超时未等到对应的电平表示失败 */
   
    /* 等待低电平 */
    while(DTH11_IO_GET && timeout){
        udelay(1); /* 等待1us */
        timeout--;
    }
    if(timeout <= 0){
        printk("dth11: wait for response timeout\n");
        return -ETIMEDOUT; /* 等待超时 */
    }

    /* 等待高电平 */
    while(!DTH11_IO_GET && timeout){
        udelay(1); /* 等待1us */
        timeout--;
    }
    if(timeout <= 0){
        printk("dth11: wait for response timeout\n");
        return -ETIMEDOUT; /* 等待超时 */
    }

    /* 等待高电平结束 */
    while(DTH11_IO_GET && timeout){
        udelay(1); /* 等待1us */
        timeout--;
    }
    if(timeout <= 0){
        printk("dth11: wait for response timeout\n");
        return -ETIMEDOUT; /* 等待超时 */
    }

    return 0; /* 成功 */
}


/* 主机和dth11起始的通信信号 */
static int dth11_start(void)
{
    int ret = 0;

    DTH11_IO_OUT;       /* 主机设置为输出模式,并拉高电平 */
    DTH11_IO_SET(0);    /* 主机拉低电平 */
    mdelay(25);         /* 主机最少拉低18ms，这里设置为25ms*/
    DTH11_IO_SET(1);    /* 主机拉高电平 */
    udelay(30);         /* 主机拉高要在20us~40us，这里设置为30us */
    DTH11_IO_IN;        /* 主机设置为输入模式，等待dth11的输入*/
    udelay(2);

    ret = dth11_wait_response(); /* 等待dth11的响应信号 */
    if(ret < 0) {
        printk("dth11: dth11 response failed\n");
        return ret; /* 响应失败 */
    }

    return 0; /* 成功 */
}

/* 读取一个字节的数据 */
/* 8bit湿度整数数据 + 8bit湿度小数数据 + 8bit温度整数数据 + 8bit温度小数数据 + 8bit校验和 */
static int dth11_read_byte(u8 *data)
{
    int timeout = 500; /* 超时时间500us */
    u8 byte = 0;
    u8 bit = 0;
    int i;

    /* dth11响应主机信号后是数据传输阶段，在一个bit的传输中会拉低电平，接着拉高电平，此时的高电平就代表着数据1或0 */
    for(i = 0; i < 8; i++){
        while(DTH11_IO_GET && timeout){     /* 等待低电平 */
            udelay(1); /* 等待1us */
            timeout--;
        }
        if(timeout <= 0){
            printk("dth11: wait for data timeout\n");
            return -ETIMEDOUT; /* 等待超时 */
        }

        while(!DTH11_IO_GET && timeout){    /* 等待高电平 */
            udelay(1); /* 等待1us */
            timeout--;
        }
        if(timeout <= 0){
            printk("dth11: wait for data timeout\n");
            return -ETIMEDOUT; /* 等待超时 */
        }
        udelay(40);             /* 在高电平40us时读取数据 */
          
        bit = DTH11_IO_GET;     /* 读取数据位 */

        byte = byte << 1;       /* 左移一位，准备存储下一个bit */

        /* 高电平持续26~28us表示数据“0”；持续70us表示数据“1” */
        /* 所以在40us是读取，如果是高电平就是1，低电平就是0 */
        if(bit){
            byte = byte | 0x01; 
        }
    }   
 
    *data = byte;   /* 将读取到的字节数据存储到data中 */

    return 0;       /* 成功 */
}

static int dth11_read_data(void)
{
    int ret = 0;
    u8 data[5] = {0}; 
    int i;

    ret = dth11_start();    /* 启动通信 */
    if(ret < 0) {
        printk("dth11: dth11 start failed\n");
        return ret; /* 启动失败 */
    }

    /* 读取五个字节的数据 */
    for(i = 0; i < 5; i++){
        ret = dth11_read_byte(&data[i]); 
        if(ret < 0) {
            printk("dth11: dth11 read byte failed\n");
            return ret; /* 读取失败 */
        }
    }

    /* 校验 */
    data[4] = data[0] + data[1] + data[2] + data[3]; /* 最后一个字节是校验和 */
    if(data[4] != (data[0] + data[1] + data[2] + data[3])){
        printk("dth11: dth11 data checksum error\n");
        return -EIO; /* 校验和错误 */
    }

    /* 将读取到的数据存储到dth11结构体中 */
    dth11.humidity = data[0]; /* 湿度整数数据 */
    dth11.temperature = data[2]; /* 温度整数数据 */

    return 0;
}

/* 初始化dth11io */
static int dth11_ioinit(void)
{
    dth11.nd = of_find_node_by_path("/dth11");
    if(dth11.nd == NULL) {
        printk("dth11: failed to find dth11 node in device tree\n");
        return -EINVAL;
    }

    dth11.gpio_dth11 = of_get_named_gpio(dth11.nd, "dth11-gpio", 0);
    if(dth11.gpio_dth11 < 0) {
        printk("dth11: failed to get dth11 gpio\n");
        return -EINVAL;
    }
    printk("dth11: gpio_dth11 = %d\n", dth11.gpio_dth11);

    gpio_request(dth11.gpio_dth11, "dth11-gpio");

    gpio_direction_output(dth11.gpio_dth11, 1);

    return 0;
}

static int dth11_open(struct inode *inode, struct file *filp)
{
    int ret = 0;

	filp->private_data = &dth11; 	

    ret = dth11_ioinit(); /* 初始化dth11io */
    if(ret < 0) {
        printk("dth11: failed to initialize dth11 io\n");
        return ret; /* 初始化失败 */
    }

	return 0;
}

static ssize_t dth11_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    int ret = 0;
    u8 databuf[2] = {0};

    dth11_read_data(); /* 读取dth11数据 */
    databuf[0] = dth11.humidity; /* 湿度 */
    databuf[1] = dth11.temperature; /* 温度 */

    ret = copy_to_user(buf, databuf, sizeof(databuf)); /* 将数据拷贝到用户空间 */
    if(ret < 0) {
        printk("dth11: failed to copy data to user space\n");
        return -EFAULT; /* 拷贝失败 */
    }

	return sizeof(databuf);
}

static int dth11_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static struct file_operations dth11_fops = {
    .owner = THIS_MODULE,
	.open = dth11_open,
	.read = dth11_read,
	.release = 	dth11_release,
};

/* 驱动入口 */
static int __init dth11_init(void)
{

    int ret = 0;

    /* 注册字符设备 */
    dth11.major = 0; /* 动态分配主设备号 */
    if(dth11.major) {
        dth11.devid = MKDEV(dth11.major, 0);
        ret = register_chrdev_region(dth11.devid, DTH11_CNT, DTH11_NAME);
    } else {
        ret = alloc_chrdev_region(&dth11.devid, 0, DTH11_CNT, DTH11_NAME);
        dth11.major = MAJOR(dth11.devid);
        dth11.minor = MINOR(dth11.devid);
    }
    if(ret < 0){
        printk("dth11: failed to register char device region\n");
        goto err_devid;
    }
    printk("dth11:  major = %d, minor = %d\n", dth11.major, dth11.minor);
    
    dth11.cdev.owner = THIS_MODULE;
    cdev_init(&dth11.cdev, &dth11_fops);
    ret = cdev_add(&dth11.cdev, dth11.devid, DTH11_CNT);
    if(ret){
        printk("dth11: failed to add cdev\n");
        goto err_cdev;
    }

    dth11.class = class_create(THIS_MODULE, DTH11_NAME);
    if(IS_ERR(dth11.class)){
        printk("dth11: failed to create class\n");
        ret = PTR_ERR(dth11.class);
        goto err_class;
    }

    dth11.device = device_create(dth11.class, NULL, dth11.devid, NULL, DTH11_NAME);
    if(IS_ERR(dth11.device)){
        printk("dth11: failed to create device\n");
        ret = PTR_ERR(dth11.device);
        goto err_device;
    }

    ret = dth11_ioinit();
    if(ret < 0) {
        printk("dth11: failed to initialize GPIO\n");
        goto err_ioinit;
    }

    return 0;

err_ioinit:
    device_destroy(dth11.class, dth11.devid);
err_device:
    class_destroy(dth11.class);
err_class:
    cdev_del(&dth11.cdev);
err_cdev:
    unregister_chrdev_region(dth11.devid, DTH11_CNT);
err_devid:
    return ret;
}

/* 驱动出口 */
static void __exit dth11_exit(void)
{   
    gpio_free(dth11.gpio_dth11);
    cdev_del(&dth11.cdev);
    unregister_chrdev_region(dth11.devid, DTH11_CNT);
    device_destroy(dth11.class, dth11.devid);
    class_destroy(dth11.class);
}

/* 驱动注册与注销 */
module_init(dth11_init);
module_exit(dth11_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("fanwenhao");
