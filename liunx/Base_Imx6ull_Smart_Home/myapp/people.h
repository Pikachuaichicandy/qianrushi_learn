#ifndef PEOPLE_H
#define PEOPLE_H

#include <QThread>

class people : public QThread
{
    Q_OBJECT
public:
     people();
    ~people();

    void run() override;

    // 触发闸门打开（由人脸认证调用）
    static void triggerGateOpen();

    // 停止线程
    void stopThread();
    bool stop = false;         // 线程停止标志
    static int sg90_fd;        // SG90舵机文件描述符（静态成员）
};

#endif // PEOPLE_H



