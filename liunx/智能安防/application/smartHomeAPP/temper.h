#ifndef TEMPER_H
#define TEMPER_H

#include <QDebug>
#include <QObject>
#include <QThread>

class temper : public QThread
{
    Q_OBJECT
public:
    temper();
    ~temper();

    int stop =0;                //停止标志
    unsigned char databuf[2];   //存储数据

    void run() override;
    void getData(int delay);
    void temperStop();

};

#endif // TEMPER_H
