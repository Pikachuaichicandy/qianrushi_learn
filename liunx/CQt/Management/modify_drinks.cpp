#include "modify_drinks.h"
#include "ui_modify_drinks.h"
#include <QDesktopWidget>
Modify_drinks::Modify_drinks(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Modify_drinks)
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

Modify_drinks::~Modify_drinks()
{
    delete ui;
}

void Modify_drinks::on_pushButton_clicked()//修改饮品价格
{
    QSqlQuery query(db);
    char str[100];

    QString number = ui->lineEdit->text();//饮品名
    QString price = ui->lineEdit_2->text();//价格

    std::string s1 = number.toStdString();
    std::string s2 = price.toStdString();

    QString temp=QString::fromUtf8("select 饮品名 from 饮品 where 序号 ='%1'").arg(number);
    query.exec(temp);
    query.next();
    if(query.value("饮品名").toString() ==  "\0")
    {
        QMessageBox::warning(this,"提示","饮品不存在!请重新输入!");
    }
    else
    {
        sprintf(str,"update  饮品 set 价格 = '%s' where 序号 = '%s'",s2.c_str(),s1.c_str());
        query.exec(str);
         //qDebug()<<str;
         temp=QString::fromUtf8("select 价格 from 饮品 where 序号 ='%1'").arg(number);
         query.exec(temp);
          query.next();
          if(query.value("价格").toString() == price)
          {
              QMessageBox::warning(this,"提示","修改成功!");
              close();
          }
          else
          {
              QMessageBox::warning(this,"提示","修改失败!");
          }

    }
}

void Modify_drinks::on_pushButton_2_clicked()//关闭
{
    close();
}
