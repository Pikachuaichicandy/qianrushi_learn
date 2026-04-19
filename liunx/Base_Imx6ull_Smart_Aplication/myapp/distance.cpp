#include "distance.h"
#include <QFile>
#include <QDebug>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
distance::distance()
{

}
distance::~distance()
{

}

void distance::run()
{
    //读取距离数据
    int fd;
    fd = open("/dev/hc_sr04", O_RDWR);
    if(fd < 0){
        qDebug() << "hc_sr04 open failed!";
    }

    while(!stop){
        ioctl(fd, 0, &distanceData);
        msleep(1000);
    }
    close(fd);
}

void distance::distanceStop()
{
    stop = 1;
}
