#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //TableWidge
    //设置列数
    ui->tableWidget->setColumnCount(3);

    //设置水平表头
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"姓名"<<"性别"<<"年龄");//匿名函数

    //设置行数
    ui->tableWidget->setRowCount(5);
    //设置正文
//    ui->tableWidget->setItem(0,0,new QTableWidgetItem("亚瑟"));
    QStringList namelist;
    namelist<<"亚瑟"<<"赵云"<<"张飞"<<"关羽"<<"花木兰";
    for(int i = 0;i<5;i++ )
    {int col = 0;
        ui->tableWidget->setItem(i,col++,new QTableWidgetItem(namelist[i]));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

