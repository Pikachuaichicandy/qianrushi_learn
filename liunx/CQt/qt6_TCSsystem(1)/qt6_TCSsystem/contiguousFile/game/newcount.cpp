#include "newcount.h"
#include "ui_newcount.h"
#include <QString>
#include <QFile>
#include <QDebug>
//#include "online.h"
#include <QTimer>
#include <QRegExpValidator>

newcount::newcount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::newcount)
{
    ui->setupUi(this);
    ui->lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]*")));
    ui->lineEdit_2->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9a-zA-Z_@]*")));
}

newcount::~newcount()
{
    delete ui;
}

void newcount::on_pushButton_2_clicked()
{
    this->close();
}

void newcount::on_pushButton_clicked()
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

    }
    file.close();
}
