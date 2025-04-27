#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置单选地按钮 男默认选择
    ui->rbtnman->setChecked(true);
    //选中woman 显示信息
    connect(ui->rbtnwoman,&QRadioButton::clicked,[=](){
    qDebug()<<"选中了women";
    });
    //多选按钮
    connect(ui->cbok1,&QCheckBox::clicked,[=](){});
//    //利用listwidege写诗
//    QListWidgetItem * item = new QListWidgetItem("我是最强的");
//    //将一行诗放入到listwidge控件中
//    ui->listWidget->addItem(item);
//    //对齐方式
//    item->setTextAlignment(Qt::AlignHCenter);

    //Qstringlist Qlist<QString>
    QStringList list;
    list<<"1"<<"2";
    ui->listWidget->addItems(list);
}

MainWindow::~MainWindow()
{
    delete ui;
}

