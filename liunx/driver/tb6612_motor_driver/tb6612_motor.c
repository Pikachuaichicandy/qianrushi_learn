#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio/consumer.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/pwm.h>
#include <linux/platform_device.h> // <--- 修正1：新增的关键头文件
#include <linux/uaccess.h>

#define TB6612_CMD_FORWARD  _IO('M', 1)
#define TB6612_CMD_BACKWARD _IO('M', 2)
#define TB6612_CMD_BRAKE    _IO('M', 3)
#define TB6612_CMD_PWM      _IOW('M', 4, int)

#define TB6612_CNT      1
#define TB6612_NAME     "tb6612"

#define TB6612_PWM_PERIOD 20000000

/* tb6612设备结构体 */
struct tb6612_device {
    dev_t devid;
    int major;                  // <--- 修正2：重新加回 major 成员
    int minor;                  // <--- 修正2：重新加回 minor 成员
    struct cdev cdev;
    struct class *class;
    struct device *device;
    struct pwm_device *pwm_dev;
    struct gpio_desc *gpiod_ain1;
    struct gpio_desc *gpiod_ain2;
};

struct tb6612_device tb6612;

static void set_pwm_duty(int duty_percent)
{
    int duty_ns = TB6612_PWM_PERIOD * duty_percent / 100;
    pwm_config(tb6612.pwm_dev, duty_ns, TB6612_PWM_PERIOD);
    pwm_enable(tb6612.pwm_dev);
}

static long tb6612_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int duty;

    switch(cmd){
        case TB6612_CMD_FORWARD:
            gpiod_set_value(tb6612.gpiod_ain1, 1);
            gpiod_set_value(tb6612.gpiod_ain2, 0);
            break;
        case TB6612_CMD_BACKWARD:
            gpiod_set_value(tb6612.gpiod_ain1, 0);
            gpiod_set_value(tb6612.gpiod_ain2, 1);
            break;
        case TB6612_CMD_BRAKE:
            gpiod_set_value(tb6612.gpiod_ain1, 0);
            gpiod_set_value(tb6612.gpiod_ain2, 0);
            break;
        case TB6612_CMD_PWM:
            if (copy_from_user(&duty, (int __user *)arg, sizeof(int))) {
                return -EFAULT;
            }
            if(duty < 0) duty = 0;
            if(duty > 100) duty = 100;
            set_pwm_duty(duty);
            break;
        default:
            return -EINVAL;
    }

    return 0;
}

static struct file_operations tb6612_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = tb6612_ioctl,
};

static int tb6612_probe(struct platform_device *pdev)
{
    int ret = 0;

    printk("tb6612 probe starting...\n");

    tb6612.pwm_dev = devm_pwm_get(&pdev->dev, NULL);
    if (IS_ERR(tb6612.pwm_dev)) {
        dev_err(&pdev->dev, "Failed to get PWM device\n");
        return PTR_ERR(tb6612.pwm_dev);
    }

    tb6612.gpiod_ain1 = devm_gpiod_get(&pdev->dev, "ain1", GPIOD_OUT_LOW);
    if (IS_ERR(tb6612.gpiod_ain1)) {
        dev_err(&pdev->dev, "Failed to get ain1-gpios\n");
        return PTR_ERR(tb6612.gpiod_ain1);
    }
    tb6612.gpiod_ain2 = devm_gpiod_get(&pdev->dev, "ain2", GPIOD_OUT_LOW);
    if (IS_ERR(tb6612.gpiod_ain2)) {
        dev_err(&pdev->dev, "Failed to get ain2-gpios\n");
        return PTR_ERR(tb6612.gpiod_ain2);
    }

    ret = alloc_chrdev_region(&tb6612.devid, 0, TB6612_CNT, TB6612_NAME);
    if(ret < 0){
        dev_err(&pdev->dev, "failed to register char device region\n");
        return ret;
    }
    tb6612.major = MAJOR(tb6612.devid);
    tb6612.minor = MINOR(tb6612.devid);
    printk("tb6612 major = %d, minor = %d\n", tb6612.major, tb6612.minor);

    cdev_init(&tb6612.cdev, &tb6612_fops);
    ret = cdev_add(&tb6612.cdev, tb6612.devid, TB6612_CNT);
    if(ret < 0){
        unregister_chrdev_region(tb6612.devid, TB6612_CNT);
        return ret;
    }

    tb6612.class = class_create(THIS_MODULE, TB6612_NAME);
    if (IS_ERR(tb6612.class)) {
        cdev_del(&tb6612.cdev);
        unregister_chrdev_region(tb6612.devid, TB6612_CNT);
        return PTR_ERR(tb6612.class);
    }

    tb6612.device = device_create(tb6612.class, NULL, tb6612.devid, NULL, TB6612_NAME);
    if (IS_ERR(tb6612.device)) {
        class_destroy(tb6612.class);
        cdev_del(&tb6612.cdev);
        unregister_chrdev_region(tb6612.devid, TB6612_CNT);
        return PTR_ERR(tb6612.device);
    }

    ret = pwm_config(tb6612.pwm_dev, 0, TB6612_PWM_PERIOD);
    if (ret < 0) {
        dev_err(&pdev->dev, "failed to configure PWM device\n");
        device_destroy(tb6612.class, tb6612.devid);
        class_destroy(tb6612.class);
        cdev_del(&tb6612.cdev);
        unregister_chrdev_region(tb6612.devid, TB6612_CNT);
        return ret;
    }
    pwm_enable(tb6612.pwm_dev);

    printk("tb6612 probe finished successfully.\n");
    return 0;
}

static int tb6612_remove(struct platform_device *pdev)
{
    printk("tb6612 remove starting...\n");
    
    pwm_disable(tb6612.pwm_dev);
    device_destroy(tb6612.class, tb6612.devid);
    class_destroy(tb6612.class);
    cdev_del(&tb6612.cdev);
    unregister_chrdev_region(tb6612.devid, TB6612_CNT);

    printk("tb6612 remove finished.\n");
    return 0;
}

static const struct of_device_id tb6612_of_match[] = {
    { .compatible = "smarthome, tb6612fng" },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, tb6612_of_match);

static struct platform_driver tb6612_driver = {
    .driver = {
        .name = TB6612_NAME,
        .of_match_table = tb6612_of_match,
    },
    .probe = tb6612_probe,
    .remove = tb6612_remove,
};

static int __init tb6612_init(void)
{
    return platform_driver_register(&tb6612_driver);
}

static void __exit tb6612_exit(void)
{
    platform_driver_unregister(&tb6612_driver);
}

module_init(tb6612_init);
module_exit(tb6612_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("fanwenhao");