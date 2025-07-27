#include "tcpwidget.h"
#include<QThread>
#include<QDebug>

TcpWidget::TcpWidget(QString roomIp,QWidget *parent)
    : QWidget{parent}
{
    m_roomIp=roomIp;

    setFixedSize(MWidth+10,MHeight+50);

    //创建子线程
    TcpSocket* m_tcp=new TcpSocket;
    QThread* subThread=new QThread;
    m_tcp->moveToThread(subThread);
    subThread->start();

    connect(this,&TcpWidget::destroyed,[=](){
        subThread->quit();
        subThread->wait();
        subThread->deleteLater();
    });
    connect(subThread,&QThread::finished,m_tcp,&TcpSocket::deleteLater);

    //连接服务器
    connect(this,&TcpWidget::startConnect,m_tcp,&TcpSocket::connectServer);

    //连接实现函数
    //connect(m_tcp,&TcpSocket::connSuccessed,this,&TcpWidget::connSuccessed);

    qDebug()<<"主界面线程："<<QThread::currentThreadId();
    QString ip="192.168.181.129";unsigned short port=8001;
    qDebug()<<"m_roomIp: "<<m_roomIp;
    emit startConnect(m_roomIp,ip,port);
}

void TcpWidget::setRoomIp(QString roomIp)
{
    m_roomIp=roomIp;
}

void TcpWidget::connSuccessed()
{

}

void TcpWidget::keyPressEvent(QKeyEvent *event)
{

}

void TcpWidget::paintEvent(QPaintEvent *event)
{

}

void TcpWidget::initSnakeBody()
{

}
