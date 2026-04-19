#include "widget.h"
#include "ui_widget.h"
#include <QDebug>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    timerNums = 0;
    motorState = false;
    ledState = false;

    //启动温湿度线程
    myTemper = new temper();
    myTemper->getData(500);
    myTemper->start();
    ui->shidu->setText("湿度:" + QString::number(myTemper->databuf[0]) + "%");
    ui->wendu->setText("温度:" + QString::number(myTemper->databuf[1]) + "℃");

    //启动距离线程
    myDistance = new distance();
    myDistance->start();
    ui->juli->setText("距离:0cm");

    //注释掉其他模块，只测试HC-SR04和DTH11
    /*
    //启动人体感应线程
    myIsPeople = new ispeople();
    myIsPeople->start();

    //启动电机线程
    myMotor = new motor();
    myMotor->start();
    ui->fengshan->setText("开风扇");
    ui->fengshanzhuangtai->setText("风扇状态:停止");

    //启动红外遥控线程
    myControler = new controler();
    connect(myControler, &controler::sendCommandData, this, &Widget::controlerCommand);
    myControler->start();

    //启动光照强度线程
    myLight = new light();
    myLight->start();

    //启动摄像头
    myCamera = new camera();
    connect(myCamera, &camera::sendPix, this, &Widget::updateVideo);
    myCamera->cameraOpen();
    myCamera->videoShow();
    */


    //设置定时器
    myTimer = new QTimer(this);
    myTimer->setInterval(100);
    connect(myTimer, &QTimer::timeout, this, &Widget::onTimeOut);
    myTimer->start();

    /*myTimer = new QTimer(this);
    myTimer->setInterval(10);
    connect(myTimer, &QTimer::timeout, myCamera, &camera::videoShow);
    myTimer->start();*/
}

Widget::~Widget()
{
    delete myTemper;
    delete myDistance;
    /*delete myIsPeople;
    delete myMotor;
    delete myLight;
    delete myCamera;
    */
    delete myTimer;
    // delete myTimer1;
    delete ui;
}

void Widget::onTimeOut()
{
    timerNums++;

    /*unsigned short rawData = myLight->lightData;

    float lux = (float)rawData / 1.2;

    ui->guangzhao->setText("光照强度: " + QString::number(lux, 'f', 2) + " Lux");
    
    if(myIsPeople->isPeople){
        ui->shifouyouren->setText("是否有人:有人");
        ui->juli->setText("距离:" + QString::number(myDistance->distanceData) + "cm");

    }else{
        ui->shifouyouren->setText("是否有人:无人");
        ui->juli->setText("距离:0cm");
    }
    */

    ui->juli->setText("距离:" + QString::number(myDistance->distanceData) + "cm");

    if(timerNums > 600){    //60s测一次
        myTimer->stop();

        myTemper->getData(500);
        if(myTemper->databuf[0] > 0 && myTemper->databuf[0] < 100 && myTemper->databuf[1] > 0 && myTemper->databuf[1]){
            ui->shidu->setText("湿度:" + QString::number(myTemper->databuf[0]) + "%");
            ui->wendu->setText("温度:" + QString::number(myTemper->databuf[1]) + "℃");
        }

        timerNums = 0;
        myTimer->start();
    }
}

void Widget::controlerCommand(unsigned char data)
{
    switch(data){
    case 69 :
        on_fengshan_clicked();
        break;
    case 70 :
        on_deng_clicked();
        break;
    }
}


void Widget::on_fengshan_clicked()
{
    int fd;
    fd = open("/dev/tb6612", O_RDWR);
    if(fd < 0){
        qDebug() << "tb6612 open failed!";
    }

    if(motorState){
        motorState = false;
        myMotor->setTask(motor::brake);
        ui->fengshan->setText("开风扇");
        ui->fengshanzhuangtai->setText("风扇状态:停止");
    }else{
        motorState = true;
        myMotor->setPwm(50);
        myMotor->setTask(motor::forward);
        ui->fengshan->setText("关风扇");
        ui->fengshanzhuangtai->setText("风扇状态:运行");
    }
}

void Widget::on_deng_clicked()
{
    int fd;
    unsigned char led[1];
    fd = open("/dev/led", O_RDWR);
    if(fd < 0){
        qDebug() << "led open failed!";
    }

    if(ledState){
        ledState = false;
        ui->deng->setText("开灯");
        ui->dengzhuangtai->setText("灯状态:灭");
        led[0] = 0;
    }else{
        ledState = true;
        ui->deng->setText("关灯");
        ui->dengzhuangtai->setText("灯状态:亮");
        led[0] = 1;
    }
    write(fd, led, sizeof(led));
    ::close(fd);
}

void Widget::updateVideo(QPixmap &frame)
{
    ui->video->setPixmap(frame.scaled(ui->video->width(),ui->video->height(),Qt::KeepAspectRatio));
}



void Widget::on_close_clicked()
{
    myTemper->temperStop();
    myDistance->distanceStop();
    myIsPeople->isPeopleStop();
    myMotor->motorStop();
    myLight->lightStop();
    myCamera->cameraClose();

    this->close();
}