#include "people.h"
#include <QDebug>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <QThread>

// 静态成员变量初始化
int people::sg90_fd = -1;

people::people()
{

}

people::~people()
{
    stopThread();
}

void people::run()
{
    qDebug() << "舵机控制线程启动...";

    // 打开SG90设备
    if (sg90_fd < 0) {
        sg90_fd = open("/dev/sg90", O_RDWR);
        if (sg90_fd < 0) {
            qDebug() << "无法打开/dev/sg90设备, 错误:" << strerror(errno);
            return;
        }
        qDebug() << "SG90设备打开成功, fd:" << sg90_fd;

        // 初始化舵机到0度（关闭状态）
        int angle = 0;
        if (write(sg90_fd, &angle, sizeof(angle)) != sizeof(angle)) {
            qDebug() << "初始化舵机失败:" << strerror(errno);
        }
    }

    // 线程主循环（保持线程运行）
    while(!stop) {
        msleep(100);
    }

    qDebug() << "舵机控制线程停止";
}

// 静态方法：触发闸门打开
void people::triggerGateOpen()
{
    if (sg90_fd < 0) {
        qDebug() << "SG90设备未打开";
        return;
    }

    qDebug() << "=== 人脸认证成功，触发闸门 ===";

    // 1. 打开闸门到90度
    int angle = 90;
    if (write(sg90_fd, &angle, sizeof(angle)) != sizeof(angle)) {
        qDebug() << "打开闸门失败:" << strerror(errno);
        return;
    }
    qDebug() << "闸门已打开 (90度)";

    // 2. 等待5秒
    QThread::sleep(5);

    // 3. 关闭闸门到0度
    angle = 0;
    if (write(sg90_fd, &angle, sizeof(angle)) != sizeof(angle)) {
        qDebug() << "关闭闸门失败:" << strerror(errno);
        return;
    }
    qDebug() << "闸门已关闭 (0度)";
    qDebug() << "=== 闸门操作完成 ===";
}

void people::stopThread()
{
    stop = true;

    // 关闭设备
    if (sg90_fd >= 0) {
        close(sg90_fd);
        sg90_fd = -1;
        qDebug() << "SG90设备已关闭";
    }
}
