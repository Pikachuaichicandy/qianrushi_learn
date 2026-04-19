#include "temper.h"

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
    if(!file.open(QIODevice::ReadWrite | QIODevice::Unbuffered)) {
        return; // 打开失败直接返回
    }

    while(!stop){
        file.read(reinterpret_cast<char*>(databuf), sizeof(databuf));

        // 温度在databuf[1]，湿度在databuf[0]
        if(databuf[0] > 0 && databuf[0] < 100 && databuf[1] > -20 && databuf[1] < 60){
            // 数据正确，等待10秒
            for(int i = 0; i < 100; i++){
                if(!stop){
                    msleep(100);    //每10s读一次数据
                } else {
                    break;
                }
            }
        } else {
            //数据不对重读
            msleep(1000);
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
        if(file.open(QIODevice::ReadWrite | QIODevice::Unbuffered)) {
            file.read(reinterpret_cast<char*>(databuf), sizeof(databuf));
            file.close();


            if(databuf[0] > 0 && databuf[0] < 100 && databuf[1] > -20 && databuf[1] < 60){
                break;
            }
        }
        msleep(delay);
    }
}

void temper::temperStop()
{
   stop = true;
}
