#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define TB6612_CMD_FORWARD  _IO('M', 1)
#define TB6612_CMD_BACKWARD _IO('M', 2)
#define TB6612_CMD_BRAKE    _IO('M', 3)
#define TB6612_CMD_PWM      _IOW('M', 4, int)

int main(int argc, char *argv[])
{
    int ret = 0;
    int fd;
    int pwm = 0;
    unsigned char buf[2];
    char *filename;
    
    if(argc != 2) {
        printf("error usage!!\n");
        return -1;
    }

    filename = argv[1];

    fd = open(filename, O_RDWR);
    if (fd < 0) {
        perror("open /dev/tb6612");
        return -1;
    }

    ioctl(fd, TB6612_CMD_FORWARD);
    printf("Motor forward\n");
    sleep(2);

    pwm = 50;
    ioctl(fd, TB6612_CMD_PWM, &pwm);
    printf("Set PWM 50%%\n");
    sleep(2);

    ioctl(fd, TB6612_CMD_BACKWARD);
    printf("Motor backward\n");
    sleep(2);

    pwm = 70;
    ioctl(fd, TB6612_CMD_PWM, &pwm);
    printf("Set PWM 70%%\n");
    sleep(2);

    ioctl(fd, TB6612_CMD_BRAKE);
    printf("Motor brake\n");
    sleep(2);

    close(fd);
    return 0;
}
