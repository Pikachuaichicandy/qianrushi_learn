#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

int main(void)
{
    int fd;
    int value;
    int ret;
    
    // 使用非阻塞方式打开设备
    fd = open("/dev/hc_sr505", O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    printf("Waiting for motion detection...\n");

    while (1) {
        ret = read(fd, &value, sizeof(value));
        
        if (ret < 0) {
            // 非阻塞模式下，没有数据时会返回-1
            usleep(100000); // 等待100ms再重试
            continue;
        }
        
        printf("Motion Detected: %s\n", value ? "YES" : "NO");
        usleep(500000); // 添加延迟，避免输出太快
    }

    close(fd);
    return 0;
}