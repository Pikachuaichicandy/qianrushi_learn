#include "add_food.h"
#include "ui_add_food.h"
#include <QDesktopWidget>
#include<QDebug>
Add_food::Add_food(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Add_food)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setFixedSize(600,400);//背景图尺寸
    //this->move(QPoint(100,100));
    this->setWindowIcon(QIcon(":/image/44.png"));//APP图标

   //窗口居中显示
   QDesktopWidget* desktop = QApplication::desktop();
   move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/31.jpg")));//背景图
    setPalette(pal);

    db = QSqlDatabase::addDatabase("QMYSQL"); //添加数据库
//    db.setHostName("192.168.79.129");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("12345678");
    db.setPort(3306);
    db.setDatabaseName("Order");

    if(!db.open())
    {
       QMessageBox::warning(this,"提示","无法连接数据库");
    }
}

Add_food::~Add_food()
{
    delete ui;
}

void Add_food::on_pushButton_2_clicked()//取消
{
    close();
}

void Add_food::on_pushButton_clicked()//添加菜品
{
    QSqlQuery query(db);
    char str[100];

    bool ret = query.exec("select *from 菜单");//数据库执行语句
     if(!ret)
     {
         QSqlError error = query.lastError();
         QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
         QMessageBox::warning(this,"提示",str);
     }
    int count = 0;//总行数
    while (query.next())   //读取下一行信息
    {
        count = query.value("序号").toInt();
    }
    count++;

    QString name = ui->lineEdit->text();//菜名
    QString price = ui->lineEdit_2->text();//价格
    QString limit = ui->comboBox->currentText();//状态

    std::string s1 = name.toStdString();
    std::string s2 = price.toStdString();
    std::string s3 = limit.toStdString();
    QString temp=QString::fromUtf8("select 序号 from 菜单 where 菜名 ='%1'").arg(name);
    query.exec(temp);
    query.next();
    if(query.value("序号").toString() !=  "\0")
    {
        QMessageBox::warning(this,"提示","已有该菜品!");
    }
    else
    {
        query.clear();
        sprintf(str,"insert into 菜单 values(%d,'%s','%s','%s')",count,s1.c_str(),s2.c_str(),s3.c_str());
        query.exec(str);
         //qDebug()<<str;
         temp=QString::fromUtf8("select 序号 from 菜单 where 菜名 ='%1'").arg(name);
         query.exec(temp);
          query.next();
          if(query.value("序号").toString() != "\0")
          {
              QMessageBox::warning(this,"提示","添加成功!");
              close();
          }
          else
          {
              QMessageBox::warning(this,"提示","添加失败!");
          }
          strcpy(M_head.food,s1.c_str());
          strcpy(M_head.number, std::to_string(count).c_str());
          strcpy(M_head.price ,s2.c_str());
          strcpy(M_head.state , s3.c_str());
          MenuVec.push_back(M_head);//菜单信息放入容器中

    }
}
