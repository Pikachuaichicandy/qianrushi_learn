#include "camera.h"
#include <QtGui/QScreen>
#include <QDateTime>
#include <QRandomGenerator>
#include <QDebug>


camera::camera()
{

}

camera::~camera()
{

}

int camera::cameraOpen()
{
    /* 打开摄像头设备 */
    videoFd = open("/dev/video1", O_RDWR);
    if(videoFd < 0){
        qDebug() << "打开摄像头失败!";
        return -1;
    }

    /* 设置摄像头类型为捕获、设置分辨率、视频采集格式 (VIDIOC_S_FMT) */
    struct v4l2_format format;
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;   /* 视频采集 */
    format.fmt.pix.width = video_width;          /* 宽 */
    format.fmt.pix.height = video_height;    	 /* 高 */
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;   /* 设置输出类型：MJPG */
    if(0 > ioctl(videoFd, VIDIOC_S_FMT, &format)){
        qDebug() << "设置摄像头参数失败！";
        close(videoFd);
        return -1;
    }

    /* 向内核申请内存 (VIDIOC_REQBUFS：个数、映射方式为mmap)
       将申请到的缓存加入内核队列 (VIDIOC_QBUF)
       将内核内存映射到用户空间 (mmap) */
    struct v4l2_requestbuffers requestbuffers;
    requestbuffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    requestbuffers.count = 4;    //申请缓存个数
    requestbuffers.memory = V4L2_MEMORY_MMAP;     //申请为物理连续的内存空间
    if(0 == ioctl(videoFd, VIDIOC_REQBUFS, &requestbuffers)){
        /* 申请到内存后 */
        for(int i = 0; i < requestbuffers.count; i++){
            /* 将申请到的缓存加入内核队列 (VIDIOC_QBUF)              */
            struct v4l2_buffer buffer;
            buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buffer.index = i;
            buffer.memory = V4L2_MEMORY_MMAP;
            if(0 == ioctl(videoFd, VIDIOC_QBUF, &buffer)){
                /* 加入内核队列成功后，将内存映射到用户空间 (mmap) */
                userbuff[i] = (char *)mmap(NULL, buffer.length, PROT_READ | PROT_WRITE, MAP_SHARED, videoFd, buffer.m.offset);
                userbuff_length[i] = buffer.length;
            }
        }
    }
    else{
        qDebug() << "申请内存失败";
        close(videoFd);
        return -1;
    }

    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(0 > ioctl(videoFd, VIDIOC_STREAMON, &type)){
        perror("打开视频流失败！");
        return -1;
    }

    return 0;
}

int camera::cameraClose()
{
    /* 停止采集，关闭视频流 (VIDIOC_STREAMOFF)
       关闭摄像头设备 & 关闭LCD设备 */
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(0 == ioctl(videoFd, VIDIOC_STREAMOFF, &type)){
        /* 释放映射 */
        for(int i = 0; i < 4; i++)
            munmap(userbuff[i], userbuff_length[i]);
        close(videoFd);
        printf("关闭相机成功!\n");
        return 0;
    }

    return -1;
}

void camera::videoShow()
{
    QPixmap pix;

    /* 采集图片数据 */
    //定义结构体变量，用于获取内核队列数据
    struct v4l2_buffer buffer;
    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    /* 从内核中捕获好的输出队列中取出一个 */
    if(0 == ioctl(videoFd, VIDIOC_DQBUF, &buffer)){
        /* 显示在label控件上 */
        //获取一帧显示
        pix.loadFromData((unsigned char *)userbuff[buffer.index], buffer.bytesused);
        emit sendPix(pix);
    }
    /* 将使用后的缓冲区放回到内核的输入队列中 (VIDIOC_QBUF) */
    if(0 > ioctl(videoFd, VIDIOC_QBUF, &buffer)){
        qDebug() << "返回队列失败";
    }
}
