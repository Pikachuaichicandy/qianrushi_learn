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
#include <linux/fs.h>

#define SG90_CNT      1
#define SG90_NAME     "sg90"

#define SG90_PWM_PERIOD 20000000

/* sg90设备结构体 */
struct sg90_device {
    dev_t devid;
    int major;                  // <--- 修正2：重新加回 major 成员
    int minor;                  // <--- 修正2：重新加回 minor 成员
    struct cdev cdev;
    struct class *class;
    struct device *device;
    struct pwm_device *pwm_dev;
};

struct sg90_device sg90;

static int sg90_open(struct inode *inode, struct file *filp)
{
	filp->private_data = &sg90; /* 设置私有数据 */
    printk("cdev test open\n");
    pwm_config(sg90.pwm_dev, 500000, SG90_PWM_PERIOD);
    pwm_set_polarity(sg90.pwm_dev,PWM_POLARITY_NORMAL);
    pwm_enable(sg90.pwm_dev);
	return 0;
}

static ssize_t sg90_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
	return 0;
}

static ssize_t sg90_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
	int retvalue;
	unsigned char databuf[1];
	unsigned char sg90stat;
	retvalue = copy_from_user(databuf, buf, cnt);
	if(retvalue < 0) {
		printk("kernel write failed!\r\n");
		return -EFAULT;
	}
	sg90stat = databuf[0];		/* 获取状态值 */
    pwm_config(sg90.pwm_dev, 500000+sg90stat*100000/9, SG90_PWM_PERIOD);
	return 0;
}

static int sg90_release(struct inode *inode, struct file *filp)
{
    pwm_config(sg90.pwm_dev, 500000, SG90_PWM_PERIOD);
    pwm_free(sg90.pwm_dev);
	return 0;
}

static struct file_operations sg90_fops = {
    .owner = THIS_MODULE,
    .open = sg90_open,
	.read = sg90_read,
	.write = sg90_write,
	.release = 	sg90_release,
};

static int sg90_probe(struct platform_device *pdev)
{
    int ret = 0;

    printk("sg90 probe starting...\n");

    sg90.pwm_dev = devm_pwm_get(&pdev->dev, NULL);
    if (IS_ERR(sg90.pwm_dev)) {
        dev_err(&pdev->dev, "Failed to get PWM device\n");
        return PTR_ERR(sg90.pwm_dev);
    }

    ret = alloc_chrdev_region(&sg90.devid, 0, SG90_CNT, SG90_NAME);
    if(ret < 0){
        dev_err(&pdev->dev, "failed to register char device region\n");
        return ret;
    }
    sg90.major = MAJOR(sg90.devid);
    sg90.minor = MINOR(sg90.devid);
    printk("sg90 major = %d, minor = %d\n", sg90.major, sg90.minor);

    cdev_init(&sg90.cdev, &sg90_fops);
    ret = cdev_add(&sg90.cdev, sg90.devid, SG90_CNT);
    if(ret < 0){
        unregister_chrdev_region(sg90.devid, SG90_CNT);
        return ret;
    }

    sg90.class = class_create(THIS_MODULE, SG90_NAME);
    if (IS_ERR(sg90.class)) {
        cdev_del(&sg90.cdev);
        unregister_chrdev_region(sg90.devid, SG90_CNT);
        return PTR_ERR(sg90.class);
    }

    sg90.device = device_create(sg90.class, NULL, sg90.devid, NULL, SG90_NAME);
    if (IS_ERR(sg90.device)) {
        class_destroy(sg90.class);
        cdev_del(&sg90.cdev);
        unregister_chrdev_region(sg90.devid, SG90_CNT);
        return PTR_ERR(sg90.device);
    }

    ret = pwm_config(sg90.pwm_dev, 0, SG90_PWM_PERIOD);
    if (ret < 0) {
        dev_err(&pdev->dev, "failed to configure PWM device\n");
        device_destroy(sg90.class, sg90.devid);
        class_destroy(sg90.class);
        cdev_del(&sg90.cdev);
        unregister_chrdev_region(sg90.devid, SG90_CNT);
        return ret;
    }
    pwm_enable(sg90.pwm_dev);

    printk("sg90 probe finished successfully.\n");
    return 0;
}

static int sg90_remove(struct platform_device *pdev)
{
    printk("sg90 remove starting...\n");
    
    pwm_disable(sg90.pwm_dev);
    device_destroy(sg90.class, sg90.devid);
    class_destroy(sg90.class);
    cdev_del(&sg90.cdev);
    unregister_chrdev_region(sg90.devid, SG90_CNT);

    printk("sg90 remove finished.\n");
    return 0;
}

static const struct of_device_id sg90_of_match[] = {
    { .compatible = "alientek,sg90" },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, sg90_of_match);

static struct platform_driver sg90_driver = {
    .driver = {
        .name = SG90_NAME,
        .of_match_table = sg90_of_match,
    },
    .probe = sg90_probe,
    .remove = sg90_remove,
};

static int __init sg90_init(void)
{
    return platform_driver_register(&sg90_driver);
}

static void __exit sg90_exit(void)
{
    platform_driver_unregister(&sg90_driver);
}

module_init(sg90_init);
module_exit(sg90_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("jiangguangbo");