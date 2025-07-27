#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include<QTcpSocket>

class TcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = nullptr);
    ~TcpSocket();

    void connectServer(QString roomIp,QString ip,unsigned short port);
    void send(int head,QByteArray data);

signals:
    //void connSuccessed();
    //void disConnection();
    void errorSignal(QString error);
    void finishTeam(QByteArray data);   //组队完成

private:
    void recv();

    bool isConnect;
    QTcpSocket* m_socket;
};

#endif // TCPSOCKET_H
