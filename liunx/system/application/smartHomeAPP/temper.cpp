#include "temper.h"
#include <QFile>


temper::temper()
{

}

temper::~temper()
{

}

void temper::run()
{
    QFile file;
    QString fileName = "/dev/dth11";
    file.setFileName(fileName);

    file.open(QIODevice::ReadWrite | QIODevice::Unbuffered);    //非缓冲读取

    while(!stop){
        file.read(reinterpret_cast<char*>(databuf), sizeof(databuf));
        if(databuf[0] > 0 && databuf[0] < 100 && databuf[1] > 0 && databuf[1]){     //判断数据是否正确
            for(int i = 0; i < 100; i++){
                if(!stop){
                    msleep(100);    //每10s读一次数据
                }
            }
        }else{      //数据不对重读
            msleep(1000);
            continue;
        }
    }
    file.close();
}

void temper::getData(int delay)
{
    //获取一次数据后退出
    while(1){
        QFile file;
        QString fileName = "/dev/dth11";
        file.setFileName(fileName);

        file.open(QIODevice::ReadWrite | QIODevice::Unbuffered);
        file.read(reinterpret_cast<char*>(databuf), sizeof(databuf));
        file.close();

        if(databuf[0] > 0 && databuf[0] < 100 && databuf[1] > 0 && databuf[1]){
            break;
        }
        msleep(delay);
    }
}

void temper::temperStop()
{
   stop = 1;
}
