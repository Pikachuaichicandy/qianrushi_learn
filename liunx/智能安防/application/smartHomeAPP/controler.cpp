#include "controler.h"
#include <QFile>
#include <QDebug>


controler::controler()
{

}

controler::~controler()
{

}

void controler::run()
{
    //1:69 2:70
    QFile file;
    QString fileName = "/dev/hx1838";
    file.setFileName(fileName);

    file.open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    while(!stop){
        file.read(reinterpret_cast<char*>(&commandData), sizeof(commandData));
        if(commandData == 69 || commandData == 70){
            emit sendCommandData(commandData);  //  发射信号
        }else{
            msleep(100);
            continue;
        }
    }
    file.close();
}

void controler::controlerStop()
{
    stop = 1;
}
