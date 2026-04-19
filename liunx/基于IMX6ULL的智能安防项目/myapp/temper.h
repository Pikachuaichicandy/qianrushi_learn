#ifndef TEMPER_H
#define TEMPER_H

#include <QObject>
#include <QThread>
#include <QFile>
class temper : public QThread
{
    Q_OBJECT
public:
    temper();
    ~temper();



    void run() override;
    void getData(int delay);
    void temperStop();
    int stop =0;                //停止标志
    unsigned char databuf[2];   //存储数据


};

#endif // TEMPER_H
