#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <string.h>
#include <sys/mman.h>
#include <signal.h>
#include <poll.h>
#include <linux/fb.h>
#include <stdlib.h>
#include <string.h>
#include <QPixmap>

#define video_width  680
#define video_height 480

class camera : public QObject
{
    Q_OBJECT

public:
    camera();
    ~camera();

    int videoFd;              //摄像头设备描述符
    char *userbuff[4];        //存放摄像头设备内核缓冲区映射后的用户内存地址
    int userbuff_length[4];   //保存映射后的数据长度，释放缓存时要用

    int cameraOpen();
    int cameraClose();

signals:
    void sendPix(QPixmap &frame);

public slots:
    void videoShow();
};

#endif // CAMERA_H
