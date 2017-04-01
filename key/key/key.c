#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <mach/regs-gpio.h>
#include <asm/io.h>
#include <linux/regulator/consumer.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define IRQ_DEBUG
#ifdef IRQ_DEBUG
#define DPRINTK(x...) printk("IRQ_CTL DEBUG:" x)
#else
#define DPRINTK(x...)
#endif

#define DRIVER_NAME "key_test"

static struct class *key_class;


static int key_drv_open(struct inode *inode, struct file *file)
{
 u8 ret;

    /*    ret = gpio_request(EXYNOS4_GPX1(1), "key_dev1");
        if (ret) {
                printk("request GPIO for key_dev1 failed,ret\n");
                return ret;
        }
*/
        s3c_gpio_cfgpin(EXYNOS4_GPX1(1), S3C_GPIO_SFN(0xF));
        s3c_gpio_setpull(EXYNOS4_GPX1(1), S3C_GPIO_PULL_UP);
        gpio_free(EXYNOS4_GPX1(1));

      // ret = gpio_request(EXYNOS4_GPX1(2), "key_dev2");
      //  if (ret) {
      //          printk("request GPIO  key_dev2  failed, ret \n");
      //          return ret;
       // }

        s3c_gpio_cfgpin(EXYNOS4_GPX1(2), S3C_GPIO_SFN(0xF));
        s3c_gpio_setpull(EXYNOS4_GPX1(2), S3C_GPIO_PULL_UP);
        gpio_free(EXYNOS4_GPX1(2));


        return 0;

}

ssize_t key_drv_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
	/* 返回4个引脚的电平 */
	unsigned char key_vals[2];
	int regval;
    unsigned long ret;
	if (size != sizeof(key_vals))
		return -EINVAL;

	regval = gpio_get_value(EXYNOS4_GPX1(1));
	key_vals[0] = regval;
	

	regval = gpio_get_value(EXYNOS4_GPX1(2));
	key_vals[1] = regval;

	ret=copy_to_user(buf, key_vals, sizeof(key_vals));
	
	return sizeof(key_vals);
}


static struct file_operations key_drv_fops = {
    .owner  =   THIS_MODULE,    /* 这是一个宏，推向编译模块时自动创建的__this_module变量 */
    .open   =   key_drv_open,     
	.read	=	key_drv_read,	   
};


int major;
static int key_drv_init(void)
{
	major = register_chrdev(0, "key_drv", &key_drv_fops);

	key_class = class_create(THIS_MODULE, "key_drv");

    device_create(key_class,NULL,MKDEV(major,0),NULL,"key");

	return 0;
}

static void key_drv_exit(void)
{
	unregister_chrdev(major, "key_drv");
	device_destroy(key_class,MKDEV(major,0));
	class_destroy(key_class);
}


module_init(key_drv_init);

module_exit(key_drv_exit);

MODULE_LICENSE("GPL");

