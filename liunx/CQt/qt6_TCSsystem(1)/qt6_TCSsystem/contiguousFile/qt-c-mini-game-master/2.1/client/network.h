#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include<QTcpSocket>
#include<QThread>
#include"struct.h"

class NetWork : public QObject
{
    Q_OBJECT
public:
    explicit NetWork(QObject *parent = nullptr);
    ~NetWork();

    int m_roomIp;    //房间号
    int m_posi;      //对应位置
    QString m_name;   //用户名

    //公共连接服务器函数
    void startConnectServer(QString ip,unsigned short port);
    //返回是否联网
    bool isConnectNetWork();

    //发送消息总体函数
    void send(int head1,int intData,int posi);
    //发送一个int + int 类型
    void sendint(int head,int intData);
    //发送客户端数据
    void sendClientData(int head,int roomIp,QString name,int snakeHead=0,int snakeBody=0);
    //发送方向
    void sendDir(int head,int dir,int posi);
    //发送死亡信息
    void sendDeath(int head, int roomIp, int posi);

    //总的接收函数
    void recv();
    //接收其他客户端的信息
    void recvClientData();
    //接收自己的数据
    void recvSelfData();
    //接收方向改变
    void recvChangeDir();
    //接收其他玩家死亡信息
    void recvDeath();
    //接收食物坐标
    void recvFoodPoint();


signals:
    //发送给贪吃蛇页面关于蛇的信息
    void snakeData(bool isSelf,int posi,Point point,Point food,QString name,int head=0,int body=0);
    //返回给主线程方向改变的数据
    void changeDir(int posi,int dir);
    //发送给主线程哪个玩家死亡
    void sendUserDeath(int posi);
    //游戏结束信号
    void gameOver();
    //发送食物
    void foodPoint(Point p);

private:
    bool isOnline;
    QTcpSocket* m_tcp;
};

#endif // NETWORK_H
