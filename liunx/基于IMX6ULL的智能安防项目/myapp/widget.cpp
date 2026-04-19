#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QString>
#include <QLabel>
#include <QImage>
#include <QPainter>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QBuffer>
#include <QDateTime>

// 添加摄像头相关头文件
#include "camera.h"
#include <opencv2/opencv.hpp>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , camera(nullptr)
    , flag(0)
{
    ui->setupUi(this);

    // 1. 初始化Camera对象
    camera = new Camera(this);

    // 连接Camera的readyImage信号
    connect(camera, SIGNAL(readyImage(QImage)),
            this, SLOT(processCameraImage(QImage)));

    // 2. 选择摄像头设备1
    camera->selectCameraDevice(1);

    // 3. 导入级联分类器文件
#ifdef WIN32
    cascade.load("C:/opencv452/etc/haarcascades/haarcascade_frontalface_default.xml");
#else
    // 直接使用已知路径
    cascade.load("/FaceAttendance/haarcascade_frontalface_default.xml");
#endif

    // 4. 启动摄像头处理
    camera->cameraProcess(true);

    // 5. 网络连接设置
    connect(&msocket, &QTcpSocket::disconnected,
            this, &Widget::start_connect);
    connect(&msocket, &QTcpSocket::connected,
            this, &Widget::stop_connect);
    connect(&msocket, &QTcpSocket::readyRead,
            this, &Widget::recv_data);

    connect(&mtimer, &QTimer::timeout, this, &Widget::timer_connect);
    mtimer.start(5000);

    // 6. 界面初始化 - 确保UI中有对应的控件
    ui->widgetLb->hide();
    ui->headpicLb->setStyleSheet("border: 2px solid red; background: transparent;");
    ui->videoLb->setScaledContents(true);


    // 启动温湿度传感器线程
    myTemper = new temper();
    myTemper->getData(500);
    myTemper->start();
    // 数据索引：温度用databuf[1]，湿度用databuf[0]
    ui->temperatureLabel->setText("温度: " + QString::number(myTemper->databuf[1]) + "℃");
    ui->humidityLabel->setText("湿度: " + QString::number(myTemper->databuf[0]) + "%");
    // 启动光照传感器线程
    myLight = new light();
    myLight->start();

    // 设置定时器 - UI更新
    myTimer = new QTimer(this);
    myTimer->setInterval(100);
    connect(myTimer, &QTimer::timeout, this, &Widget::onTimeOut);
    myTimer->start();
    // 启动六轴传感器线程
    myicm20608 = new icm20608();
    myicm20608->start();
    ui->attitudeLabel->setText("物品姿态: 正常");

    // 启动超声波测距线程
    myDistance = new distance();
    myDistance->start();
    ui->distanceLabel->setText("安全距离: " + QString::number(myDistance->distanceData) + "cm");
    // 通过次数变化信号
    myPeople = new people();
    myPeople->start();




    // 启动传送带电机线程
    myMotor = new motor();
    myMotor->start();
    ui->conveyorStateLabel->setText("传送带状态: 停止");
}

Widget::~Widget()
{
    if (camera) {
        camera->cameraProcess(false);
        delete camera;
    }

    delete myMotor;
    delete myTemper;
    delete myLight;
    delete myicm20608;
    delete ui;
    delete myDistance;
    delete myPeople;
}

// 处理摄像头图像的槽函数
void Widget::processCameraImage(const QImage &image)
{
    if (image.isNull()) {
        return;
    }

    // 1. 直接显示图像（Camera输出的QImage已经是RGB格式）
    ui->videoLb->setPixmap(QPixmap::fromImage(image));

    // 2. 将QImage转换为Mat用于人脸检测
    // 注意：Camera输出的QImage是RGB888格式
    cv::Mat mat;

    if (image.format() == QImage::Format_RGB888) {
        // 将RGB转换为BGR（OpenCV需要BGR格式）
        cv::Mat rgbMat(image.height(), image.width(), CV_8UC3,
                       const_cast<uchar*>(image.bits()), image.bytesPerLine());

        // RGB转BGR
        cv::cvtColor(rgbMat, mat, cv::COLOR_RGB2BGR);
    } else {
        // 如果不是RGB888格式，先转换
        QImage rgbImage = image.convertToFormat(QImage::Format_RGB888);
        cv::Mat rgbMat(rgbImage.height(), rgbImage.width(), CV_8UC3,
                       const_cast<uchar*>(rgbImage.bits()), rgbImage.bytesPerLine());
        cv::cvtColor(rgbMat, mat, cv::COLOR_RGB2BGR);
    }

    if (mat.empty()) {
        return;
    }

    // 3. 人脸检测
    cv::Mat grayFrame;
    cv::cvtColor(mat, grayFrame, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(grayFrame, grayFrame);

    std::vector<cv::Rect> faces;
    cascade.detectMultiScale(grayFrame, faces, 1.1, 3, 0, cv::Size(100, 100));

    // 4. 处理检测结果
    if (faces.size() > 0) {
        cv::Rect faceRect = faces[0];

        // 移动人脸框
        int x = faceRect.x;
        int y = faceRect.y;
        int width = faceRect.width;
        int height = faceRect.height;

        ui->headpicLb->setGeometry(x, y, width, height);
        ui->headpicLb->show();

        // 5. 发送人脸图像
        if (flag > 2) {
            // 截取人脸区域
            cv::Mat faceMat = mat(faceRect);

            // 保存人脸图片
            cv::imwrite("/tmp/face.jpg", faceMat);

            // 将人脸Mat转换回QImage发送
            // 注意：faceMat是BGR格式，需要转RGB
            cv::Mat rgbFace;
            cv::cvtColor(faceMat, rgbFace, cv::COLOR_BGR2RGB);

            QImage faceImage(rgbFace.data, rgbFace.cols, rgbFace.rows,
                            rgbFace.step, QImage::Format_RGB888);

            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            faceImage.save(&buffer, "JPG", 90);
            buffer.close();

            // 发送数据
            if (msocket.state() == QAbstractSocket::ConnectedState) {
                QByteArray sendData;
                QDataStream out(&sendData, QIODevice::WriteOnly);
#ifdef WIN32
                out.setVersion(QDataStream::Qt_5_14);
#else
                out.setVersion(QDataStream::Qt_5_12);
#endif
                out << (quint64)byteArray.size() << byteArray;
                msocket.write(sendData);
                qDebug() << "发送人脸数据";
            }

            flag = -2;
        }
        flag++;
    } else {
        ui->headpicLb->hide();
        flag = 0;
    }
}

// 网络相关函数
void Widget::recv_data()
{
    QByteArray array = msocket.readAll();
    qDebug() << "收到服务器响应:" << array;

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(array, &err);
    if (err.error != QJsonParseError::NoError) {
        return;
    }

    QJsonObject obj = doc.object();
    QString employeeID = obj.value("employeeID").toString();
    QString name = obj.value("name").toString();
    QString department = obj.value("department").toString();
    QString timestr = obj.value("time").toString();

    ui->numberEdit_6->setText(employeeID);
    ui->nameEdit_6->setText(name);
    ui->departmentEdit_6->setText(department);
    ui->timeEdit_6->setText(timestr);

    // 显示保存的人脸图片
    QPixmap facePixmap("/tmp/face.jpg");
    if (!facePixmap.isNull()) {
        ui->headLb_6->setPixmap(facePixmap.scaled(ui->headLb_6->size(),
                                               Qt::KeepAspectRatio));
    }

    ui->widgetLb->show();

    // 人脸认证成功，触发闸门打开
        qDebug() << "人脸认证成功，触发闸门打开";
        people::triggerGateOpen();  // 直接调用静态方法
        ui->gateStateLabel->setText("闸状态: 人脸认证开门");
}

void Widget::timer_connect()
{
    if (msocket.state() != QAbstractSocket::ConnectedState) {
        msocket.connectToHost("192.168.74.100", 9999);
    }
}

void Widget::stop_connect()
{
    mtimer.stop();
    qDebug() << "成功连接服务器";
}

void Widget::start_connect()
{
    mtimer.start(5000);
    qDebug() << "与服务器断开连接";
}

// 其他函数
void Widget::onTimeOut()
{
    timerNums++;

    // 更新光照强度显示
    ui->lightLabel->setText("光照强度: " + QString::number(myLight->databuf[0]) + " Lux");

    if(timerNums > 600){    //60s测一次
        myTimer->stop();

        myTemper->getData(500);
        if(myTemper->databuf[0] > 0 && myTemper->databuf[0] < 100 && myTemper->databuf[1] > 0 && myTemper->databuf[1]){
            ui->humidityLabel->setText("湿度:" + QString::number(myTemper->databuf[0]) + "%");
            ui->temperatureLabel->setText("温度:" + QString::number(myTemper->databuf[1]) + "℃");
        }

        timerNums = 0;
        myTimer->start();
    }

    // 更新安全距离显示
    int distance = myDistance->distanceData;
    ui->distanceLabel->setText("安全距离: " + QString::number(distance) + "cm");

    // 安全距离警告
    if (distance < 20)
    {
        ui->systemStatusLabel->setText("警告-安全距离过近");
    }
}

void Widget::onSensorDataChanged(float gyro_x_act, float gyro_y_act, float gyro_z_act,
                                 float accel_x_act, float accel_y_act, float accel_z_act,
                                 float temp_act)
{
    if (accel_x_act > 0.01 || accel_y_act > 0.01 || accel_z_act > 0.01)
    {
        ui->attitudeLabel->setText("物品姿态: 异常");
    } else
    {
        ui->attitudeLabel->setText("物品姿态: 正常");
    }
}


void Widget::on_startConveyor_clicked()
{
    myMotor->setCommand(1000, 10);  // 启动传送带
    myMotor->run();
    ui->conveyorStateLabel->setText("传送带状态: 运行中");
    ui->systemStatusLabel->setText("系统状态: 运行中");
}


