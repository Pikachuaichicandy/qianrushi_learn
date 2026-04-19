#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QThread>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <light.h>
#include <temper.h>
#include <icm20608.h>
#include <distance.h>
#include <people.h>
#include <motor.h>
#include <camera.h>
#include <QDebug>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/videoio/videoio.hpp"
#include <QTcpSocket>

#include <QImage>
#include <QPainter>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QBuffer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public:
    // 人脸识别和网络相关成员变量
    Camera *camera;
    cv::CascadeClassifier cascade;
    QTcpSocket msocket;
    QTimer mtimer;
    int flag;

    int timerNums;          //定时器计数
    bool conveyorStateLabel;        //电机状态
    bool emergencyStateLabel;  //系统状态

    QTimer *myTimer;        //定时器
    temper *myTemper;       //获取温湿度
    light *myLight;         //光照强度
    QTimer *myTimer1;
    icm20608 *myicm20608;
    distance *myDistance;
    people *myPeople;
    motor *myMotor;



private slots:

    // 人脸识别和网络相关的槽函数
    void processCameraImage(const QImage &image);
    void recv_data();
    void timer_connect();
    void stop_connect();
    void start_connect();

    void onTimeOut();
    void onSensorDataChanged(float gyro_x, float gyro_y, float gyro_z,
                            float accel_x, float accel_y, float accel_z,
                            float temp);  // icm20608
    // void passCountChanged(int count);
    void on_startConveyor_clicked();


private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
