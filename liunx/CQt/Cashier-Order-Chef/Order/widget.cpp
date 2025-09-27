#include "widget.h"
#include "ui_widget.h"
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

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->tabWidget->setTabText(0,"主餐");
    ui->tabWidget->setTabText(1,"饮品");
    this->setFixedSize(1500,800);//背景图尺寸
    this->setWindowTitle(QStringLiteral("点餐窗口"));
   // this->move(QPoint(100,100));
    this->setWindowIcon(QIcon(":/image/28.png"));//APP图标

    //居中显示
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    //add back picture
    QPalette pal = this->palette();
    //pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/27.jpg")));//背景图
    setPalette(pal);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);

    //设置为只读模式
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应
    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应

    Init();//初始化
    client();//客服端操作

    QStringList headtext2;
    headtext2<<"序号"<<"餐名"<<"价格"<<"数量"<<"备注";
    ui->tableWidget->setColumnCount(headtext2.count());//列表设置为和headtext相等
    ui->tableWidget->setHorizontalHeaderLabels(headtext2);//插入表头
    ui->tableWidget->setRowCount(0);
    m_sum = 0;

    child = new childdorm();
    child->hide();
    child->getData(this);   //通过this把父窗口的指针传到子窗口当中
    connect(child,SIGNAL(Interface()),this,SLOT(comeback()));//信号槽
    connect(child,SIGNAL(Inter_delete()),this,SLOT(Delete()));//信号槽
}

Widget::~Widget()
{
    delete ui;
}

void Widget::Init()//初始化
{
    tcpsocket = new QTcpSocket(this);//创建客户端套接字


    connect(tcpsocket,&QTcpSocket::connected,//请求连接信号
           [=]()
    {
        //ui->textEdit->setText("成功连接服务器");
    }
    );

//    QString ip = "192.168.79.129";
    QString ip = "192.168.174.129";
    qint16 port = 8888;
    tcpsocket->connectToHost(QHostAddress(ip),port);

    strcpy(F_head.table,"\0");
    strcpy(F_head.number,"\0");
    strcpy(F_head.food,"order");  //放入标志位
    strcpy(F_head.price,"\0");
    strcpy(F_head.price,"\0");
    strcpy(F_head.post,"\0");
    FoodVec.push_back(F_head);//订单信息标志位放入容器中首元素

    strcpy(F_head.food,"order_up");  //放入标志位
    FoodVec_no.push_back(F_head);//订单信息标志位放入容器中首元素
}

void Widget::client()        //客服端操作
{
    strcpy(F_head.table,"\0");
    strcpy(F_head.number,"\0");
    strcpy(F_head.food,"menu");  //放入标志位
    strcpy(F_head.price,"\0");
    strcpy(F_head.price,"\0");
    strcpy(F_head.post,"\0");
    foodVec.push_back(F_head);//订单信息标志位放入容器中首元素
    QByteArray a1;
    a1.resize(sizeof(FoodInfo)*foodVec.size());
    memcpy(a1.data(),foodVec.data(),sizeof(FoodInfo)*foodVec.size());
    tcpsocket->write(a1);


    connect(tcpsocket,&QTcpSocket::readyRead,
            [=]()
    {
        QByteArray array = tcpsocket->readAll();
        flag(array);     //数据解析
    }
    );
}

void Widget::flag(QByteArray array) //服务端信息判断
{
    vector<MenuInfo> menuVec; //菜单容器
    menuVec.resize(array.size()/sizeof(MenuInfo));
    memcpy(menuVec.data(),array.data(),array.size());
    qDebug()<<menuVec[0].food;
    if(strcmp(menuVec[0].food,"menu")==0)
    {
        MenuVec = menuVec;
        Menu_print();
    }
    else if(strcmp(menuVec[0].food,"chef_in")==0)
    {
        Chef_in(menuVec);
    }

}

void Widget::Chef_in(vector<MenuInfo> menuVec)//上菜
{
    for(int i = 0; i < FoodVec_no.size(); i++)
    {
        if(strcmp(FoodVec_no[i].number,menuVec[1].number) == 0)
        {
            strcpy(F_head.table,FoodVec_no[i].table);
            strcpy(F_head.number,FoodVec_no[i].number);
            strcpy(F_head.food,FoodVec_no[i].food);
            strcpy(F_head.price,FoodVec_no[i].price);
            strcpy(F_head.quatity,FoodVec_no[i].quatity);
            strcpy(F_head.post,FoodVec_no[i].post);
            FoodVec_in.push_back(F_head);//将上菜信息放入已上菜单容器中
            FoodVec_no.erase(FoodVec_no.begin ()+i);//删除未上菜单中该元素
        }
    }
    for(int i = 1; i < FoodVec.size(); i++)
    {
        if(strcmp(FoodVec[i].number,menuVec[1].number) == 0)
        {
            strcpy(FoodVec[i].table,"0");
        }
    }
    child->getvec(FoodVec_no,FoodVec_in);
}

void Widget::Menu_print()//打印菜单
{
    QStringList headtext;
    headtext<<"序号"<<"餐名"<<"价格"<<"状态";
    ui->tableWidget_2->setColumnCount(headtext.count());     //列表设置为和headtext相等
    ui->tableWidget_2->setHorizontalHeaderLabels(headtext);  //插入表头
    ui->tableWidget_2->setRowCount(0);

    QStringList headtext1;
    headtext1<<"序号"<<"饮品名"<<"价格"<<"状态";
    ui->tableWidget_3->setColumnCount(headtext1.count());    //列表设置为和headtext相等
    ui->tableWidget_3->setHorizontalHeaderLabels(headtext1); //插入表头
    ui->tableWidget_3->setRowCount(0);

    for (int j=1;j< MenuVec.size();j++)
    {
        QString temp = QString(MenuVec[j].number);
        int number = temp.toInt();
        if( number < 20000)
        {
            int rowcount = ui->tableWidget_2->rowCount();
            ui->tableWidget_2->insertRow(rowcount);
            QTableWidgetItem *column = new QTableWidgetItem(MenuVec[j].number);
            QTableWidgetItem *column1 = new QTableWidgetItem(MenuVec[j].food);
            QTableWidgetItem *column2 = new QTableWidgetItem(MenuVec[j].price);
            QTableWidgetItem *column3 = new QTableWidgetItem(MenuVec[j].state);

            ui->tableWidget_2->setItem(rowcount,0,column);
            ui->tableWidget_2->setItem(rowcount,1,column1);
            ui->tableWidget_2->setItem(rowcount,2,column2);
            ui->tableWidget_2->setItem(rowcount,3,column3);
        }
        else
        {
            int rowcount = ui->tableWidget_3->rowCount();
            ui->tableWidget_3->insertRow(rowcount);
            QTableWidgetItem *column = new QTableWidgetItem(MenuVec[j].number);
            QTableWidgetItem *column1 = new QTableWidgetItem(MenuVec[j].food);
            QTableWidgetItem *column2 = new QTableWidgetItem(MenuVec[j].price);
            QTableWidgetItem *column3 = new QTableWidgetItem(MenuVec[j].state);

            ui->tableWidget_3->setItem(rowcount,0,column);
            ui->tableWidget_3->setItem(rowcount,1,column1);
            ui->tableWidget_3->setItem(rowcount,2,column2);
            ui->tableWidget_3->setItem(rowcount,3,column3);
        }
     }
}

void Widget::on_pushButton_3_clicked()//提交订单
{
    // 添加数据校验
    if(isnew_order == false)
    {
        QMessageBox::warning(this, "提示", "请先添加订单！");
        return;
    }
    QString S = ui->lineEdit->text(); //桌号
    char s[5];
    QByteArray ba = S.toLocal8Bit();
    memcpy(s,ba.data(),ba.size()+1);  //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止

    if(S == "\0")
    {
        QMessageBox::warning(this,"提示!","请填写桌号!");
    }
    else
    {
       // qDebug()<< MenuVec[0].food;
        strcpy(FoodVec[0].table,s);
        int rowcount = ui->tableWidget->rowCount();//总行数
        for (int j=0;j<rowcount;j++)
        {
            QString str = ui->tableWidget->item(j,0)->text(); //获取序号
            QString str1 = ui->tableWidget->item(j,1)->text();//获取菜名
            QString str2 = ui->tableWidget->item(j,2)->text();//获取价格
            QString str3 = ui->tableWidget->item(j,3)->text();//获取价格
            QString str4 = ui->tableWidget->item(j,4)->text();//获取备注

            strcpy(F_head.table,s);
            ba = str.toLocal8Bit();
            memcpy(F_head.number,ba.data(),ba.size()+1);  //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
            ba = str1.toLocal8Bit();
            memcpy(F_head.food,ba.data(),ba.size()+1);    //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
            ba = str2.toLocal8Bit();
            memcpy(F_head.price,ba.data(),ba.size()+1);   //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
            ba = str3.toLocal8Bit();
            memcpy(F_head.quatity,ba.data(),ba.size()+1); //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
            ba = str4.toLocal8Bit();
            memcpy(F_head.post,ba.data(),ba.size()+1);    //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止

            FoodVec.push_back(F_head);//菜品信息放入容器中
            FoodVec_no.push_back(F_head);//菜品信息放入未上菜单容器中
        }

        this->hide();
        child->getvec(FoodVec_no,FoodVec_in);
        child->show();

        //向服务端发送订单信息
        QByteArray a1;
        a1.resize(sizeof(FoodInfo)*FoodVec.size());
        memcpy(a1.data(),FoodVec.data(),sizeof(FoodInfo)*FoodVec.size());
        tcpsocket->write(a1);
    }
    isnew_order = false;
}

void Widget::on_pushButton_4_clicked()//退出
{
    exit(-1);
}

void Widget::on_pushButton_clicked()//添加菜品
{
    int flag = 0;
    char number[10];
    QString s = ui->lineEdit_2->text();//餐品序号
    QString text = ui->lineEdit_3->text();//份数
    if(s == "\0" || text == "\0" )
    {
        QMessageBox::warning(this,"提示!","请填写桌号,份数!");
    }
    QByteArray ba = s.toLocal8Bit();
    memcpy(number,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
    QTableWidgetItem *column = new QTableWidgetItem(s);
    QString s1;
    QString s2;
    for(int i = 0; i < MenuVec.size(); i++)
    {
        if(strcmp(MenuVec[i].number,number) == 0)
        {
            s1 = QString(MenuVec[i].food);
            s2 = QString(MenuVec[i].price);
            flag = 1;
        }
    }

    QTableWidgetItem *column1 = new QTableWidgetItem(s1);

    QTableWidgetItem *column2 = new QTableWidgetItem(s2);

    QString s3 = ui->lineEdit_3->text();//餐品数量
    QTableWidgetItem *column3 = new QTableWidgetItem(s3);

    QString s4 = ui->lineEdit_4->text();//备注
    QTableWidgetItem *column4 = new QTableWidgetItem(s4);

    if(flag == 1)
    {
        int rowcount = ui->tableWidget->rowCount();//总行数
        ui->tableWidget->insertRow(rowcount);
        ui->tableWidget->setItem(rowcount,0,column);
        ui->tableWidget->setItem(rowcount,1,column1);
        ui->tableWidget->setItem(rowcount,2,column2);
        ui->tableWidget->setItem(rowcount,3,column3);
        ui->tableWidget->setItem(rowcount,4,column4);
        m_sum = m_sum+s2.toFloat()*s3.toInt();
        ui->lcdNumber->display(m_sum);
    }
    else
    {
        QMessageBox::warning(this,"提示","输入有误!");
    }
    isnew_order = true;
}

void Widget::on_pushButton_2_clicked() //删除
{
    int currow = ui->tableWidget->currentRow();//获取行号
    if(currow == -1)
    {
        QMessageBox::warning(this,"提示","操作有误!");
    }
    else
    {
        ui->tableWidget->removeRow(currow);//删除当前行
    }
}

void Widget::comeback()//信号槽,显示父窗口
{
    //判断该桌号是否点过餐
    if(FoodVec.begin() != FoodVec.end())
    {
        QString Table = FoodVec[1].table;
        ui->lineEdit->setText(Table);

        ui->lineEdit->setFocusPolicy(Qt::NoFocus);  //设置为只读模式
    }
    ui->tableWidget->setRowCount(0);
    m_sum = 0;
    this->show();
}

void Widget::Delete()//信号槽,退餐
{
    child->Delete_food();
}
