#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include<QPushButton>
#include<QLineEdit>
#include"gamewidget.h"
#include"network.h"
#include"snakewidget.h"

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    //游戏返回结束
    void returnLogin();

signals:
    void startConnectServer(QString ip="192.168.181.129", unsigned short port=8001);
    void send(int head1, int intData=0,int posi=-1);

private:
    void init();
    void initConnect();
    void start_stand_alone();
    void start_online();
    bool verify();

    QLineEdit* m_name;
    QLineEdit* m_roomIp;
    QPushButton* m_stand_alone;
    QPushButton* m_online;

    GameWidget* m_gameWidget;
    NetWork* m_netWork;
    SnakeWidget* m_snakeWidget;
};

#endif // LOGIN_H
