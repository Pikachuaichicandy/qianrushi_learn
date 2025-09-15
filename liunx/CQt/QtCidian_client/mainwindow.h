#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include<QTcpSocket>
#include<QLabel>
#include <QHostAddress>
#include <QString>
#include <QPushButton>
#include <QMessageBox>
#include "projectstruct.h"
#include "cd_work.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connect_clicked();

    void on_disconnect_clicked();

    void on_sendmsg_clicked();

    void on_cd_register_clicked();

    void on_login_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *m_tcp;
    QLabel *m_status;
    cd_work cd_work;
};
#endif // MAINWINDOW_H
