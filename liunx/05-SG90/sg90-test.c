#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
int main(int argc, char *argv[])
{
    int fd;
    int retvalue;
    unsigned char buffer[1];
    /* 检查参数数量 */
    if (argc != 3) {
        printf("Usage: %s <device> <angle(0-180)>\n", argv[0]);
        printf("Example: %s /dev/sg90 90\n", argv[0]);
        return -1;
    }
    /* 打开SG90设备 */
    fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        perror("open sg90 device failed");
        return -1;
    }
    
    printf("SG90 device opened successfully\n");
    buffer[0] = atoi(argv[2]); /* 从命令行参数获取角度值 */
    
    /* 向SG90设备写入角度数据 */
    retvalue = write(fd, buffer, 1);
    if (retvalue < 0) {
        perror("write to sg90 device failed");
        close(fd);
        return -1;
    }
    
    printf("Angle set successfully\n");
    
    /* 等待2秒观察舵机运动 */
    sleep(2);
    
    /* 关闭设备 */
    close(fd);
    printf("SG90 test completed\n");
    
    return 0;
}