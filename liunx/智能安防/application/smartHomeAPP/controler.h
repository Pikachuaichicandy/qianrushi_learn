#ifndef CONTROLER_H
#define CONTROLER_H

#include <QObject>
#include <QThread>

class controler : public QThread
{
    Q_OBJECT

public:
    controler();
    ~controler();

    int stop = 0;
    unsigned char commandData;  //遥控器指令数据

    void run() override;
    void controlerStop();

signals:
    void sendCommandData(unsigned char commandData);
};

#endif // CONTROLER_H
