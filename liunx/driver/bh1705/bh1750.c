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
#include <linux/semaphore.h>
#include <asm/mach/map.h>
#include <linux/timer.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/i2c.h>

#define BH1750_CNT      1
#define BH1750_NAME     "bh1750"

/* 芯片指令 */
#define POWER_ON            0x01    //通电
#define RESET               0x07    //复位
#define H_RESOLUTION_MODE   0x10    //连续高分辨率模式

struct bh1750_dev{
    dev_t devid;                //设备号
    struct cdev cdev;           //字符设备
    struct class *class;        //类
    struct device *device;      //设备
    struct device_node *nd;     //设备树节点
    int major;                  //主设备号
    int minor;                  //次设备号
    void *private_data;         //私有数据
};

struct bh1750_dev bh1750;

/* 初始化bh1750 */
int bh1750_device_init(void)
{
    unsigned char write_buf[1];

    /* 上电 */
    write_buf[0] = POWER_ON;
    i2c_master_send(bh1750.private_data, write_buf, sizeof(write_buf));

    /* 复位 */
    write_buf[0] = RESET;
    i2c_master_send(bh1750.private_data, write_buf, sizeof(write_buf));

    return 0;
}

/* 读取bh1750数据 */
int bh1750_read_data(unsigned short *data, struct file *filp)
{
    unsigned char send_buf[1];
    unsigned char recv_buf[2];
    struct i2c_msg msgs[1];
    struct bh1750_dev *dev = filp->private_data;
    struct i2c_client *client = (struct i2c_client *)dev->private_data;
    int ret;

    send_buf[0] = H_RESOLUTION_MODE; //设置高分辨率模式
    msgs[0].addr = client->addr; //设备地址
    msgs[0].flags = 0; //写操作
    msgs[0].len = sizeof(send_buf);
    msgs[0].buf = send_buf;

    ret = i2c_transfer(client->adapter, msgs, 1);
    
    msleep(180); //等待数据准备好，180ms

    msgs[0].addr = client->addr; //设备地址
    msgs[0].flags = I2C_M_RD; //读操作
    msgs[0].len = sizeof(recv_buf);
    msgs[0].buf = recv_buf;

    ret = i2c_transfer(client->adapter, msgs, 1);


    *data = (recv_buf[0] << 8) | recv_buf[1];   //将两个字节合并为一个16位数据

    return sizeof(data);
}

static int bh1750_open(struct inode *inode, struct file *filp)
{
	filp->private_data = &bh1750; 	

	return 0;
}

static ssize_t bh1750_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    unsigned short data;
    int ret;

    bh1750_read_data(&data, filp);

    ret = copy_to_user(buf, &data, sizeof(data));
    if(ret < 0) {
        printk("bh1750: failed to copy data to user space\n");
        return -EFAULT;
    }

	return 0;
}

static int bh1750_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static const struct file_operations bh1750_fops = {
    .owner      = THIS_MODULE,
    .open       = bh1750_open,        //打开设备
    .release    = bh1750_release,     //关闭设备
    .read       = bh1750_read,        //读数据
};

static int bh1750_probe(struct i2c_client *client, const struct i2c_device_id *id) {
   
    int ret;

    /* 创建设备号 */
    bh1750.major = 0;       //由系统自动分配
    if(bh1750.major) {
        bh1750.devid = MKDEV(bh1750.major, 0);
        ret = register_chrdev_region(bh1750.devid, BH1750_CNT, BH1750_NAME);
    } else {
        ret = alloc_chrdev_region(&bh1750.devid, 0, BH1750_CNT, BH1750_NAME);
        bh1750.major = MAJOR(bh1750.devid);
        bh1750.minor = MINOR(bh1750.devid);
    }
    if(ret < 0) {
        printk("bh1750: failed to register char device region\n");
        return ret;
    }
    
    /* 注册设备 */
    bh1750.cdev.owner = THIS_MODULE;
    cdev_init(&bh1750.cdev, &bh1750_fops);
    cdev_add(&bh1750.cdev, bh1750.devid, BH1750_CNT);
    
    /* 创建类 */
    bh1750.class = class_create(THIS_MODULE, BH1750_NAME);
    if(IS_ERR(bh1750.class)) {
        unregister_chrdev_region(bh1750.devid, BH1750_CNT);
        printk("bh1750: failed to create class\n");
        return PTR_ERR(bh1750.class);
    }

    /* 创建设备 */
    bh1750.device = device_create(bh1750.class, NULL, bh1750.devid, NULL, BH1750_NAME);
    if(IS_ERR(bh1750.device)) {
        class_destroy(bh1750.class);    
        unregister_chrdev_region(bh1750.devid, BH1750_CNT);
        printk("bh1750: failed to create device\n");
        return PTR_ERR(bh1750.device);
    }

    bh1750.private_data = client;      //保存i2c_client指针

    bh1750_device_init();  //初始化bh1750设备

    return 0;
}

static int bh1750_remove(struct i2c_client *client) {
    
    /* 销毁设备 */
    cdev_del(&bh1750.cdev);
    unregister_chrdev_region(bh1750.devid, BH1750_CNT);
    device_destroy(bh1750.class, bh1750.devid);
    class_destroy(bh1750.class);
    bh1750.private_data = NULL; //清空私有数据

    return 0;
}

/* 传统id匹配列表 */
static const struct i2c_device_id bh1750_id[] = {
    {"bh1750", 0},
    {}
};

/* 设备树匹配列表 */
static const struct of_device_id bh1750_of_match[] = {
    { .compatible = "smarthome,bh1750" },
    { /* Sentinel */ }
};

/* i2c驱动结构体 */
static struct i2c_driver bh1750_driver = {
    .driver = {
        .name = "bh1750",
        .of_match_table = bh1750_of_match,
    },
    .probe = bh1750_probe,
    .remove = bh1750_remove,
    .id_table = bh1750_id,
};

static int __init bh1750_init(void){
    
    int ret;

    ret = i2c_add_driver(&bh1750_driver);
    
    return ret;
}

static void __exit bh1750_exit(void){
    
    i2c_del_driver(&bh1750_driver);
    
}

module_init(bh1750_init);
module_exit(bh1750_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Fanwenhao");