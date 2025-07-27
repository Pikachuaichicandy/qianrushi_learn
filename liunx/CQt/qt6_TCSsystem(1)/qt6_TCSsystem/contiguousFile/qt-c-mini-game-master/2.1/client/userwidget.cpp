#include "userwidget.h"
#include<QDebug>

UserWidget::UserWidget(int w,int h,QWidget *parent)
    : QWidget{parent}
{
    m_userName=new QLabel(this);
    m_score=new QLabel(this);
    m_head=new QLabel(this);
    m_body=new QLabel(this);
    setFixedSize(w,h);
    m_userName->setGeometry(0,0,w/2,h/2);
    m_score->setGeometry(w/2,0,w/2,h/2);
    m_head->setGeometry(0,h/2,w/2,h/2);
    m_body->setGeometry(w/2,h/2,w/2,h/2);
}

UserWidget::~UserWidget()
{
    qDebug()<<"UserWidget析构";
}

void UserWidget::setVariable(QString name, int score, QPixmap &head, QPixmap &body)
{
    m_userName->setText(name);
    m_score->setText(QString::number(score));
    m_head->setPixmap(head);
    m_body->setPixmap(body);
    //qDebug()<<"设置了数据"<<name<<"  "<<score;
}

void UserWidget::setFontColor()
{
    m_userName->setStyleSheet("color: red;");
    m_score->setStyleSheet("color: red;");
}

void UserWidget::setScore(int score)
{
    m_score->setText(QString::number(score));
}
