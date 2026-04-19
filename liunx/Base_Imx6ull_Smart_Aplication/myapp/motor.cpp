#include "motor.h"
#include <QFile>
motor::motor()
{

}
motor::~motor()
{

}
void motor::run()
{
    QFile file;
    QString fileName = "/dev/motor";
    file.setFileName(fileName);

    file.open(QIODevice::WriteOnly | QIODevice::Unbuffered);

    int buf[2] = {stepNumber, delayNumber};
    file.write(reinterpret_cast<char*>(buf), 8);

    file.close();
}
void motor::setCommand(int step, int delay)
{
    stepNumber = step;
    delayNumber = delay;
}
void motor::motorStop()
{
   stop = 1;
   stepNumber = 0;   // 设置步数为0，表示停止
   delayNumber = 0;  // 设置延迟为0
   this->run();      // 执行写入，将停止命令发送到设备
}
