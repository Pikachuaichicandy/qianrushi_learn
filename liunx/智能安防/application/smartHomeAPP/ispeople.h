#ifndef ISPEOPLE_H
#define ISPEOPLE_H

#include <QObject>
#include <QThread>
#include <QMutex>

class ispeople : public QThread
{
    Q_OBJECT

public:
    ispeople();
    ~ispeople();

    int stop = 0;
    int value = 0;      //存储电平高低值
    bool isPeople;      //是否有人标志

    void run() override;
    void isPeopleStop();
};

#endif // ISPEOPLE_H
