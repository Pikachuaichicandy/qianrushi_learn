#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fd;
    unsigned short data;
    float lux;

    fd = open("/dev/bh1750", O_RDONLY);
    if (fd < 0) {
        perror("open /dev/bh1750 failed");
        return -1;
    }

    while(1){
        read(fd, &data, sizeof(data)) != sizeof(data);


    
        // 根据数据手册，将结果除以1.2得到Lux值
        lux = (float)data / 1.2;
    
        printf("Raw data: %d\n", data);
        printf("Ambient Light: %.2f Lux\n", lux);

        sleep(1);  // 每秒读取一次数据
    }



    close(fd);
    return 0;
}