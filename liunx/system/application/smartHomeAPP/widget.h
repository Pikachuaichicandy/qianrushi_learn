#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QThread>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "temper.h"
#include "distance.h"
#include "ispeople.h"
#include "motor.h"
#include "controler.h"
#include "light.h"
#include "camera.h"

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
    int timerNums;          //定时器计数
    bool motorState;        //电机状态
    bool ledState;          //灯状态

    QTimer *myTimer;        //定时器
    temper *myTemper;       //获取温湿度
    distance *myDistance;   //获取距离
    ispeople *myIsPeople;   //检测是否有人
    motor *myMotor;         //电机线程
    controler *myControler; //红外遥控
    light *myLight;         //光照强度
    camera *myCamera;       //摄像头
    QTimer *myTimer1;

private slots:
    void onTimeOut();                           //定时器超时函数
    void controlerCommand(unsigned char data);  //红外遥控槽函数
    void on_fengshan_clicked();
    void on_deng_clicked();
    void updateVideo(QPixmap &frame);
    void on_close_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
