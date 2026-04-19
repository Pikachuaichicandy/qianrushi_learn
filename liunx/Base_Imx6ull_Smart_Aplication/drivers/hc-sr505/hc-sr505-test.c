#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

int main(void)
{
    int fd;
    int value;


    fd = open("/dev/hc_sr505", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return -1;
    }



    printf("Waiting for motion detection...\n");

    while (1) {


        read(fd, &value, sizeof(value));
        printf("Motion Detected: %s\n", value ? "YES" : "NO");


    }

    close(fd);
    return 0;
}
