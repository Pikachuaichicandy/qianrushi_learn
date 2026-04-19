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
    static bool deviceOpened = false; /* 静态变量，确保设备只打开一次 */
    QFile file;
    QString fileName = "/dev/dth11";
    file.setFileName(fileName);

    if(!deviceOpened) {
        if(!file.open(QIODevice::ReadWrite | QIODevice::Unbuffered)){    //非缓冲读取
            qDebug() << "DTH11设备打开失败";
            return;
        }
        deviceOpened = true;
        qDebug() << "DTH11设备成功打开";
    }

    // file.open(QIODevice::ReadWrite | QIODevice::Unbuffered);    //非缓冲读取
    // while(!stop){
    //     msleep(2000);  // 每2秒读取一次
    //     file.read(reinterpret_cast<char*>(databuf), sizeof(databuf));
    //     if(databuf[0] > 0 && databuf[0] < 100 && databuf[1] > 0 && databuf[1]){     //判断数据是否正确
    //         for(int i = 0; i < 100; i++){
    //             if(!stop){
    //                 msleep(100);    //每10s读一次数据
    //             }
    //         }
    //     }else{      //数据不对重读
    //         msleep(1000);
    //         continue;
    //     }
    // }
    // file.close();

    while(!stop){
        // 添加延时，避免频繁读取导致时序冲突
        msleep(2000);  // 每2秒读取一次
        
        if(file.isOpen()) {
            file.read(reinterpret_cast<char*>(databuf), sizeof(databuf));
            if(databuf[0] > 0 && databuf[0] < 100 && databuf[1] > 0 && databuf[1] < 100){     //判断数据是否正确
                qDebug() << "DTH11数据正常:" << "湿度=" << databuf[0] << "温度=" << databuf[1];
            }else{      //数据不对重读
                qDebug() << "DTH11数据异常，重新读取";
                msleep(1000);
            }
        }
    }
    
    if(file.isOpen()) {
        file.close();
        deviceOpened = false;
    }
}

void temper::getData(int delay)
{
    //获取一次数据后退出
    while(1){
        QFile file;
        QString fileName = "/dev/dth11";
        file.setFileName(fileName);
        qDebug() << "DTH11设备文件打开状态:" << file.isOpen();
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
