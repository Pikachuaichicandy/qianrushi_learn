#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdint.h>

#define CMD_TRIG 3

int main(int argc, char *argv[])
{
    int fd;
    int buf[2];
    int ret;
    char *filename;
 
    if(argc != 4)
    {
        printf("Usage: %s <dev> <steps> <mdelay>\n", argv[0]);
        return -1;
    }

    filename = argv[1];

    /* 打开设备 */
    fd = open(filename, O_RDWR | O_NONBLOCK);  // 使用非阻塞模式
    if(fd == -1)
    {
        printf("file %s open failed!\r\n", argv[1]);
        return -1;
    }

    buf[0] = strtol(argv[2],NULL,0);
    buf[0] = strtol(argv[3],NULL,0);
    ret = write(fd,buf,8);
    close(fd);
    return 0;
}