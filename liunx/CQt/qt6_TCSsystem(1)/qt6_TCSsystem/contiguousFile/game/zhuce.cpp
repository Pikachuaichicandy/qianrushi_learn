#include "zhuce.h"
#include "ui_zhuce.h"
#include <QString>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QRegExpValidator>
#include "dialog.h"

ZhuCe::ZhuCe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ZhuCe)
{
    ui->setupUi(this);
    //正则表达式校验器限制输入
    ui->lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]*")));
    ui->lineEdit_2->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9a-zA-Z_@]*")));
}

ZhuCe::~ZhuCe()
{
    delete ui;
}

void ZhuCe::dialogShow(QString str)
{
    Dialog* dialog=new Dialog(this);
    dialog->setText(str);
    dialog->setModal(true);
    dialog->show();
    dialog->exec();
    delete dialog;
}

void ZhuCe::on_pushButton_2_clicked()//返回按钮
{
    this->close();
}

void ZhuCe::on_pushButton_clicked()//注册按钮
{
   QString newCount;
   //指定数据库文件，并且以添加模式打开
   QFile file("C:/Users/75738/Desktop/code/QT_project/game/data.txt");
   if(! file.open(QIODevice::Append)){
       qDebug()<<"文件打开失败"<<endl;
       return;
   }
   //将注册的账号密码添加到data中，并且打开注册成功的对话框
   if(ui->lineEdit->text().length()>=3 && ui->lineEdit_2->text().length()>=3){
       newCount="\n账号："+ui->lineEdit->text()+"\n密码："+ui->lineEdit_2->text()+"\n";
       //文件写入时将QString类型转换为Utf-8
       file.write(newCount.toUtf8());
       dialogShow("注册成功");
       this->close();
   }
   else
       dialogShow("账号密码长度不低于3");
   file.close();
}
