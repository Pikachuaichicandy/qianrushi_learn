// spi_pin_test.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>  // 添加这个头文件
// GPIO操作函数
int gpio_export(int pin) {
    char buffer[64];
    int fd;
    
    snprintf(buffer, sizeof(buffer), "/sys/class/gpio/gpio%d", pin);
    if (access(buffer, F_OK) == 0) {
        return 0;  // 已经导出
    }
    
    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) return -1;
    
    snprintf(buffer, sizeof(buffer), "%d", pin);
    write(fd, buffer, strlen(buffer));
    close(fd);
    
    usleep(100000);  // 等待导出完成
    return 0;
}

int gpio_set_direction(int pin, const char *direction) {
    char path[64];
    int fd;
    
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) return -1;
    
    write(fd, direction, strlen(direction));
    close(fd);
    return 0;
}

int gpio_set_value(int pin, int value) {
    char path[64];
    int fd;
    
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) return -1;
    
    write(fd, value ? "1" : "0", 1);
    close(fd);
    return 0;
}

int main() {
    printf("=== SPI引脚详细测试 ===\n\n");
    
    // IMX6UL的GPIO编号计算: GPIO组号 * 32 + IO号
    // GPIO1_IO20 = 1 * 32 + 20 = 52
    int cs_pin = 52;    // GPIO1_IO20 (CS)
    // 注意：其他SPI引脚(MISO, MOSI, SCLK)通常不能直接作为GPIO操作
    // 因为它们被复用为SPI功能
    
    printf("1. 测试CS引脚 (GPIO1_IO20):\n");
    
    if (gpio_export(cs_pin) == 0) {
        printf("  成功导出GPIO%d\n", cs_pin);
        
        if (gpio_set_direction(cs_pin, "out") == 0) {
            printf("  设置方向为输出\n");
            
            // 测试CS引脚
            for (int i = 0; i < 5; i++) {
                gpio_set_value(cs_pin, 1);
                printf("  CS高电平\n");
                usleep(200000);
                
                gpio_set_value(cs_pin, 0);
                printf("  CS低电平\n");
                usleep(200000);
            }
        } else {
            printf("  设置方向失败\n");
        }
    } else {
        printf("  导出GPIO失败\n");
    }
    
    printf("\n2. 检查SPI引脚复用:\n");
    system("cat /sys/kernel/debug/pinctrl/20e0000.iomuxc/pinmux-pins | grep -i uart2");
    
    printf("\n3. 检查SPI设备状态:\n");
    system("ls -la /sys/bus/spi/devices/spi2.0/ 2>/dev/null");
    
    printf("\n4. 尝试直接内存映射测试（需要root）:\n");
    
    // 尝试直接读写SPI寄存器（高级调试）
    int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (mem_fd >= 0) {
        printf("  成功打开/dev/mem\n");
        
        // ECSPI3基地址: 0x02010000
        void *ecspi_base = mmap(0, 4096, PROT_READ | PROT_WRITE, 
                               MAP_SHARED, mem_fd, 0x02010000);
        if (ecspi_base != MAP_FAILED) {
            printf("  成功映射ECSPI3寄存器\n");
            
            // 读取CONREG寄存器 (偏移0x00)
            unsigned int *conreg = (unsigned int*)ecspi_base;
            printf("  CONREG = 0x%08X\n", *conreg);
            
            // 读取STATREG寄存器 (偏移0x04)
            unsigned int *statreg = (unsigned int*)(ecspi_base + 0x04);
            printf("  STATREG = 0x%08X\n", *statreg);
            
            munmap(ecspi_base, 4096);
        } else {
            printf("  映射ECSPI3寄存器失败\n");
        }
        close(mem_fd);
    } else {
        printf("  无法打开/dev/mem\n");
    }
    
    printf("\n5. 检查dmesg中的SPI相关消息:\n");
    system("dmesg | grep -i 'spi\\|ecspi' | tail -10");
    
    return 0;
}
