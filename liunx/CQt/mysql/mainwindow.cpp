#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("12345678");
    db.setPort(3306);
    db.setDatabaseName("Order");
    if(!db.open())
    {
        //       qDeug()<<"erro:"<<db.lastError().text();
        qDebug() << "Error:" << db.lastError().text();
        QMessageBox::warning(this,"提示","无法连接数据库");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

