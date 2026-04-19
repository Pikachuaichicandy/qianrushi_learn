#ifndef ICM20608_H
#define ICM20608_H

#include <QObject>
#include <QThread>

class icm20608: public QThread
{
    Q_OBJECT

public:
    icm20608();
    ~icm20608();
    void run() override;
    void icm20608Stop();
    int stop;
    signed int databuf[7];
signals:
    void sensorDataChanged(
        float gyro_x, float gyro_y, float gyro_z,
        float accel_x, float accel_y, float accel_z,
        float temperature
    );

};

#endif // ICM20608_H
