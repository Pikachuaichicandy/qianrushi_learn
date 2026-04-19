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
#include <linux/pwm.h>
#include <linux/wait.h>
#include <linux/platform_device.h> 
#include <linux/ktime.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/sched.h>


#define HC_SR505_CNT     1
#define HC_SR505_NAME    "hc_sr505"

struct hc_sr505_device {
    int major;
    int minor;
    dev_t devid;                        /* 设备号 */
    struct device *device;              /* 设备结构体 */
    struct cdev cdev;                   /* 字符设备结构体 */
    struct class *class;                /* 设备类 */
    struct device_node *node;           /* 设备树节点 */
    int out_gpio;                       /* out引脚GPIO编号 */
    int irq_num;                        /* 中断号 */
    wait_queue_head_t hc_wq;
};

struct hc_sr505_device hc_sr505;

static int motion_detected = 0;

static irqreturn_t hc_sr505_irq_handler(int irq, void *dev_id)
{   
    // printk("irq ok\n");

    motion_detected = 1;
    wake_up_interruptible(&hc_sr505.hc_wq);
    return IRQ_HANDLED;
}

static int sr505_open(struct inode *inode, struct file *filp)
{

    return 0;
}

static int sr505_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static ssize_t sr505_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    int val;
   
    motion_detected = 0;

    // 等待中断唤醒
    wait_event_interruptible(hc_sr505.hc_wq, motion_detected);
    
    // 读取GPIO电平，写回用户空间
    val = gpio_get_value(hc_sr505.out_gpio);
    if (copy_to_user(buf, &val, sizeof(val)))
        return -EFAULT;

    return sizeof(val);
}

static int hc_sr505_ioinit(void)
{
    int ret = 0;

    /* 获取设备树节点 */
    hc_sr505.node = of_find_node_by_path("/hc_sr505");
    if (!hc_sr505.node) {
        printk("hc_sr505: device tree node not found!\n");
        return -ENODEV;
    }

    /* 获取Trig引脚GPIO编号 */
    hc_sr505.out_gpio = of_get_named_gpio(hc_sr505.node, "out-gpios", 0);
    if (hc_sr505.out_gpio < 0) {
        printk("hc_sr505: failed to get out GPIO!\n");
        return -EINVAL;
    }

    /* 请求GPIO */    
    ret = gpio_request(hc_sr505.out_gpio, "out-gpios");
    if(ret < 0) {
        printk("hc_sr505: request trigger GPIO failed!\n");
        return ret;
    }

    /* 设置GPIO为输入模式 */
    ret = gpio_direction_input(hc_sr505.out_gpio);
    if (ret < 0) {
        printk("hc_sr505: set GPIO direction failed!\n");
        gpio_free(hc_sr505.out_gpio);
        return ret;
    }

    hc_sr505.irq_num = gpio_to_irq(hc_sr505.out_gpio);
    if (hc_sr505.irq_num < 0) {
        printk("hc_sr505: failed to get IRQ number for echo GPIO!\n");
        gpio_free(hc_sr505.out_gpio);
        return hc_sr505.irq_num;
    }

    /* 申请中断 */
    ret = request_irq(hc_sr505.irq_num, hc_sr505_irq_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, HC_SR505_NAME, &hc_sr505);
    if (ret) {
        printk("hc_sr505: request irq failed!\n");
        gpio_free(hc_sr505.out_gpio);
        return ret;
    }

    init_waitqueue_head(&hc_sr505.hc_wq);

    return 0;
}

static const struct file_operations hc_sr505_fops = {
    .owner      = THIS_MODULE,
    .release    = sr505_release,
    .open       = sr505_open,
    .read       = sr505_read,
};

static int hc_sr505_probe(struct platform_device *dev)
{
    int ret = 0;

    printk("hc_sr505 driver and device was matched!\n");

    /* 注册字符设备 */
    /* 系统主动申请设备号 */
    hc_sr505.major = 0;
    if(hc_sr505.major) {
        hc_sr505.devid = MKDEV(hc_sr505.major, 0);
        ret = register_chrdev_region(hc_sr505.devid, HC_SR505_CNT, HC_SR505_NAME);
    } else {
        ret = alloc_chrdev_region(&hc_sr505.devid, 0, HC_SR505_CNT, HC_SR505_NAME);
        hc_sr505.major = MAJOR(hc_sr505.devid);
        hc_sr505.minor = MINOR(hc_sr505.devid);
    }
    if (ret < 0) {
        printk("hc_sr505: register chrdev failed!\n");
        goto err_devid;
    }
    printk("hc_sr505 major = %d, minor = %d\n", hc_sr505.major, hc_sr505.minor);

    /* 初始化cdev */
    hc_sr505.cdev.owner = THIS_MODULE;
    cdev_init(&hc_sr505.cdev, &hc_sr505_fops);
    ret = cdev_add(&hc_sr505.cdev, hc_sr505.devid, HC_SR505_CNT);
    if (ret) {
        printk("hc_sr505: cdev add failed!\n");
        goto err_cdev;
    }

    /* 创建类 */
    hc_sr505.class = class_create(THIS_MODULE, HC_SR505_NAME);
    if (IS_ERR(hc_sr505.class)) {
        printk("hc_sr505: class create failed!\n");
        ret = PTR_ERR(hc_sr505.class);
        goto err_class;
    }

    /* 创建设备 */
    hc_sr505.device = device_create(hc_sr505.class, NULL, hc_sr505.devid, NULL, HC_SR505_NAME);
    if (IS_ERR(hc_sr505.device)) {
        printk("hc_sr505: device create failed!\n");
        ret = PTR_ERR(hc_sr505.device);
        goto err_device;
    }

    ret = hc_sr505_ioinit();
    if(ret < 0) {
        printk("hc_sr505: IO initialization failed!\n");
        goto err_ioinit;
    }

    return 0;

err_ioinit:
    device_destroy(hc_sr505.class, hc_sr505.devid);
err_device:
    class_destroy(hc_sr505.class);
err_class:
    cdev_del(&hc_sr505.cdev);
err_cdev:
    unregister_chrdev_region(hc_sr505.devid, HC_SR505_CNT);
err_devid:
    return ret;
}

static int hc_sr505_remove(struct platform_device *dev)
{
    printk("hc_sr505 driver and device was removed!\n");

    gpio_free(hc_sr505.out_gpio);
    free_irq(hc_sr505.irq_num, &hc_sr505);
    cdev_del(&hc_sr505.cdev);
    unregister_chrdev_region(hc_sr505.devid, HC_SR505_CNT);
    device_destroy(hc_sr505.class, hc_sr505.devid);
    class_destroy(hc_sr505.class);

    return 0;
}

/* 设备树匹配列表 */
static const struct of_device_id hc_sr505_of_match[] = {
    { .compatible = "alientek, hc-sr505", },
    { /* sentinel */ }
};

/* platform驱动结构体 */
static struct platform_driver hc_sr505_driver = {
    .driver = {
        .name = "hc_sr505",
        .of_match_table = hc_sr505_of_match,
    },
    .probe = hc_sr505_probe,
    .remove = hc_sr505_remove,
};

/* 驱动入口 */
static int __init hc_sr505_init(void)
{
    return platform_driver_register(&hc_sr505_driver);
}

/* 驱动出口 */
static void __exit hc_sr505_exit(void)
{
    platform_driver_unregister(&hc_sr505_driver);
}

/* 驱动注册和注销 */
module_init(hc_sr505_init);
module_exit(hc_sr505_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("fanwenhao");