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
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/fs.h>
#define HC_SR04_CNT     1
#define HC_SR04_NAME    "hc_sr04"

/* 设备结构体 */
struct hc_sr04_device {
    int major;
    int minor;
    dev_t devid;                        /* 设备号 */
    struct device *device;              /* 设备结构体 */
    struct cdev cdev;                   /* 字符设备结构体 */
    struct class *class;                /* 设备类 */
    struct device_node *node;           /* 设备树节点 */
    int trig_gpio;                      /* Trig引脚GPIO编号 */
    int echo_gpio;                      /* Echo引脚GPIO编号 */
    int irq_num;                        /* 中断号 */
    u8 timeout;                         /* 超时时间 */
    wait_queue_head_t wait_queue;       /* 等待队列头 */
    int echo_captured;                  /* 判断等待条件标志 */
    ktime_t start_time, end_time;       /* 计时变量 */
};

struct hc_sr04_device hc_sr04;      /* hc-sr04设备结构体 */

/* 中断处理函数 */
static irqreturn_t hc_sr04_irq_handler(int irq, void *dev_id)
{
    int echo_value;

    /* hc_sr04获取距离是靠echo高电平时间计算 */
    /* echo上升沿触发中断时开始计时 */
    echo_value = gpio_get_value(hc_sr04.echo_gpio);
    if(echo_value){
        hc_sr04.start_time = ktime_get();
    }else{                                              /* 下降沿触发获取结束时间 */
        hc_sr04.end_time = ktime_get();
        hc_sr04.echo_captured = 1;                      /* 设置标志位，表示已捕获到echo信号 */
        wake_up_interruptible(&hc_sr04.wait_queue);     /* 唤醒等待队列 */
    }

    return IRQ_HANDLED;
}

/* ioctl */
static long hc_sr04_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    int duration;   /* 高电平持续时间 */
    int distance;   /* 距离值 */
    
    /* 发送检测信号 10us以上 */
    gpio_set_value(hc_sr04.trig_gpio, 1);  /* 设置Trig引脚为高电平 */
    udelay(15);                            /* 延时15us */
    gpio_set_value(hc_sr04.trig_gpio, 0);  /* 设置Trig引脚为低电平 */

    hc_sr04.echo_captured = 0;              /* 标志位置为0 */
    
    /* 等待下降沿中断唤醒等待队列并且设置超时时间100ms */
    ret = wait_event_interruptible_timeout(hc_sr04.wait_queue, hc_sr04.echo_captured, msecs_to_jiffies(100));
    if(ret == 0) {
        printk("hc_sr04: wait for echo timeout!\n");
        return -ETIMEDOUT;  /* 超时 */
    } else if (ret < 0) {
        printk("hc_sr04: wait for echo interrupted!\n");
        return ret;  /* 被信号中断 */
    }

    /* 计算距离 */
    duration = ktime_to_us(hc_sr04.end_time) - ktime_to_us(hc_sr04.start_time);  /* 获取高电平持续时间 */

    distance = (duration * 340 * 100) / (2 * 1000000);      /* 声速340m/s，除以2是因为声波来回走的距离， duration是us 100是米到厘米 除1000000是us到s*/

    if (copy_to_user((int __user *)arg, &distance, sizeof(distance))) {  /* 将距离值拷贝到用户空间 */
        printk("hc_sr04: copy_to_user failed!\n");
        return -EFAULT;
    }

    return 0;
}

static int hc_sr04_ioinit(void)
{
    int ret = 0;

    /* 获取设备树节点 */
    /*struct device_node *nd;
    nd = of_find_compatible_node(NULL, NULL, "alientek,hc-sr04");
    if (nd == NULL) {
        printk("hc_sr04: failed to find device tree node by compatible\n");
             return -ENODEV;
    }*/
    hc_sr04.node = of_find_node_by_path("/hc_sr04");
    if (!hc_sr04.node) {
        printk("hc_sr04: device tree node not found!\n");
        return -ENODEV;
    }

    /* 获取Trig引脚GPIO编号 */
    hc_sr04.trig_gpio = of_get_named_gpio(hc_sr04.node, "trigger-gpios", 0);
    if (hc_sr04.trig_gpio < 0) {
        printk("hc_sr04: failed to get trigger GPIO!\n");
        return -EINVAL;
    }
    /* 获取Echo引脚GPIO编号 */
    hc_sr04.echo_gpio = of_get_named_gpio(hc_sr04.node, "echo-gpios", 0);
    if (hc_sr04.echo_gpio < 0) {
        printk("hc_sr04: failed to get echo GPIO!\n");
        return -EINVAL;
    }

    /* 请求GPIO */    
    ret = gpio_request(hc_sr04.trig_gpio, "trigger-gpios");
    if(ret < 0) {
        printk("hc_sr04: request trigger GPIO failed!\n");
        return ret;
    }
    ret = gpio_request(hc_sr04.echo_gpio, "echo-gpios");
    if(ret < 0) {
        printk("hc_sr04: request echo GPIO failed!\n");
        gpio_free(hc_sr04.trig_gpio);
        return ret;
    }

    /* 设置GPIO方向 */
    ret = gpio_direction_output(hc_sr04.trig_gpio, 0);  /* trig为输出 */
    if(ret < 0) {
        printk("hc_sr04: set trigger GPIO direction failed!\n");
        gpio_free(hc_sr04.trig_gpio);
        gpio_free(hc_sr04.echo_gpio);
        return ret;
    }

    ret = gpio_direction_input(hc_sr04.echo_gpio);  /* echo为输入 */
    if(ret < 0) {
        printk("hc_sr04: set echo GPIO direction failed!\n");
        gpio_free(hc_sr04.trig_gpio);
        gpio_free(hc_sr04.echo_gpio);
        return ret;
    }

    /* 设置中断 */
    /* 使用echo引脚的上升沿下降沿来触发中断 */
    /* 将echo的gpio编号映射为irq中断编号 */
    hc_sr04.irq_num = gpio_to_irq(hc_sr04.echo_gpio);
    if (hc_sr04.irq_num < 0) {
        printk("hc_sr04: failed to get IRQ number for echo GPIO!\n");
        gpio_free(hc_sr04.trig_gpio);
        gpio_free(hc_sr04.echo_gpio);
        return hc_sr04.irq_num;
    }

    /* 申请中断 */
    ret = request_irq(hc_sr04.irq_num, hc_sr04_irq_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, HC_SR04_NAME, NULL);
    if (ret) {
        printk("hc_sr04: request irq failed!\n");
        gpio_free(hc_sr04.trig_gpio);
        gpio_free(hc_sr04.echo_gpio);
        return ret;
    }

    return 0;
}

static const struct file_operations hc_sr04_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = hc_sr04_ioctl,
};

static int hc_sr04_probe(struct platform_device *dev)
{
    int ret = 0;

    printk("hc_sr04 driver and device was matched!\n");

    /* 注册字符设备 */
    /* 系统主动申请设备号 */
    hc_sr04.major = 0;
    if(hc_sr04.major) {
        hc_sr04.devid = MKDEV(hc_sr04.major, 0);
        ret = register_chrdev_region(hc_sr04.devid, HC_SR04_CNT, HC_SR04_NAME);
    } else {
        ret = alloc_chrdev_region(&hc_sr04.devid, 0, HC_SR04_CNT, HC_SR04_NAME);
        hc_sr04.major = MAJOR(hc_sr04.devid);
        hc_sr04.minor = MINOR(hc_sr04.devid);
    }
    if (ret < 0) {
        printk("hc_sr04: register chrdev failed!\n");
        goto err_devid;
    }
    printk("hc_sr04 major = %d, minor = %d\n", hc_sr04.major, hc_sr04.minor);

    /* 初始化cdev */
    hc_sr04.cdev.owner = THIS_MODULE;
    cdev_init(&hc_sr04.cdev, &hc_sr04_fops);
    ret = cdev_add(&hc_sr04.cdev, hc_sr04.devid, HC_SR04_CNT);
    if (ret) {
        printk("hc_sr04: cdev add failed!\n");
        goto err_cdev;
    }

    /* 创建类 */
    hc_sr04.class = class_create(THIS_MODULE, HC_SR04_NAME);
    if (IS_ERR(hc_sr04.class)) {
        printk("hc_sr04: class create failed!\n");
        ret = PTR_ERR(hc_sr04.class);
        goto err_class;
    }

    /* 创建设备 */
    hc_sr04.device = device_create(hc_sr04.class, NULL, hc_sr04.devid, NULL, HC_SR04_NAME);
    if (IS_ERR(hc_sr04.device)) {
        printk("hc_sr04: device create failed!\n");
        ret = PTR_ERR(hc_sr04.device);
        goto err_device;
    }

    ret = hc_sr04_ioinit();
    if(ret < 0) {
        printk("hc_sr04: IO initialization failed!\n");
        goto err_ioinit;
    }

    /* 初始化等待队列 */
    init_waitqueue_head(&hc_sr04.wait_queue);

    return 0;

err_ioinit:
    device_destroy(hc_sr04.class, hc_sr04.devid);
err_device:
    class_destroy(hc_sr04.class);
err_class:
    cdev_del(&hc_sr04.cdev);
err_cdev:
    unregister_chrdev_region(hc_sr04.devid, HC_SR04_CNT);
err_devid:
    return ret;
}

static int hc_sr04_remove(struct platform_device *dev)
{
    printk("hc_sr04 driver and device was removed!\n");

    gpio_free(hc_sr04.trig_gpio);
    gpio_free(hc_sr04.echo_gpio);
    free_irq(hc_sr04.irq_num, NULL);
    cdev_del(&hc_sr04.cdev);
    unregister_chrdev_region(hc_sr04.devid, HC_SR04_CNT);
    device_destroy(hc_sr04.class, hc_sr04.devid);
    class_destroy(hc_sr04.class);

    return 0;
}

/* 设备树匹配列表 */
static const struct of_device_id hc_sr04_of_match[] = {
    { .compatible = "alientek, hc-sr04", },
    { /* sentinel */ }
};

/* platform驱动结构体 */
static struct platform_driver hc_sr04_driver = {
    .driver = {
        .name = "hc_sr04",
        .of_match_table = hc_sr04_of_match,
    },
    .probe = hc_sr04_probe,
    .remove = hc_sr04_remove,
};

/* 驱动入口 */
static int __init hc_sr04_init(void)
{
    return platform_driver_register(&hc_sr04_driver);
}

/* 驱动出口 */
static void __exit hc_sr04_exit(void)
{
    platform_driver_unregister(&hc_sr04_driver);
}

/* 驱动注册和注销 */
module_init(hc_sr04_init);
module_exit(hc_sr04_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("fanwenhao");
