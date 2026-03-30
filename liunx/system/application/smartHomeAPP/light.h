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

    int stop = 0;
    unsigned short lightData;

    void run() override;
    void lightStop();


};

#endif // LIGHT_H
