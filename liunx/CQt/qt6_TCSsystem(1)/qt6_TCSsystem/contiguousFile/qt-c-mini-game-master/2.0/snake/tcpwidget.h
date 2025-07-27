#ifndef TCPWIDGET_H
#define TCPWIDGET_H

#include <QWidget>
#include<QKeyEvent>
#include<QVector>
#include"config.h"
#include"struct.h"
#include"tcpsocket.h"

class TcpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TcpWidget(QString roomIp,QWidget *parent = nullptr);

    //设置房间号
    void setRoomIp(QString roomIp);
    void connSuccessed();

protected:
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

signals:
    void startConnect(QString roomIp,QString ip, unsigned short port);

private:
    void initSnakeBody();

    QString m_roomIp;
    //对于每一个用户来说，0是自己
    QVector<QVector<snakeBody>> m_sanke;
    QVector<moveDir> m_dir;

};

#endif // TCPWIDGET_H
