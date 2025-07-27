#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    //打开选择模式的主窗口
    ui->setupUi(this);
    this->resize(1200,800);
    //主窗口先被登录窗口覆盖
    dengluWidget = new denglu(this);
    dengluWidget->setGeometry(0,0,1200,800);
    dengluWidget->show();

    //定时器检测是否登录成功
    timer=new QTimer(this);
    timer->start(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));


}

Widget::~Widget()
{
    delete ui;
}

void Widget::deleteStart()//删除开始页面的控件
{
    delete ui->pushButton;
    delete ui->pushButton_2;
    delete ui->pushButton_3;
    delete ui->pushButton_4;
    delete ui->label_8;
    delete ui->label_9;

}

void Widget::GameWidgetShow()//显示游戏的各窗口
{
    ui->label_4->setText("上澳门新葡京，圆财富自由梦");
    ui->label_5->setText("账户余额:");
    ui->label_6->setText("====>");
    //贪吃蛇游戏窗口
    if(mode == GameDifficulty::common)
        snakeWidget=new snake("F:/qt6project/qt6_TCSsystem/contiguousFile/game/common.MP3",300,this);
    else
        snakeWidget=new snake("F:/qt6project/qt6_TCSsystem/contiguousFile/game/hard.MP3",100,this);
    snakeWidget->setGeometry(2,100,1000,600);
    snakeWidget->show();

    //轮播文字窗口
    movetextWidget=new movetext("澳门新葡京娱乐城:www.xxxx.com", this);
    movetextWidget->setGeometry(0,0,1004,98);
    movetextWidget->show();

    //自定义动画窗口
    moneyUpWidget=new moneyUp(this);
    moneyUpWidget->setGeometry(691,730,841,730);
    moneyUpWidget->show();
}

void Widget::GameWidgetClose()
{
    //关闭游戏窗口，且将各指针置空
    ui->label_4->setText("");
    ui->label_5->setText("");
    ui->label_6->setText("");
    if(snakeWidget && movetextWidget && moneyUpWidget){
        delete snakeWidget;
        delete movetextWidget;
        delete moneyUpWidget;
        snakeWidget=nullptr;
        movetextWidget=nullptr;
        moneyUpWidget=nullptr;
    }
}

void Widget::on_pushButton_clicked()
{
   mode=GameDifficulty::common;
   deleteStart();
   GameWidgetShow();
}

void Widget::on_pushButton_2_clicked()
{
    mode=GameDifficulty::hard;
    deleteStart();
    GameWidgetShow();
}

void Widget::on_pushButton_3_clicked()
{
    this->close();
}

void Widget::on_pushButton_4_clicked()
{
    ranklistWidget = new ranklist(this);
    ranklistWidget->resize(1004,800);
    ranklistWidget->show();
    ranklistWidget->exec();
    delete ranklistWidget;
    ranklistWidget = nullptr;
}


void Widget::onTimeOut()
{
    //登录成功则关闭登录窗口和定时器
    if(dengluWidget && dengluWidget->success){
        dengluWidget->close();
        delete dengluWidget;
        dengluWidget=nullptr;
        delete timer;
    }

}

