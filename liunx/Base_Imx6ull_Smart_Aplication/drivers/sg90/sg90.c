/*
 * SG90舵机驱动
 * 使用PWM子系统控制SG90舵机角度
 * 作者: fanwenhao
 * 日期: 2024
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/pwm.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

/* 设备常量定义 */
#define SG90_CNT           1               /* 设备数量 */
#define SG90_NAME          "sg90"          /* 设备名称 */

/* SG90舵机设备结构体 */
struct sg90_dev {
    dev_t devid;                /* 设备号 */
    struct cdev cdev;           /* 字符设备结构体 */
    struct class *class;        /* 设备类 */
    struct device *device;      /* 设备实例 */
    int major;                  /* 主设备号 */
    int minor;                  /* 次设备号 */
    struct pwm_device *pwm;     /* PWM设备指针 */
    int current_angle;          /* 当前舵机角度 */
};

/* 全局设备实例 */
static struct sg90_dev sg90;

/* SG90舵机PWM参数定义 (单位: 纳秒ns) */
#define SG90_PWM_PERIOD_NS     20000000     /* PWM周期: 20ms = 20000000ns (50Hz) */
#define SG90_MIN_PULSE_NS      500000       /* 最小脉冲宽度: 0.5ms = 500000ns (对应0度) */
#define SG90_MAX_PULSE_NS      2500000      /* 最大脉冲宽度: 2.5ms = 2500000ns (对应180度) */

/**
 * sg90_set_angle - 设置SG90舵机角度
 * @angle: 目标角度 (0-180度)
 * 
 * 返回值: 0成功，负数错误码
 * 
 * 功能: 根据输入角度计算对应的PWM脉冲宽度，并配置PWM输出
 */
static int sg90_set_angle(int angle)
{
    int pulse_ns;   /* 脉冲宽度 (纳秒) */
    int ret;        /* 返回值 */
    
    /* 参数有效性检查: 限制角度在0-180度范围内 */
    if (angle < 0) 
        angle = 0;
    else if (angle > 180) 
        angle = 180;
    
    /* 
     * 计算对应角度的脉冲宽度
     * 公式: pulse_ns = MIN_PULSE + (angle * (MAX_PULSE - MIN_PULSE)) / 180
     * 线性映射: 0度->0.5ms, 180度->2.5ms
     */
    pulse_ns = SG90_MIN_PULSE_NS + 
               (angle * (SG90_MAX_PULSE_NS - SG90_MIN_PULSE_NS)) / 180;
    
    /* 配置PWM: 设置脉冲宽度和周期 */
    ret = pwm_config(sg90.pwm, pulse_ns, SG90_PWM_PERIOD_NS);
    if (ret) {
        printk(KERN_ERR "sg90: failed to config PWM, error: %d\n", ret);
        return ret;
    }
    
    /* 启用PWM输出 */
    ret = pwm_enable(sg90.pwm);
    if (ret) {
        printk(KERN_ERR "sg90: failed to enable PWM, error: %d\n", ret);
        return ret;
    }
    
    /* 更新当前角度状态 */
    sg90.current_angle = angle;
    
    /* 调试信息: 显示设置的角度和对应的脉冲宽度 */
    printk(KERN_DEBUG "sg90: set angle to %d degrees, pulse width: %d ns\n", 
           angle, pulse_ns);
    
    return 0;
}

/**
 * sg90_read - 读取当前舵机角度
 * @filp: 文件指针
 * @buf: 用户空间缓冲区
 * @cnt: 请求读取的字节数
 * @offt: 文件偏移量
 * 
 * 返回值: 成功读取的字节数，负数表示错误
 */
static ssize_t sg90_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    /* 检查缓冲区大小是否足够 */
    if (cnt < sizeof(sg90.current_angle)) {
        return -EINVAL;
    }
    
    /* 将当前角度复制到用户空间 */
    if (copy_to_user(buf, &sg90.current_angle, sizeof(sg90.current_angle))) {
        return -EFAULT;
    }
    
    return sizeof(sg90.current_angle);
}

/**
 * sg90_write - 设置舵机角度
 * @filp: 文件指针
 * @buf: 用户空间缓冲区（包含角度值）
 * @cnt: 写入的字节数
 * @offt: 文件偏移量
 * 
 * 返回值: 成功写入的字节数，负数表示错误
 */
static ssize_t sg90_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
    int angle;  /* 目标角度 */
    int ret;    /* 返回值 */
    
    /* 检查写入数据大小是否正确 */
    if (cnt != sizeof(angle)) {
        printk(KERN_WARNING "sg90: invalid write size %zu, expected %zu\n", 
               cnt, sizeof(angle));
        return -EINVAL;
    }
    
    /* 从用户空间读取角度值 */
    if (copy_from_user(&angle, buf, sizeof(angle))) {
        printk(KERN_ERR "sg90: failed to copy data from user space\n");
        return -EFAULT;
    }
    
    /* 设置舵机角度 */
    ret = sg90_set_angle(angle);
    if (ret) {
        return ret;
    }
        
    return sizeof(angle);
}

/**
 * sg90_open - 打开设备
 * @inode: 设备inode
 * @filp: 文件指针
 * 
 * 返回值: 0成功，负数错误码
 */
static int sg90_open(struct inode *inode, struct file *filp)
{
    printk(KERN_DEBUG "sg90: device opened\n");
    return 0;
}

/**
 * sg90_release - 关闭设备
 * @inode: 设备inode
 * @filp: 文件指针
 * 
 * 返回值: 0成功，负数错误码
 */
static int sg90_release(struct inode *inode, struct file *filp)
{
    /* 关闭设备时禁用PWM输出以节省功耗 */
    pwm_disable(sg90.pwm);
    printk(KERN_DEBUG "sg90: device closed\n");
    return 0;
}

/* 文件操作结构体定义 */
static struct file_operations sg90_fops = {
    .owner = THIS_MODULE,       /* 模块所有者 */
    .open = sg90_open,          /* 打开设备 */
    .read = sg90_read,          /* 读取角度 */
    .write = sg90_write,        /* 设置角度 */
    .release = sg90_release,    /* 关闭设备 */
};

/**
 * sg90_probe - 平台设备探测函数
 * @pdev: 平台设备指针
 * 
 * 返回值: 0成功，负数错误码
 * 
 * 功能: 当设备树匹配时调用，完成设备初始化
 */
static int sg90_probe(struct platform_device *pdev)
{
    int ret = 0;    /* 返回值 */
    
    printk(KERN_INFO "sg90: driver probe started\n");

    /* 
     * 获取PWM设备
     * 从设备树中获取PWM控制器引用
     */
    sg90.pwm = devm_pwm_get(&pdev->dev, NULL);
    if (IS_ERR(sg90.pwm)) {
        ret = PTR_ERR(sg90.pwm);
        printk(KERN_ERR "sg90: failed to get PWM device, error: %d\n", ret);
        return ret;
    }

    printk(KERN_INFO "sg90: PWM device acquired successfully\n");

    /* 动态分配字符设备号 */
    ret = alloc_chrdev_region(&sg90.devid, 0, SG90_CNT, SG90_NAME);
    if (ret < 0) {
        printk(KERN_ERR "sg90: failed to allocate char device region\n");
        return ret;
    }
    
    /* 记录主次设备号 */
    sg90.major = MAJOR(sg90.devid);
    sg90.minor = MINOR(sg90.devid);
    printk(KERN_INFO "sg90: allocated device number - major: %d, minor: %d\n", 
           sg90.major, sg90.minor);

    /* 初始化字符设备 */
    cdev_init(&sg90.cdev, &sg90_fops);
    sg90.cdev.owner = THIS_MODULE;
    
    /* 添加字符设备到系统 */
    ret = cdev_add(&sg90.cdev, sg90.devid, SG90_CNT);
    if (ret) {
        printk(KERN_ERR "sg90: failed to add char device\n");
        goto err_cdev;
    }

    /* 创建设备类 */
    sg90.class = class_create(THIS_MODULE, SG90_NAME);
    if (IS_ERR(sg90.class)) {
        ret = PTR_ERR(sg90.class);
        printk(KERN_ERR "sg90: failed to create device class\n");
        goto err_class;
    }

    /* 创建设备节点 /dev/sg90 */
    sg90.device = device_create(sg90.class, NULL, sg90.devid, NULL, SG90_NAME);
    if (IS_ERR(sg90.device)) {
        ret = PTR_ERR(sg90.device);
        printk(KERN_ERR "sg90: failed to create device node\n");
        goto err_device;
    }

    /* 初始化舵机到中间位置 (90度) */
    ret = sg90_set_angle(90);
    if (ret) {
        printk(KERN_ERR "sg90: failed to set initial angle\n");
        goto err_angle;
    }
    
    printk(KERN_INFO "sg90: driver probed successfully\n");
    return 0;

/* 错误处理路径 */
err_angle:
    device_destroy(sg90.class, sg90.devid);
err_device:
    class_destroy(sg90.class);
err_class:
    cdev_del(&sg90.cdev);
err_cdev:
    unregister_chrdev_region(sg90.devid, SG90_CNT);
    return ret;
}

/**
 * sg90_remove - 平台设备移除函数
 * @pdev: 平台设备指针
 * 
 * 返回值: 0成功
 * 
 * 功能: 当设备卸载时调用，清理所有资源
 */
static int sg90_remove(struct platform_device *pdev)
{
    /* 禁用PWM输出 */
    pwm_disable(sg90.pwm);
    
    /* 清理设备节点 */
    device_destroy(sg90.class, sg90.devid);
    
    /* 销毁设备类 */
    class_destroy(sg90.class);
    
    /* 删除字符设备 */
    cdev_del(&sg90.cdev);
    
    /* 释放设备号 */
    unregister_chrdev_region(sg90.devid, SG90_CNT);
    
    printk(KERN_INFO "sg90: driver removed successfully\n");
    return 0;
}

/* 设备树匹配表 */
static const struct of_device_id sg90_of_match[] = {
    { 
        .compatible = "alientek,sg90",    /* 设备树兼容性字符串 */
    },
    { /* sentinel */ }  /* 结束标记 */
};

/* 平台驱动结构体 */
static struct platform_driver sg90_driver = {
    .probe = sg90_probe,        /* 探测函数 */
    .remove = sg90_remove,      /* 移除函数 */
    .driver = {
        .name = "sg90_platform_driver",     /* 驱动名称 */
        .of_match_table = sg90_of_match,    /* 设备树匹配表 */
    },
};

/**
 * sg90_drv_init - 驱动初始化函数
 * 
 * 返回值: 0成功，负数错误码
 */
static int __init sg90_drv_init(void)
{
    int ret;
    
    /* 注册平台驱动 */
    ret = platform_driver_register(&sg90_driver);
    if (ret) {
        printk(KERN_ERR "sg90: failed to register platform driver\n");
        return ret;
    }
    
    printk(KERN_INFO "sg90: driver initialized successfully\n");
    return 0;
}

/**
 * sg90_drv_exit - 驱动退出函数
 */
static void __exit sg90_drv_exit(void)
{
    /* 注销平台驱动 */
    platform_driver_unregister(&sg90_driver);
    printk(KERN_INFO "sg90: driver exited\n");
}

/* 模块入口和出口 */
module_init(sg90_drv_init);
module_exit(sg90_drv_exit);

/* 模块信息 */
MODULE_LICENSE("GPL");              /* 模块许可证 */
MODULE_AUTHOR("fanwenhao");         /* 作者 */
MODULE_DESCRIPTION("SG90 Servo Motor Driver using PWM subsystem");  /* 描述 */
MODULE_VERSION("1.0");              /* 版本号 */
