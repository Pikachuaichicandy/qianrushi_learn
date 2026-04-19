#include "motor.h"


motor::motor()
{

}

motor::~motor()
{

}

void motor::run()
{
    fd = open("/dev/tb6612", O_RDWR);
    if(fd < 0){
        qDebug() << "tb6612 open failed!";
    }

    while(!stop){
        if(taskType != none){
            switch(taskType){
            case forward:
                ioctl(fd, TB6612_CMD_FORWARD);
                break;
            case backward:
                ioctl(fd, TB6612_CMD_BACKWARD);
                break;
            case brake:
                ioctl(fd, TB6612_CMD_BRAKE);
                break;
            }
            taskType = none;
        }
        msleep(100);
    }
    close(fd);
}

void motor::motorStop()
{
    stop = 1;
}

void motor::setTask(int task)
{
    taskType = task;
}

void motor::setPwm(int pwmValue)
{
    int pwm = pwmValue;

    ioctl(fd, TB6612_CMD_PWM, &pwm);
}
