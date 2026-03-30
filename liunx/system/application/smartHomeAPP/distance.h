#ifndef DISTANCE_H
#define DISTANCE_H

#include <QObject>
#include <QThread>
#include <QDebug>

class distance : public QThread
{
    Q_OBJECT

public:
    distance();
    ~distance();

    int stop = 0;   //停止标志
    int distanceData;   //距离数据

    void run() override;
    void distanceStop();


};

#endif // DISTANCE_H
