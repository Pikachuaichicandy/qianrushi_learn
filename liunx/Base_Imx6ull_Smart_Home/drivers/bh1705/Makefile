KERNELDIR := /home/fanwenhao/linux/imx6ull/linux-imx-rel_imx_4.1.15_2.1.0_ga_alientek

CURRENT_PATH := $(shell pwd)

obj-m := bh1750.o

bulid: kernel_module
kernel_module:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
clean:	
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean