#ifndef FACEATTENDENCE_H
#define FACEATTENDENCE_H

#include <QWidget>


#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/videoio/videoio.hpp"
#include <QTcpSocket>
#include <QTimer>
using namespace cv;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class Widght; }
QT_END_NAMESPACE

class FaceAttendence : public QWidget
{
    Q_OBJECT
public:
    FaceAttendence(QWidget *parent = nullptr);
    ~FaceAttendence();
    //定时器事件
    void timerEvent(QTimerEvent *e);

protected slots:
    void recv_data();
private slots:
    void timer_connect();
    void stop_connect();
    void start_connect();


private:
    Ui::Widght *ui;

    //摄像头
    VideoCapture cap;
    //haar--级联分类器
    cv::CascadeClassifier cascade;

    //创建网络套接字，定时器
    QTcpSocket msocket;
    QTimer mtimer;

    //标志是否是同一个人脸进入到识别区域
    int flag;

    //保存人类的数据
    cv::Mat faceMat;

};
#endif // FACEATTENDENCE_H
