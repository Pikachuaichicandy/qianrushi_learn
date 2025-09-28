#include "change_status.h"
#include "ui_change_status.h"
#include <QDesktopWidget>
change_status::change_status(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::change_status)
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

change_status::~change_status()
{
    delete ui;
}

void change_status::on_pushButton_2_clicked()//关闭
{
    close();
}

void change_status::on_pushButton_clicked()//修改菜品状态
{
    QSqlQuery query(db);
    char str[100];

    QString number = ui->lineEdit->text();//菜名
    QString limit = ui->comboBox->currentText();//权限

    std::string s1 = number.toStdString();
    std::string s2 = limit.toStdString();

    QString temp=QString::fromUtf8("select 菜名 from 菜单 where 序号 ='%1'").arg(number);
    query.exec(temp);
    query.next();
    if(query.value("菜名").toString() ==  "\0")
    {
        QMessageBox::warning(this,"提示","菜品不存在!请重新输入!");
    }
    else
    {
        sprintf(str,"update  菜单 set 状态 = '%s' where 序号 = '%s'",s2.c_str(),s1.c_str());
        query.exec(str);
         //qDebug()<<str;
         temp=QString::fromUtf8("select 状态 from 菜单 where 序号 ='%1'").arg(number);
         query.exec(temp);
          query.next();
          if(query.value("状态").toString() == limit)
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
