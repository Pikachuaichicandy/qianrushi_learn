#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include<QThread>
#include<QObject>
#include<QWidget>
#include<QTcpSocket>
#include<QByteArray>
#include<QSqlQuery>
#include<vector>
#include<cstring>
#include<string>
#include<QDateTime>
#include"structural.h"
class mythread:public QThread
{
    Q_OBJECT
public:
    mythread(QTcpSocket *socket);
    void run();    //只有在这个函数里面执行的语句，才能开启新的线程，不能手动调用
    void Init();   //初始化
    void Flag(QByteArray array);//客户端信息标志位判断
    void order_menu();     //向顾客发送菜单
    void order_food();     //顾客订单处理
    void chef_sent();      //向厨房发送订单
    void chef_in();        //厨房上菜
    void cashier_in();     //收银登录
    void cashier_query();  //收银查询账单
    void cashier_refresh();//收银刷新餐桌状态
    void cashier_finish(); //收银结账
public:
   QTcpSocket *tcpsocket;
   static QTcpSocket *order_socket[20];  //顾客端套接字
   static QTcpSocket *chef_socket;       //厨房端套接字
   static QTcpSocket *cashier_socket;    //收银端套接字
   vector<MenuInfo> MenuVec;       //菜单容器
   MenuInfo M_head;                //菜单结构体
   vector<FoodInfo> Foodvec;       //订单容器 
   FoodInfo F_head;                //订单结构体
   QSqlDatabase db;
};
static vector<FoodInfo> foodvec[20]; //订单容器

#endif // MYTHREAD_H
