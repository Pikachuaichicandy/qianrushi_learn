#include "childdorm.h"
#include "ui_childdorm.h"
#include "widget.h"
#include <QDesktopWidget>
#include<QSqlQuery>
#include<QDebug>
#include<QString>
#include<cstring>
#include<QMessageBox>
#include<QSqlError>
#include<QHostAddress>
#include<QTableWidgetItem>
#include <QVariant>
#include<unistd.h>
using namespace std;
childdorm::childdorm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::childdorm)
{
    ui->setupUi(this);
    this->setFixedSize(1500,800);//背景图尺寸
    // this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setWindowTitle(QStringLiteral("收银界面"));
    this->move(QPoint(100,100));
    this->setWindowIcon(QIcon(":/image/5.png"));//APP图标

    //窗口居住显示
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    QPalette pal = this->palette();
    //pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/27.jpg")));//背景图
    setPalette(pal);

    //ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应

    Init();
    client();
//    // 添加定时器实现自动刷新
//    refreshTimer = new QTimer(this);
//    connect(refreshTimer, &QTimer::timeout, this, &childdorm::on_pushButton_5_clicked);
//    refreshTimer->start(2000); // 每2秒自动刷新一次
}

childdorm::~childdorm()
{
//    delete refreshTimer; // 释放定时器资源
    delete ui;
}

void childdorm::Init()//初始化
{
    tcpsocket = new QTcpSocket(this);//创建客户端套接字


    connect(tcpsocket,&QTcpSocket::connected,//请求连接信号
           [=]()
    {
        //ui->textEdit->setText("成功连接服务器");
    }
    );

//    QString ip = "192.168.79.129";
    QString ip = " 192.168.174.129";
    qint16 port = 8888;
    tcpsocket->connectToHost(QHostAddress(ip),port);
}

void childdorm::client()        //客服端操作
{
    connect(tcpsocket,&QTcpSocket::readyRead,
            [=]()
    {
        QByteArray array = tcpsocket->readAll();
        flag(array);     //数据解析
    }
    );
}

void childdorm::flag(QByteArray array) //服务端信息判断
{
    FoodVec.clear(); //接收容器
    FoodVec.resize(array.size()/sizeof(FoodInfo));
    memcpy(FoodVec.data(),array.data(),array.size());
    qDebug()<<FoodVec[0].food;
    if(strcmp(FoodVec[0].food,"cashier_in")==0)//登录允许
    {
        cashier_in();
    }
    else if(strcmp(FoodVec[0].food,"cashier_no")==0)//登录失败
    {
        cashier_no();
    }
    else if(strcmp(FoodVec[0].food,"cashier_query")==0)//查询账单结果
    {
        cashier_query();
    }
    else if(strcmp(FoodVec[0].food,"cashier_refresh")==0)//刷新餐桌状态
    {
        cashier_refresh();
    }
}

void childdorm::log_in(QString name, QString pw)  //发送登录信息
{
    m_name = name;//记录用户名
    foodVec.clear();
    strcpy(F_head.table,"\0");
    strcpy(F_head.number,"\0");
    strcpy(F_head.food,"cashier");  //放入标志位
    strcpy(F_head.price,"\0");
    strcpy(F_head.quatity,"\0");
    strcpy(F_head.post,"\0");
    foodVec.push_back(F_head);//订单信息标志位放入容器中首元素

    QByteArray ba = name.toLocal8Bit();
    memcpy(F_head.food,ba.data(),ba.size()+1);  //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
    ba = pw.toLocal8Bit();
    memcpy(F_head.post,ba.data(),ba.size()+1);    //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
    foodVec.push_back(F_head);//放入登录信息

    QByteArray a1;
    a1.resize(sizeof(FoodInfo)*foodVec.size());
    memcpy(a1.data(),foodVec.data(),sizeof(FoodInfo)*foodVec.size());
    tcpsocket->write(a1);
}

void childdorm::cashier_in()//允许登录
{
    emit Interface();
    ui->lineEdit_4->setText(m_name);
    ui->lineEdit_4->setFocusPolicy(Qt::NoFocus);  //设置为只读模式
    ui->lineEdit->setFocusPolicy(Qt::NoFocus);    //设置为只读模式
    ui->lineEdit_3->setFocusPolicy(Qt::NoFocus);  //设置为只读模式
    this->show();
}

void childdorm::cashier_no()//登录失败
{
     emit Inter_no();
}

void childdorm::cashier_query() //打印账单信息
{
    m_sum = 0;
    QStringList headtext;
    headtext<<"桌号"<<"序号"<<"餐名"<<"价格"<<"数量"<<"备注";
    ui->tableWidget->setColumnCount(headtext.count());//列表设置为和headtext相等
    ui->tableWidget->setHorizontalHeaderLabels(headtext);//插入表头
    ui->tableWidget->setRowCount(0);

    for (int j=1; j<FoodVec.size(); j++)
    {
        int rowcount = ui->tableWidget->rowCount();

        ui->tableWidget->insertRow(rowcount);

        QTableWidgetItem *column = new QTableWidgetItem(FoodVec[j].table);
        QTableWidgetItem *column1 = new QTableWidgetItem(FoodVec[j].number);
        QTableWidgetItem *column2 = new QTableWidgetItem(FoodVec[j].food);
        QTableWidgetItem *column3 = new QTableWidgetItem(FoodVec[j].price);
        QTableWidgetItem *column4 = new QTableWidgetItem(FoodVec[j].quatity);
        QTableWidgetItem *column5 = new QTableWidgetItem(FoodVec[j].post);

        ui->tableWidget->setItem(rowcount,0,column);
        ui->tableWidget->setItem(rowcount,1,column1);
        ui->tableWidget->setItem(rowcount,2,column2);
        ui->tableWidget->setItem(rowcount,3,column3);
        ui->tableWidget->setItem(rowcount,4,column4);
        ui->tableWidget->setItem(rowcount,5,column5);

        QString price = QString(FoodVec[j].price);
        QString quatity = QString(FoodVec[j].quatity);

        m_sum += (price.toFloat()) * (quatity.toInt());
        QString data = QString("%1").arg(m_sum);
        ui->lineEdit_3->setText(data);  //输出总消费额
     }
}

void childdorm::cashier_refresh() //刷新餐桌状态
{
    QStringList headtext;
    headtext<<"餐桌号"<<"使用状态";
    ui->tableWidget_2->setColumnCount(headtext.count());//列表设置为和headtext相等
    ui->tableWidget_2->setHorizontalHeaderLabels(headtext);//插入表头
    ui->tableWidget_2->setRowCount(0);

    for (int j=1; j<FoodVec.size(); j++)
    {
        int rowcount = ui->tableWidget_2->rowCount();

        ui->tableWidget_2->insertRow(rowcount);

        QTableWidgetItem *column = new QTableWidgetItem(FoodVec[j].table);
        QTableWidgetItem *column1 = new QTableWidgetItem(FoodVec[j].food);

        ui->tableWidget_2->setItem(rowcount,0,column);  //餐桌号
        ui->tableWidget_2->setItem(rowcount,1,column1); //使用状态
     }
}

void childdorm::on_pushButton_4_clicked()//重新登录
{
    this->hide();
    Widget *child = new Widget();
    child->show();
}

void childdorm::on_pushButton_3_clicked()//退出
{
    exit(-1);
}

void childdorm::on_pushButton_clicked()//查询
{
    QString S = ui->lineEdit_2->text(); //桌号
    ui->lineEdit->setText(S);
    char s[5];
    QByteArray ba = S.toLocal8Bit();
    memcpy(s,ba.data(),ba.size()+1);  //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止

    if(S == "\0")
    {
        QMessageBox::warning(this,"提示!","请输入查询桌号!");
    }
    else
    {
        foodVec.clear();
        strcpy(F_head.table,s);
        strcpy(F_head.number,"\0");
        strcpy(F_head.food,"cashier_query");  //放入标志位:查询订单
        strcpy(F_head.price,"\0");
        strcpy(F_head.quatity,"\0");
        strcpy(F_head.post,"\0");
        foodVec.push_back(F_head);    //订单信息标志位放入容器中首元素

        QByteArray a1;
        a1.resize(sizeof(FoodInfo)*foodVec.size());
        memcpy(a1.data(),foodVec.data(),sizeof(FoodInfo)*foodVec.size());
        tcpsocket->write(a1);
    }
}

void childdorm::on_pushButton_2_clicked()//结算
{
    QString S = ui->lineEdit->text(); //桌号
    char s[5];
    QByteArray ba = S.toLocal8Bit();
    memcpy(s,ba.data(),ba.size()+1);  //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止

    QString Sum = ui->lineEdit_3->text(); //消费额
    char sum[5];
    ba = Sum.toLocal8Bit();
    memcpy(sum,ba.data(),ba.size()+1);  //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止

    char name[100];
    ba = m_name.toLocal8Bit();
    memcpy(name,ba.data(),ba.size()+1);  //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止

    if(S == "\0")
    {
        QMessageBox::warning(this,"提示!","请先查询账单!");
    }
    else
    {
        foodVec.clear();
        strcpy(F_head.table,s);  //桌号
        strcpy(F_head.number,"\0");
        strcpy(F_head.food,"cashier_finish");  //放入标志位:结算
        strcpy(F_head.price,sum);  //消费额
        strcpy(F_head.quatity,"\0");
        strcpy(F_head.post,name);  //收银人
        foodVec.push_back(F_head);//订单信息标志位放入容器中首元素

        QByteArray a1;
        a1.resize(sizeof(FoodInfo)*foodVec.size());
        memcpy(a1.data(),foodVec.data(),sizeof(FoodInfo)*foodVec.size());
        tcpsocket->write(a1);

        QMessageBox::warning(this,"提示!","结算成功!");
        //ui->tableWidget->clearContents();  //清空表中数据
        ui->tableWidget->setRowCount(0);   //清空表中数据
        ui->lineEdit->clear();             //清空表中数据
        ui->lineEdit_3->clear();           //清空表中数据
    }
}

void childdorm::on_pushButton_5_clicked()//刷新餐桌状态
{
    foodVec.clear();
    strcpy(F_head.table,"\0");  //桌号
    strcpy(F_head.number,"\0");
    strcpy(F_head.food,"cashier_refresh");  //放入标志位:刷新餐桌状态
    strcpy(F_head.price,"\0");  //消费额
    strcpy(F_head.quatity,"\0");
    strcpy(F_head.post,"\0");
    foodVec.push_back(F_head);  //订单信息标志位放入容器中首元素

    QByteArray a1;
    a1.resize(sizeof(FoodInfo)*foodVec.size());
    memcpy(a1.data(),foodVec.data(),sizeof(FoodInfo)*foodVec.size());
    tcpsocket->write(a1);
}
