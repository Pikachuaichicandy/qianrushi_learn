#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>
#include <QThread>

class motor:public QThread
{
    Q_OBJECT
public:
    motor();
    ~motor();
    void run() override;
    void motorStop();
    int stop;

    int stepNumber;
    int delayNumber;
public slots:
    void setCommand(int step, int delay);
};

#endif // MOTOR_H
