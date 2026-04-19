#include "ispeople.h"
#include <QFile>
#include <QDebug>


ispeople::ispeople()
{

}

ispeople::~ispeople()
{

}

void ispeople::run()
{
    QFile file;
    QString fileName = "/dev/hc_sr505";
    file.setFileName(fileName);

    file.open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    while(!stop){
        file.read(reinterpret_cast<char*>(&value), sizeof(value));
        if(value == 0 || value == 1){
            isPeople = (value == 1);    //为1就真,为0就假
        }else{
            msleep(1);
            continue;
        }
    }
    file.close();
}

void ispeople::isPeopleStop()
{
   stop = 1;
}
