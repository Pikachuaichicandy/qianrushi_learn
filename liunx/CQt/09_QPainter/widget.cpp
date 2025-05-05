#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QPen>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

}

void Widget::paintEvent(QPaintEvent *)
{
    //实列化画家对象
    QPainter painter(this);

    //设置画笔
//    painter.setPen(QColor(255,0,0));
    QPen Pen(QColor(255,0,0));
    //让画家 使用这个笔
    painter.setPen(Pen);
    //画线
    painter.drawLine(QPoint(0,0),QPoint(100,100));
    //画圆 椭圆
    painter.drawEllipse(QPoint(100,100),100,50);
    //画矩形,
    painter.drawRect(QRect(20,20,50,50));
    //画字
    painter.drawText(QRect(10,200,100,50),"学习");

}

Widget::~Widget()
{
    delete ui;
}

