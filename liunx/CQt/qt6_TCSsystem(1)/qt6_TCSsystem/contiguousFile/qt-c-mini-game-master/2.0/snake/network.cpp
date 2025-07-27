#include "network.h"
#include<QtEndian>
#include<QDebug>
#include <QDataStream>

NetWork::NetWork(QObject *parent)
    : QObject{parent}
{
    m_tcp=nullptr;
    isOnline=false;
}

NetWork::~NetWork()
{
    if(m_tcp!=nullptr)
    {
        m_tcp->close();   //断开连接
        m_tcp->deleteLater();
    }
    qDebug()<<"netWork析构";
}

void NetWork::startConnectServer(QString ip, unsigned short port)
{
    qDebug()<<"network线程id："<<QThread::currentThreadId();
    m_tcp=new QTcpSocket;

    connect(m_tcp,&QTcpSocket::connected,[=](){
        isOnline=true;
    });
    connect(m_tcp,&QTcpSocket::disconnected,[=](){
        isOnline=false;
    });

    //接收消息
    connect(m_tcp,&QTcpSocket::readyRead,this,&NetWork::recv);

    //连接服务器
    m_tcp->connectToHost(ip,port);
}

bool NetWork::isConnectNetWork()
{
    return isOnline;
}

void NetWork::send(int head1, int intData,int posi)
{
    //qDebug()<<"network线程id："<<QThread::currentThreadId();
    int head=qToBigEndian(head1);
    switch (head1) {
    case 1:
        sendDir(head,intData,posi);    //发送方向
        break;
    case 3:
        sendint(head,intData);
        break;
    case 7:
        sendDeath(head,m_roomIp,posi);
        break;
    }
}

void NetWork::sendint(int head, int intData)
{
    QByteArray block((char*)&head,sizeof(int));
    intData=qToBigEndian(intData);
    block.append((char*)&intData,sizeof(int));

    //调用tcp的发送,这里可以加超时等待的函数
    if(isOnline)
        m_tcp->write(block);
}

void NetWork::sendClientData(int head, int roomIp, QString name, int snakeHead, int snakeBody)
{
    //qDebug()<<"发送调试: "<<roomIp<<"  "<<name;
    head=qToBigEndian(head);
    QByteArray block((char*)&head,sizeof(int));

    roomIp=qToBigEndian(roomIp);
    block.append((char*)&roomIp,sizeof(int));

    int nameSize=name.toUtf8().size();
    nameSize=qToBigEndian(nameSize);
    block.append((char*)&nameSize,sizeof(int));
    block.append(name.toUtf8());

    snakeHead=qToBigEndian(snakeHead);
    snakeBody=qToBigEndian(snakeBody);
    block.append((char*)&snakeHead,sizeof(int));
    block.append((char*)&snakeBody,sizeof(int));

    if(isOnline)
        m_tcp->write(block);
}

void NetWork::recv()
{
    int head;
    m_tcp->read((char*)&head,sizeof(int));
    head=qFromBigEndian(head);
    qDebug()<<"接收消息：head: "<<head;
    //包头分类
    switch (head) {
    case 1:
        recvChangeDir();
        break;
    case 4:
        sendClientData(5,m_roomIp,m_name);
        break;
    case 5:
        recvClientData();
        break;
    case 6:
        recvSelfData();
        break;
    case 7:
        recvDeath();
        break;
    case 8:
        emit gameOver();
        break;
    }
}

void NetWork::recvClientData()
{
    int posi,x,y,snakeHead,snakeBody,name_len;
    m_tcp->read((char*)&posi,sizeof(int));
    m_tcp->read((char*)&x,sizeof(int));
    m_tcp->read((char*)&y,sizeof(int));
    m_tcp->read((char*)&snakeHead,sizeof(int));
    m_tcp->read((char*)&snakeBody,sizeof(int));
    m_tcp->read((char*)&name_len,sizeof(int));

    //转化主机字节
    posi=qFromBigEndian(posi);
    x=qFromBigEndian(x);
    y=qFromBigEndian(y);
    snakeHead=qFromBigEndian(snakeHead);
    snakeBody=qFromBigEndian(snakeBody);
    name_len=qFromBigEndian(name_len);
    QByteArray name;
    name = m_tcp->read(name_len);
    qDebug()<<"别人的数据"<<posi<<' '<<x<<' '<<y<<' '<<snakeHead<<' '<<snakeBody<<' '<<name_len<<' '<<QString(name);

    //打包数据发送给主线程
    emit snakeData(false,posi,Point(x,y),Point(),QString(name),snakeHead,snakeBody);
}

void NetWork::recvSelfData()
{
    int posi,x,y,foodx,foody;
    m_tcp->read((char*)&posi,sizeof(int));
    m_tcp->read((char*)&x,sizeof(int));
    m_tcp->read((char*)&y,sizeof(int));
    m_tcp->read((char*)&foodx,sizeof(int));
    m_tcp->read((char*)&foody,sizeof(int));

    m_posi=posi=qFromBigEndian(posi);
    x=qFromBigEndian(x);
    y=qFromBigEndian(y);
    foodx=qFromBigEndian(foodx);
    foody=qFromBigEndian(foody);
    qDebug()<<"自己的数据："<<posi<<' '<<x<<' '<<y<<' '<<foodx<<' '<<foody;

    //打包数据发送给主线程
    emit snakeData(true,posi,Point(x,y),Point(foodx,foody),m_name);
}

void NetWork::recvChangeDir()
{
    int posi,dir;
    m_tcp->read((char*)&posi,sizeof(int));
    m_tcp->read((char*)&dir,sizeof(int));
    posi=qFromBigEndian(posi);
    dir=qFromBigEndian(dir);

    emit changeDir(posi,dir);
}

void NetWork::recvDeath()
{
    int posi;
    m_tcp->read((char*)&posi,sizeof(int));
    posi=qFromBigEndian(posi);
    emit sendUserDeath(posi);
}

void NetWork::sendDir(int head,int dir,int posi)
{
    //qDebug()<<"发送了自己的方向："<<dir;
    int roomIp=qToBigEndian(m_roomIp);
    dir=qToBigEndian(dir);
    posi=qToBigEndian(posi);
    QByteArray block((char*)&head,sizeof(int));
    block.append((char*)&roomIp,sizeof(int));
    block.append((char*)&posi,sizeof(int));
    block.append((char*)&dir,sizeof(int));

    m_tcp->write(block);
}

void NetWork::sendDeath(int head, int roomIp, int posi)
{
    qDebug()<<"自己死亡了";
    roomIp=qToBigEndian(roomIp);
    posi=qToBigEndian(posi);
    QByteArray block((char*)&head,sizeof(int));
    block.append((char*)&roomIp,sizeof(int));
    block.append((char*)&posi,sizeof(int));

    m_tcp->write(block);
}
