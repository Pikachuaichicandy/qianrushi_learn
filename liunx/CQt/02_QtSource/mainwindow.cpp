#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->actionnew->setIcon(QIcon("F:/32keil_chenxundaimakaifa/liunx/CQt/IMG/pikachu.jpg"));
    //使用添加QT资源 “：+前缀名+文件名”
    ui->actionnew->setIcon(QIcon(":/IMG/pikachu.jpg"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

