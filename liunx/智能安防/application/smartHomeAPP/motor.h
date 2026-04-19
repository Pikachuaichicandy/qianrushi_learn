#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define TB6612_CMD_FORWARD  _IO('M', 1)         //正转
#define TB6612_CMD_BACKWARD _IO('M', 2)         //反转
#define TB6612_CMD_BRAKE    _IO('M', 3)         //停止
#define TB6612_CMD_PWM      _IOW('M', 4, int)   //设置转速

class motor : public QThread
{
    Q_OBJECT

public:
    motor();
    ~motor();

    enum taskTypeNum{
        none = 0,
        forward = 1,
        backward = 2,
        brake = 3
    };

    int stop = 0;
    int taskType = 0;   //任务类型
    int fd = -1;

    void run() override;
    void motorStop();
    void setTask(int task);
    void setPwm(int pwmValue);

};

#endif // MOTOR_H
