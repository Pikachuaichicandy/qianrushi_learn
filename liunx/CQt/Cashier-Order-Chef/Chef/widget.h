#ifndef WIDGET_H
#define WIDGET_H

#include <QTimer>
#include <QWidget>
#include <QSqlQuery>
#include <vector>
#include <QString>
#include <QTcpSocket>
#include "structural.h"
using namespace std;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void Init();//初始化
    void client();//客服端
    void Order_print(QByteArray array);//打印订单
private slots:
    void on_pushButton_2_clicked();//退出

    void on_pushButton_clicked();//上菜

    void on_pushButton_3_clicked();//刷新

public:
    Ui::Widget *ui;
    //QSqlDatabase db;
    FoodInfo F_head;//订单结构体
    vector<FoodInfo> FoodVec;//订单容器
    vector<FoodInfo> foodVec;//通用容器
    QTcpSocket *tcpsocket;
private:
     QTimer *refreshTimer;
};

#endif // WIDGET_H
