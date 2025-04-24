#include "mywidget.h"
#include "ui_mywidget.h"
#include <QPushButton>

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    //创建一个按钮
    QPushButton *btn = new QPushButton;
    //让btn依赖在mywidget窗口中
    btn->setParent (this);
    //显示文本
    btn->setText("第一个按钮");
    QPushButton *btn2 = new QPushButton("第二个按钮",this);
    //移动btn2按钮
    btn2->move(100,100);
    //重置窗口大小
    //resize(600,400);
    //设置窗口标题
    setWindowTitle("第一个窗口");



    //需求 点击我的按钮 关闭窗口
    //参数1 信号的发送者 参数2 发送的信号（函数的地址） 参数3信号的接收者  参数4 处理的槽函数
    connect(btn,&QPushButton::clicked,this,&MyWidget::close);
}

MyWidget::~MyWidget()
{
    delete ui;
}

// ctrl + r 运行
// ctrl + b 编译
