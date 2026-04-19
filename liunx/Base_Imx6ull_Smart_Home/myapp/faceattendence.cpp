#include "faceattendence.h"
#include "ui_widget.h"
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QBuffer>
FaceAttendence::FaceAttendence(QWidget *parent)
    : QWidget(parent)

{
    ui->setupUi(this);

#ifdef WIN32
    //打开摄像头
    cap.open(0);//dev/video
    //导入级联分类器文件
    cascade.load("C:/opencv452/etc/haarcascades/haarcascade_frontalface_default.xml");
#else

    //打开摄像头
    cap.open("/dev/video0");
    //导入级联分类器文件
    cascade.load("/home/alientek/opencv/opencv-3.4.1/data/haarcascades/haarcascade_frontalface_default.xml");
#endif
    //启动定时器事件
    startTimer(100);

    //QTcpSocket当断开连接的时候disconnected信号，连接成功会发送connected
    connect(&msocket,&QTcpSocket::disconnected,this, &FaceAttendence::start_connect);
    connect(&msocket,&QTcpSocket::connected,this, &FaceAttendence::stop_connect);
    //关联接收数据的槽函数
    connect(&msocket, &QTcpSocket::readyRead,this, &FaceAttendence::recv_data);

    //定时器连接服务器
    connect(&mtimer, &QTimer::timeout,this,&FaceAttendence::timer_connect);
    //启动定时器
    mtimer.start(5000);//每5s钟连接一次，直到连接成功就不在连接

    flag =0;

    ui->widgetLb->hide();
}

FaceAttendence::~FaceAttendence()
{

}

void FaceAttendence::timerEvent(QTimerEvent *e)
{
    //采集数据
    Mat srcImage;
    if(cap.grab())
    {
        cap.read(srcImage);//读取一帧数据
    }

    //把图片大小设与显示窗口一样大
   // cv::resize(srcImage,srcImage,Size(480,480));
   // if(srcImage.data == nullptr) return;
    //把opencv里面的Mat格式数据（BGR）转Qt里面的QImage(RGB)
    cvtColor(srcImage,srcImage, COLOR_BGR2RGB);
    QImage image(srcImage.data,srcImage.cols, srcImage.rows,srcImage.step1(),QImage::Format_RGB888);
    QPixmap mmp = QPixmap::fromImage(image);
    ui->videoLb->setPixmap(mmp);


    Mat grayImage;
    //转灰度图
    cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
    //检测人脸数据
    std::vector<Rect> faceRects;
    cascade.detectMultiScale(grayImage,faceRects,1.1,3,0,cv::Size(150,150));//检测人脸
    if(faceRects.size()>0 && flag>=0)
    {

        Rect rect = faceRects.at(0);//第一个人脸的矩形框
        //rectangle(srcImage,rect,Scalar(0,0,255));
        //移动人脸框（图片--QLabel）
        ui->headpicLb->move(rect.x,rect.y);

        if(flag > 2){
            //把Mat数据转化为QbyteArray， --》编码成jpg格式
            //std::vector<uchar> buf;
            //cv::imencode(".jpg",srcImage,buf);
            //QByteArray byte((const char*)buf.data(),buf.size());

            //QImage转换为QByteArray
            QByteArray byte;
            QBuffer buffer(&byte);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "jpg");
            buffer.close();

            //准备发送
            quint64 backsize = byte.size();
            QByteArray sendData;
            QDataStream stream(&sendData,QIODevice::WriteOnly);
#ifdef WIN32
            stream.setVersion(QDataStream::Qt_5_14);
#else
            stream.setVersion(QDataStream::Qt_5_12);
#endif
            stream<<backsize<<byte;
            //发送
            msocket.write(sendData);
            flag = -2;
            faceMat = srcImage(rect);
            //保存
            imwrite("./face.jpg",faceMat);
        }
        flag++;
    }
    if(faceRects.size() == 0)
    {
        //把人脸框移动到中心位置
        ui->headpicLb->move(100,60);
        ui->widgetLb->hide();
        flag=0;
    }
}

void FaceAttendence::recv_data()
{
    //{employeeID:%1,name:%2,department:软件,time:%3}
    QByteArray array = msocket.readAll();
    qDebug()<<array;
    //Json解析
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(array,&err);
    if(err.error != QJsonParseError::NoError)
    {
        qDebug()<<"json数据错误";
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

    //通过样式来显示图片
    ui->headLb->setStyleSheet("border-radius:75px;border-image: url(./face.jpg);");
    ui->widgetLb->show();
}

void FaceAttendence::timer_connect()
{
    //连接服务器
    msocket.connectToHost("192.168.74.100",9999);
    qDebug()<<"正在连接服务器";
}

void FaceAttendence::stop_connect()
{
    mtimer.stop();
    qDebug()<<"成功连接服务器";
}

void FaceAttendence::start_connect()
{
    mtimer.start(5000);//启动定时器
    qDebug()<<"断开连接";
}

