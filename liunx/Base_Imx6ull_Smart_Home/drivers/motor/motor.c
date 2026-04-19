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
#include <linux/slab.h>  // 包含 kmalloc 和 kfree 的声明

#define MOTOR_CNT           1
#define MOTOR_NAME          "motor"

struct motor_dev {
    dev_t devid;
    struct cdev cdev;
    struct class *class;
    struct device *device;
    int major;
    int minor;
    int gpio_count;
    int *gpios;
    int motor_index;
};

static struct motor_dev motor;

/* 保持原有的马达引脚序列 */
static int g_motor_pin_ctrl[8] = {0x2, 0x3, 0x1, 0x9, 0x8, 0xc, 0x4, 0x6};

void set_pins_for_motor(int index)
{
    int i;
    for (i = 0; i < 4; i++) {
        gpio_set_value(motor.gpios[i], g_motor_pin_ctrl[index] & (1 << i) ? 1 : 0);
    }
}

void disable_motor(void)
{
    int i;
    for (i = 0; i < 4; i++) {
        gpio_set_value(motor.gpios[i], 0);
    }
}

/* 保持原有的write函数逻辑 */
static ssize_t motor_write(struct file *file, const char __user *buf, size_t cnt, loff_t *offt)
{
    int ker_buf[2];
    int err;
    int step;

    if (cnt != 8)
        return -EINVAL;

    err = copy_from_user(ker_buf, buf, cnt);
    if (err)
        return -EFAULT;

    if (ker_buf[0] > 0) {
        /* 逆时针旋转 - 保持原有逻辑 */
        for (step = 0; step < ker_buf[0]; step++) {
            set_pins_for_motor(motor.motor_index);
            mdelay(ker_buf[1]);
            motor.motor_index--;
            if (motor.motor_index == -1)
                motor.motor_index = 7;
        }
    } else {
        /* 顺时针旋转 - 保持原有逻辑 */
        ker_buf[0] = 0 - ker_buf[0];
        for (step = 0; step < ker_buf[0]; step++) {
            set_pins_for_motor(motor.motor_index);
            mdelay(ker_buf[1]);
            motor.motor_index++;
            if (motor.motor_index == 8)
                motor.motor_index = 0;
        }
    }

    /* 改进：旋转到位后让马达不再消耗电源 */
    disable_motor();

    return 8;
}

static ssize_t motor_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    u8 data = motor.motor_index;
    
    if (copy_to_user(buf, &data, sizeof(data)))
        return -EFAULT;
        
    return sizeof(data);
}

static int motor_open(struct inode *inode, struct file *filp)
{
    return 0;
}

static int motor_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static const struct file_operations motor_fops = {
    .owner = THIS_MODULE,
    .open = motor_open,
    .read = motor_read,
    .write = motor_write,
    .release = motor_release,
};

/* platform_driver结构 */
static int motor_probe(struct platform_device *pdev)
{
    int ret = 0;
    int i;
    struct device_node *np = pdev->dev.of_node;
    
    printk("motor: probe started\n");

    /* 从设备树获取GPIO信息 */
    motor.gpio_count = of_gpio_count(np);
    if (motor.gpio_count != 4) {
        printk("motor: expected 4 GPIOs, got %d\n", motor.gpio_count);
        return -EINVAL;
    }

    motor.gpios = kmalloc(motor.gpio_count * sizeof(int), GFP_KERNEL);
    if (!motor.gpios) {
        printk("motor: failed to allocate memory for GPIOs\n");
        return -ENOMEM;
    }

    for (i = 0; i < motor.gpio_count; i++) {
        motor.gpios[i] = of_get_gpio(np, i);
        if (motor.gpios[i] < 0) {
            printk("motor: failed to get gpio %d\n", i);
            ret = motor.gpios[i];
            goto err_gpio;
        }
        printk("motor: gpio[%d] = %d\n", i, motor.gpios[i]);
    }

    /* 申请并配置GPIO */
    for (i = 0; i < motor.gpio_count; i++) {
        ret = gpio_request(motor.gpios[i], "motor-gpio");
        if (ret) {
            printk("motor: failed to request gpio %d\n", motor.gpios[i]);
            goto err_gpio_request;
        }
        
        ret = gpio_direction_output(motor.gpios[i], 0);
        if (ret) {
            printk("motor: failed to set gpio %d as output\n", motor.gpios[i]);
            gpio_free(motor.gpios[i]);
            goto err_gpio_request;
        }
    }

    /* 注册字符设备 */
    ret = alloc_chrdev_region(&motor.devid, 0, MOTOR_CNT, MOTOR_NAME);
    if (ret < 0) {
        printk("motor: failed to alloc chrdev region\n");
        goto err_chrdev;
    }
    
    motor.major = MAJOR(motor.devid);
    motor.minor = MINOR(motor.devid);

    cdev_init(&motor.cdev, &motor_fops);
    motor.cdev.owner = THIS_MODULE;
    ret = cdev_add(&motor.cdev, motor.devid, MOTOR_CNT);
    if (ret) {
        printk("motor: failed to add cdev\n");
        goto err_cdev;
    }

    motor.class = class_create(THIS_MODULE, MOTOR_NAME);
    if (IS_ERR(motor.class)) {
        ret = PTR_ERR(motor.class);
        goto err_class;
    }

    motor.device = device_create(motor.class, NULL, motor.devid, NULL, MOTOR_NAME);
    if (IS_ERR(motor.device)) {
        ret = PTR_ERR(motor.device);
        goto err_device;
    }

    /* 初始化电机状态 */
    disable_motor();
    motor.motor_index = 0;
    
    printk("motor: driver probed successfully, %d GPIOs configured\n", motor.gpio_count);
    return 0;

err_device:
    class_destroy(motor.class);
err_class:
    cdev_del(&motor.cdev);
err_cdev:
    unregister_chrdev_region(motor.devid, MOTOR_CNT);
err_chrdev:
err_gpio_request:
    for (i = 0; i < motor.gpio_count; i++) {
        if (motor.gpios[i] >= 0)
            gpio_free(motor.gpios[i]);
    }
err_gpio:
    kfree(motor.gpios);
    return ret;
}

static int motor_remove(struct platform_device *pdev)
{
    int i;
    
    printk("motor: removing driver\n");

    /* 确保电机已关闭 */
    disable_motor();

    /* 清理设备节点 */
    device_destroy(motor.class, motor.devid);
    class_destroy(motor.class);
    cdev_del(&motor.cdev);
    unregister_chrdev_region(motor.devid, MOTOR_CNT);

    /* 释放GPIO */
    for (i = 0; i < motor.gpio_count; i++) {
        gpio_free(motor.gpios[i]);
    }

    kfree(motor.gpios);
    
    printk("motor: driver removed\n");
    return 0;
}

static const struct of_device_id motor_of_match[] = {
    { .compatible = "alientek,motor", },
    { /* sentinel */ }
};

static struct platform_driver motor_driver = {
    .probe = motor_probe,
    .remove = motor_remove,
    .driver = {
        .name = "motor_platform_driver",
        .of_match_table = motor_of_match,
    },
};

/* 保持您的入口出口函数结构 */
static int __init motor_drv_init(void)
{
    return platform_driver_register(&motor_driver);
}

static void __exit motor_drv_exit(void)
{
    platform_driver_unregister(&motor_driver);
}

module_init(motor_drv_init);
module_exit(motor_drv_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("fanwenhao");
