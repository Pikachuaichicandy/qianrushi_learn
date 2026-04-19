#include "icm20608.h"
#include <QFile>
icm20608::icm20608()
{
}

icm20608::~icm20608()
{
}

void icm20608::run()
{
    QFile file;
    QString fileName = "/dev/icm20608";
    file.setFileName(fileName);

    file.open(QIODevice::ReadOnly | QIODevice::Unbuffered);

    while(!stop){
        int ret = file.read(reinterpret_cast<char*>(databuf), sizeof(databuf));

        if(ret == sizeof(databuf)) {  // 数据读取成功
            // 读取原始数据
            signed int gyro_x_adc = databuf[0];
            signed int gyro_y_adc = databuf[1];
            signed int gyro_z_adc = databuf[2];
            signed int accel_x_adc = databuf[3];
            signed int accel_y_adc = databuf[4];
            signed int accel_z_adc = databuf[5];
            signed int temp_adc = databuf[6];

            // 计算实际值
            float gyro_x_act = (float)(gyro_x_adc) / 16.4;
            float gyro_y_act = (float)(gyro_y_adc) / 16.4;
            float gyro_z_act = (float)(gyro_z_adc) / 16.4;
            float accel_x_act = (float)(accel_x_adc) / 2048;
            float accel_y_act = (float)(accel_y_adc) / 2048;
            float accel_z_act = (float)(accel_z_adc) / 2048;
            float temp_act = ((float)(temp_adc) - 25) / 326.8 + 25;

            // 发射数据变化信号
            emit sensorDataChanged(
                gyro_x_act, gyro_y_act, gyro_z_act,
                accel_x_act, accel_y_act, accel_z_act,
                temp_act
            );

        if(!stop){
            msleep(100);
        }
    }

    file.close();
}
}
void icm20608::icm20608Stop()
{
   stop = 1;
}

