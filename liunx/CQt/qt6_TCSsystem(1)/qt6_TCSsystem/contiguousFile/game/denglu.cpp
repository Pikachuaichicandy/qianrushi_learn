#include "denglu.h"
#include "ui_denglu.h"
#include <QDebug>
#include <QRegExpValidator>
#include <QString>
#include <QDebug>
#include <QThread>
#include <QTimer>


denglu::denglu(QWidget *parent) :
    QWidget(parent),
    success(false),
    ui(new Ui::denglu)
{
    ui->setupUi(this);
    this->resize(1004,800);
    //正则表达式校验器限制输出的内容
    ui->lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]*")));
    ui->lineEdit_2->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9a-zA-Z_@]*")));
}

denglu::~denglu()
{
    delete ui;
}

void denglu::dialogShow(QString str)
{
    Dialog* dialog=new Dialog(this);
    dialog->setText(str);
    dialog->setModal(true);
    dialog->show();
    dialog->exec();
    delete dialog;
}



void denglu::on_pushButton_clicked()
{
    qDebug()<<"123"<<endl;
}

void denglu::on_pushButton_3_clicked()//密码可视化
{
    if(ui->lineEdit_2->echoMode()==QLineEdit::Password)
        ui->lineEdit_2->setEchoMode(QLineEdit::Normal);
    else
        ui->lineEdit_2->setEchoMode(QLineEdit::Password);
}

void denglu::on_pushButton_2_clicked()//登录按钮
{
    QString countAndPassword;
    QString nowText;
    //指定文件路径，并且打开文件
    QString fileName = "F:/qt6project/qt6_TCSsystem/contiguousFile/game/data.txt";
    file.setFileName(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"文件打开失败"<<endl;
        return ;
    }
    //获取data库的字符串和当前账号密码的字符串
    countAndPassword=(QString)file.readAll();
    nowText="账号："+ui->lineEdit->text()+"\n"+"密码："+ui->lineEdit_2->text()+"\n";
    //当前账号密码在字符串中，则登录成功。反正则登录失败。
    if(countAndPassword.indexOf(nowText) != -1){
        //登录成功则弹出对话框,并且success=true
        dialogShow("登录成功！");
        success=true;
    }
    else{
        dialogShow("账号或密码错误！");
    }
    file.close();
}

void denglu::on_pushButton_4_clicked()
{
    //打开注册的对话窗口
    zhuceWidget = new ZhuCe(this);
    zhuceWidget->resize(1004,800);
    zhuceWidget->setModal(true);
    zhuceWidget->show();
    zhuceWidget->exec();
}

