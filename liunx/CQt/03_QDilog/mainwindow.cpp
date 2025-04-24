#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //点击新建按钮 弹出一个对话框
    connect(ui->actionnew,&QAction::triggered,[=](){
        //对话框 分类
        //模态对话框（不可以对其它窗口进行操作） 非模态对话框（可以对其他窗口进行操作）
        //模态创建
//        QDialog DIg(this);
//        DIg.exec();

        //非模态
        QDialog *dlg = new QDialog(this);//放堆上
        dlg->setAttribute(Qt::WA_DeleteOnClose);
        dlg->show();


    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

