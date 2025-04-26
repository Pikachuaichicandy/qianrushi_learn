#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QColorDialog>
#include <QFileDialog>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //点击新建按钮 弹出一个对话框
    connect(ui->actionnew, &QAction::triggered, [=]() {
        //对话框 分类
        //模态对话框（不可以对其它窗口进行操作） 非模态对话框（可以对其他窗口进行操作）
        //模态创建
//        QDialog DIg(this);
//        DIg.exec();

        //非模态
        QDialog *dlg = new QDialog(this); // 放堆上
        dlg->setAttribute(Qt::WA_DeleteOnClose);
//        dlg->show();

        //消息对话框
        //错误对话框
//        QMessageBox::critical(this,"critical","错误");

        //消息对话框
//        QMessageBox::information(this,"info","消息");

       //问题对话框
       //参数1 父亲 参数2 标题 参数3 提示内容 参数4 按键模型 参数5 默认关联回车按键
//        if (QMessageBox::Save == QMessageBox::question(
//                this, "question", "提问", QMessageBox::Save | QMessageBox::Cancel, QMessageBox::Cancel))
//        {
//            qDebug() << "选择的是保存";
//        }
//        else {
//            qDebug() << "选择是取消";
//        }
        //其它对话框
        //颜色对话框
//        QColorDialog::getColor(QColor(255,0,0));
        //文件对话框 参数1 父亲 参数2 标题 参数3 默认打开路径 参数4 过lu文件格式
        //返回值 是选取路径
        QFileDialog::getOpenFileName(this,"打开文件","D:\\OneDrive\\桌面\\liunx学习");

        //字体对话框
        //返回值
        bool flag;
        QFontDialog::getFont(&flag,QFont("宋体",36));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
