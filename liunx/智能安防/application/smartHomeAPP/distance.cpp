#include "distance.h"
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

// 在distance.cpp中添加简单的互斥保护
static QMutex distanceMutex;

void distance::run()
{
    qDebug() << "HC-SR04距离传感器线程启动";
    
    // 使用互斥锁保护测量过程
    distanceMutex.lock();
    
    //读取距离数据
    int fd;
    fd = open("/dev/hc_sr04", O_RDWR);
    if(fd < 0){
        qDebug() << "HC-SR04设备打开失败!";
        distanceMutex.unlock();
        return;
    }

    qDebug() << "HC-SR04设备打开成功";
    
    int tempDistance = 0;
    int ret = ioctl(fd, 0, &tempDistance);
    
    if(ret < 0){
        qDebug() << "HC-SR04 ioctl失败，错误码:" << ret;
    } else {
        qDebug() << "HC-SR04原始数据:" << tempDistance;
        
        // 数据验证
        if(tempDistance >= 2 && tempDistance <= 400) {
            distanceData = tempDistance;
        } else {
            // 数据异常处理
            distanceData = 0;
        }
        
        qDebug() << "最终距离数据:" << distanceData << "cm";
    }
    
    close(fd);
    distanceMutex.unlock();
    
    // 线程完成任务后退出
    qDebug() << "HC-SR04测量完成";
}

void distance::distanceStop()
{
    stop = 1;
}