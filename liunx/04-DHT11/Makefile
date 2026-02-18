KERNELDIR := /home/alientek/alpha/alientek-zzk/kernel-zzk/linux-imx-rel_imx_4.1.15_2.1.0_ga_alientek
MODULE_SYM_VERS := /usr/src/linux-headers-4.15.0-142-generic/Module.symvers

CURRENT_PATH := $(shell pwd)

obj-m := dht11.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
