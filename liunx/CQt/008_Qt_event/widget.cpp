#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QPushButton>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //启动定时器
//    startTimer(1000); //参数1 间隔 单位 毫秒

    id1 = startTimer(1000);
    id2 = startTimer(2000);


    //启动定时器的第二种方式
    QTimer * timer = new QTimer(this);
    //启动定时器
    timer->start(500);
    connect(timer,&QTimer::timeout,[=](){
        static int num = 1;//改成静态的就不会一直为一了
        ui->label_2->setText(QString::number(num++));
    });

    //点击暂停
    connect(ui->btn,&QPushButton::clicked,[=](){
        timer->stop();
    });

}
void Widget::timerEvent(QTimerEvent * ev)
{
//    if(ev->timerId() == id1)
//    {
//    static int num = 1;//改成静态的就不会一直为一了
//    ui->label_2->setText(QString::number(num++));
//    }
}

Widget::~Widget()
{
    delete ui;
}

