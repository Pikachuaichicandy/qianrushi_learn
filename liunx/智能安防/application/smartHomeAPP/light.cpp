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
    QString fileName = "/dev/bh1750";
    file.setFileName(fileName);

    file.open(QIODevice::ReadWrite | QIODevice::Unbuffered);    //非缓冲读取

    while(!stop){
        file.read(reinterpret_cast<char*>(&lightData), sizeof(lightData));

        for(int i = 0; i < 100; i++){
            if(!stop){
                msleep(10);    //每1s读一次数据
            }
        }
    }
    file.close();
}

void light::lightStop()
{
    stop = 1;
}
