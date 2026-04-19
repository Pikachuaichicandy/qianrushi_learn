#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main()
{
    int fd = open("/dev/hc_sr04", O_RDWR);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    int distance;
    while(1){
        if (ioctl(fd, 0, &distance) == 0)
            printf("Distance: %d cm\n", distance);
        else
            perror("ioctl");

        sleep(1);
    }


    close(fd);
    return 0;
}