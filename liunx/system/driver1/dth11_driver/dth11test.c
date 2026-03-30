#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int ret = 0;
    int fd;
    unsigned char buf[2];
    char *filename;

    if(argc != 2) {
        printf("error usage!!\n");
        return -1;
    }
 
    filename = argv[1];

    fd = open(filename, O_RDWR);
    if(fd < 0) {
        perror("open");
        return -1;
    }

    /* 读取数据 */
    while(1){
        ret = read(fd, buf, sizeof(buf));
        if(ret < 0) {
            printf("read failed\n");
            close(fd);
            return -1;
        }
        printf("Humidity: %d, Temperature: %d\n", buf[0], buf[1]);
        sleep(1); /* 每秒读取一次 */
    }

    close(fd);

    return 0;
}