KERNELDIR := /home/alientek/alpha/alientek-zzk/kernel-zzk
CURRENT_PATH := $(shell pwd)

obj-m := motor.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
