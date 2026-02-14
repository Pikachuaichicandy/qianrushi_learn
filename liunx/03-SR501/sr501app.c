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
    int fd, retvalue;
    char *filename;
    int state;
    
    if(argc != 2){
        printf("Usage: %s <device>\n", argv[0]);
        printf("Example: %s /dev/sr501\n", argv[0]);
        return -1;
    }

    filename = argv[1];

    /* 打开设备 */
    fd = open(filename, O_RDWR);
    if(fd < 0){
        printf("file %s open failed!\r\n", argv[1]);
        return -1;
    }

    printf("SR501 Motion Sensor Test Program\n");
    printf("Press Ctrl+C to exit\n");
    printf("SR501状态: 0=无运动, 1=检测到运动\n\n");

    while(1){
        if(read(fd,&state,sizeof(state))==sizeof(state)){
            if(state == 1)
                printf("检测到运动! (状态: %d)\n", state);
            else
                printf("无运动 (状态: %d)\n", state);
        }
        else {
            printf("读取失败\n");
        }

        usleep(200000); // 等待0.2秒
    }

    close(fd);
    return 0;
}