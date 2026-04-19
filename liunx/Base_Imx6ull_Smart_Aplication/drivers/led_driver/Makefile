# 1. 定义内核源码路径
KERNELDIR := /home/fanwenhao/linux/imx6ull/linux-5.15.161

# 2. 定义当前路径
CURRENT_PATH := $(shell pwd)

# 3. --- 新增内容开始 ---
#    定义 ARCH 和 CROSS_COMPILE 变量。
#    我们使用 ?= 是为了在外部环境变量已设置的情况下，不覆盖它，增加灵活性。
ARCH ?= arm
CROSS_COMPILE ?= arm-linux-gnueabihf-
# --- 新增内容结束 ---


# 4. 指定要编译成模块的源文件
obj-m := led.o


# 5. 定义默认的构建目标 (修正了拼写错误 bulid -> build)
build: kernel_module

# 6. 在这里将 ARCH 和 CROSS_COMPILE 传递给内核的Makefile
kernel_module:
	$(MAKE) -C $(KERNELDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(CURRENT_PATH) modules

# 7. clean命令也需要这些变量，以确保清理正确架构的文件
clean:	
	$(MAKE) -C $(KERNELDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(CURRENT_PATH) clean