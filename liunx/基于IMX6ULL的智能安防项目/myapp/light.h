#ifndef LIGHT_H
#define LIGHT_H

#include <QObject>
#include <QThread>
class light : public QThread
{
    Q_OBJECT

public:
    light();
    ~light();

    void run() override;
    void lightStop();
    int stop = 0;
    unsigned char databuf[1];


};

#endif // LIGHT_H

