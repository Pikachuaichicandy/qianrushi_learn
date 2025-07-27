#include "tcpsocket.h"
#include<QThread>
#include<QtEndian>
#include<QDebug>

TcpSocket::TcpSocket(QObject *parent)
    : QObject{parent}
{
    m_socket=nullptr;
    isConnect=false;
}

TcpSocket::~TcpSocket()
{
    if(m_socket!=nullptr)
        m_socket->deleteLater();
    qDebug()<<"tcpsocke删除";
}

void TcpSocket::connectServer(QString roomIp,QString ip, unsigned short port)
{
    qDebug()<<"socket线程："<<QThread::currentThreadId();
    m_socket=new QTcpSocket;
    connect(m_socket,&QTcpSocket::connected,this,[=](){
        isConnect=true;
        qDebug()<<"连接成功ip: "<<roomIp;
        send(2,roomIp.toUtf8());
    });
    connect(m_socket,&QTcpSocket::disconnected,this,[&](){
        isConnect=false;
        emit errorSignal("断开连接");
    });
    connect(m_socket,&QTcpSocket::readyRead,this,&TcpSocket::recv);

    //准备连接
    m_socket->connectToHost(ip,port);

    //超时错误检测
    if(!m_socket->waitForConnected(10000))
        emit errorSignal("连接不成功，请退出重试！");
}

void TcpSocket::send(int head,QByteArray data)
{
    if(data.isEmpty())
        return;

    //转化成大端
    head=qToBigEndian(head);
    int head2=qToBigEndian(data.size());

    QByteArray block((char*)&head,sizeof(int));
    block.append((char*)&head2,sizeof(int));
    block.append(data);

    if (m_socket->write(block)>0&&isConnect)
    {

    }
    else
    {
        emit errorSignal("未发送成功");
        return;
    }
}

void TcpSocket::recv()
{
    qDebug()<<"有数据可读：";
    int head;
    unsigned int totalBytes,currBytes=0;
    QByteArray data;

    if(m_socket->bytesAvailable()<2*sizeof(int))
        return;

    //第一个包头
    QByteArray head1 = m_socket->read(sizeof(int));
    head=qFromBigEndian(*(int*)head1.data());

    //第二个包头
    QByteArray head2 = m_socket->read(sizeof(int));
    totalBytes=qFromBigEndian(*(int*)head2.data());

    while (totalBytes-currBytes>0&&m_socket->bytesAvailable())
    {
        data.append(m_socket->read(totalBytes-currBytes));
        currBytes=data.size();
    }

    qDebug()<<"读取到数据：包头1："<<head1<<" ,总数据大小"<<totalBytes<<"  "<<data;

    if(totalBytes=currBytes)
    {
        switch (head) {
        case 2:
            emit finishTeam(data);
            break;
        default:
            break;
        }
    }
    else
        return;

    if(m_socket->bytesAvailable()>0)
        recv();
}
