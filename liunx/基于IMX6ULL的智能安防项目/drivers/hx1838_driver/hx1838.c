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
#include <linux/platform_device.h> 
#include <linux/ktime.h>
#include <linux/interrupt.h>

#define HX1838_CNT     1
#define HX1838_NAME    "hx1838"

#define MAX_PULSES 70

/* hx1838设备结构体 */
struct hx1838_dev {
    struct cdev cdev;                       // 字符设备结构体
    struct class *class;                    // 设备类
    struct device *device;                  // 设备
    dev_t devid;                            // 设备号
    int major;                              // 主设备号
    int minor;                              // 次设备号
    int gpio_pin;                           // GPIO引脚号
    struct device_node *node;               // 设备树节点
    int irq_num;                            // 中断号
    wait_queue_head_t wq;                   // 等待队列头
    ktime_t last_time;
    u32 code;
    u32 bit_count;
    struct work_struct work;
    u8 key_code;
    int data_ready;
};

struct hx1838_dev hx1838;

struct signal_info{
    u32 intervals[MAX_PULSES];
    int count;
    ktime_t last_time;
    bool receiving;  // 添加接收状态标志
};

struct signal_info signal_data = {0};

/* 解码 */
static void decode_nec_signal(void)
{
    u8 address = 0, address_inv = 0;
    u8 command = 0, command_inv = 0;
    int i, bit_index = 0;

    // 检查数据量和起始信号
    /* 第一个间隔应该在9000us左右，第二个间隔应该在4500us左右 */
    if (signal_data.count < 66 ||
        signal_data.intervals[0] < 8500 || signal_data.intervals[0] > 9500 ||
        signal_data.intervals[1] < 4000 || signal_data.intervals[1] > 5000) {   
        return;
    }
        
    // 解码32位数据
    for (i = 2; i < 66 && bit_index < 32; i += 2) {
        
        /* 经过起始信号后，每个数据为由560us的低电平和560us或者1680us(取决于逻辑0还是逻辑1)高电平组成 */
        /* i + 1 就是判断一个数据位里的高电平长度 */
        u32 high_time = signal_data.intervals[i + 1];   
        
        int bit_value = (high_time > 1000) ? 1 : 0;
        
        /* 数据处理 */
        if (bit_index < 8) {
            address |= (bit_value << bit_index);
        } else if (bit_index < 16) {
            address_inv |= (bit_value << (bit_index - 8));
        } else if (bit_index < 24) {
            command |= (bit_value << (bit_index - 16));
        } else {
            command_inv |= (bit_value << (bit_index - 24));
        }     
        bit_index++;
    }
    
    /* 命令码存入设备结构体中 */
    hx1838.key_code = command;

    hx1838.data_ready = 1;                  /* 数据已准备好，可以读取 */

    wake_up_interruptible(&hx1838.wq);      /* 唤醒等待队列 */     
}

void hx1838_work_func_t(struct work_struct *work)
{
    decode_nec_signal();
    
    // 重置数据和状态
    signal_data.count = 0;
    signal_data.receiving = false;
    memset(signal_data.intervals, 0, sizeof(signal_data.intervals));
}

/* 中断处理函数 */
static irqreturn_t hx1838_irq_handler(int irq, void *dev_id)
{
    ktime_t now = ktime_get();  /* 获取当前时间 */
    u64 delta = 0;
    
    if (signal_data.count > 0) {
        delta = ktime_to_us(ktime_sub(now, signal_data.last_time));     /* 每个脉冲的间隔 */
        
        // 超时脉冲重置
        if (delta > 100000) {
            signal_data.count = 0;
            signal_data.receiving = false;
        }
    }
    
    // 将每个脉冲间隔存入脉冲间隔数组中去
    if (signal_data.count > 0 && signal_data.count < MAX_PULSES) {
        signal_data.intervals[signal_data.count - 1] = delta;       /* count为1，存入数组0位置 */
        
        // 检测起始信号
        if (signal_data.count == 1 && delta > 8000 && delta < 10000) {  /* 判断是否满足第一个电平的时间，应在9000us左右 */
            signal_data.receiving = true;
        }
    }
    
    signal_data.last_time = now;
    signal_data.count++;
    
    // 处理完整信号
    // hx1838一个完整信号有67次脉冲起始信号：9ms低电平 + 4.5ms高电平，32位数据：每位用低电平+高电平表示，结束信号：0.56ms低电平
    // 2 + 64 + 1 = 67
    if (signal_data.receiving && signal_data.count == 67) {
        schedule_work(&hx1838.work);    /* 工作队列处理复杂数据 */     
    }

    return IRQ_HANDLED;
}

/* read */
static ssize_t hx1838_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    u8 data;
    int ret;

    wait_event_interruptible(hx1838.wq, hx1838.data_ready);     /* 等待唤醒 */
    
    data = hx1838.key_code;
    
    hx1838.data_ready = 0;

    ret = copy_to_user(buf, &data, sizeof(data));
    if(ret < 0){
        return -EFAULT;
    }

    return sizeof(data);
}

/* 初始化io */
static int hx1838_ioinit(void)
{
    int ret = 0;

    /* 获取设备树节点 */
    hx1838.node = of_find_node_by_path("/hx1838");
    if (!hx1838.node) {
        printk("hx1838: device tree node not found!\n");
        return -ENODEV;
    }

    /* 获取Trig引脚GPIO编号 */
    hx1838.gpio_pin = of_get_named_gpio(hx1838.node, "hx1838-gpios", 0);
    if (hx1838.gpio_pin < 0) {
        printk("hx1838: failed to get out GPIO!\n");
        return -EINVAL;
    }

    /* 请求GPIO */    
    ret = gpio_request(hx1838.gpio_pin, "hx1838-gpios");
    if(ret < 0) {
        printk("hx1838: request trigger GPIO failed!\n");
        return ret;
    }

    /* 设置GPIO为输入模式 */
    ret = gpio_direction_input(hx1838.gpio_pin);
    if (ret < 0) {
        printk("hx1838: set GPIO direction failed!\n");
        gpio_free(hx1838.gpio_pin);
        return ret;
    }

    hx1838.irq_num = gpio_to_irq(hx1838.gpio_pin);
    if (hx1838.irq_num < 0) {
        printk("hx1838: failed to get IRQ number for echo GPIO!\n");
        gpio_free(hx1838.gpio_pin);
        return hx1838.irq_num;
    }

    /* 申请中断 */
    ret = request_irq(hx1838.irq_num, hx1838_irq_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, HX1838_NAME, &hx1838);
    if (ret) {
        printk("hx1838: request irq failed!\n");
        gpio_free(hx1838.gpio_pin);
        return ret;
    }

    /* 初始化等待队列头 */
    init_waitqueue_head(&hx1838.wq);

    return 0;
}

/* 操作集合 */
static const struct file_operations hx1838_fops = {
    .owner      = THIS_MODULE,
    .read       = hx1838_read,
};

/* 设备树匹配列表 */
static const struct of_device_id hx1838_of_match[] = {
    { .compatible = "smarthome, hx1838", },
    { /* sentinel */ }
};

static int hx1838_probe(struct platform_device *dev)
{
    int ret = 0;

    printk("hx1838 driver and device was matched!\n");
    
    /* 系统申请设备号 */
    hx1838.major = 0; 
    if(hx1838.major){
        hx1838.devid = MKDEV(hx1838.major, 0);
        ret = register_chrdev_region(hx1838.devid, HX1838_CNT, HX1838_NAME);
    } else {
        ret = alloc_chrdev_region(&hx1838.devid, 0, HX1838_CNT, HX1838_NAME);
        hx1838.major = MAJOR(hx1838.devid);
        hx1838.minor = MINOR(hx1838.devid);
    }
    if (ret < 0) {
        printk("hx1838: register chrdev failed!\n");
        goto err_devid;
    }
    printk("hx1838 major = %d, minor = %d\n", hx1838.major, hx1838.minor);

    /* 初始化cdev */
    hx1838.cdev.owner = THIS_MODULE;
    cdev_init(&hx1838.cdev, &hx1838_fops); 
    ret = cdev_add(&hx1838.cdev, hx1838.devid, HX1838_CNT);
    if (ret) {
        printk("hx1838: cdev add failed!\n");
        goto err_cdev;
    }

    /* 创建类 */
    hx1838.class = class_create(THIS_MODULE, HX1838_NAME);
    if (IS_ERR(hx1838.class)) {
        printk("hx1838: class create failed!\n");
        ret = PTR_ERR(hx1838.class);
        goto err_class;
    }

    /* 创建设备 */
    hx1838.device = device_create(hx1838.class, NULL, hx1838.devid, NULL, HX1838_NAME);
    if (IS_ERR(hx1838.device)) {
        printk("hx1838: device create failed!\n");
        ret = PTR_ERR(hx1838.device);
        goto err_device;
    }

    ret = hx1838_ioinit();
    if(ret < 0) {
        printk("hx1838: IO initialization failed!\n");
        goto err_dioinit;
    }

    INIT_WORK(&hx1838.work, hx1838_work_func_t);

    return 0;

err_dioinit:
    device_destroy(hx1838.class, hx1838.devid);
    gpio_free(hx1838.gpio_pin);
    free_irq(hx1838.irq_num, &hx1838);
err_device:
    class_destroy(hx1838.class);
err_class:
    cdev_del(&hx1838.cdev);
err_cdev:
    unregister_chrdev_region(hx1838.devid, HX1838_CNT);
err_devid:
    return ret; 
}

static int hx1838_remove(struct platform_device *dev)
{
    printk("hx1838 driver and device was removed!\n");
   
    gpio_free(hx1838.gpio_pin);
    free_irq(hx1838.irq_num, &hx1838);
    cdev_del(&hx1838.cdev);
    unregister_chrdev_region(hx1838.devid, HX1838_CNT);
    device_destroy(hx1838.class, hx1838.devid);
    class_destroy(hx1838.class);

    return 0;
}

/* platform驱动结构体 */
static struct platform_driver hx1838_driver = {
    .driver = {
        .name = "hx1838",
        .of_match_table = hx1838_of_match,
    },
    .probe = hx1838_probe,
    .remove = hx1838_remove,
};


/* 模块入口 */
static int __init hx1838_init(void)
{
    return platform_driver_register(&hx1838_driver);
}

/* 模块出口 */
static void __exit hx1838_exit(void)
{
    platform_driver_unregister(&hx1838_driver);
}

/* 注册和卸载模块 */
module_init(hx1838_init);
module_exit(hx1838_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("fanwenhao");