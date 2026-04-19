#include "light.h"
#include <QFile>
#include <QDebug>

light::light()
{
}
light::~light()
{
}

void light::run()
{
    QFile file;
    QString fileName = "/sys/class/misc/ap3216c/als";
    file.setFileName(fileName);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Unbuffered))
    {
        return; // 打开失败直接返回
    }

    while(!stop){
        int ret = file.read(reinterpret_cast<char*>(databuf), sizeof(databuf));

        if(ret == sizeof(databuf)) {  // 数据读取成功
            unsigned short als = databuf[0];   // ir传感器数据
            //unsigned short ir = databuf[1];  // als传感器数据
            //unsigned short ps = databuf[2];   // ps传感器数据

        if(!stop){
            msleep(200);  // 每200ms读取一次，
        }
    }

    file.close();
}
}
void light::lightStop()
{
        stop = 1;
}
