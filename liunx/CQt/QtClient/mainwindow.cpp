#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->port->setText("8899");
    ui->ip->setText("127.0.0.1");
    setWindowTitle("客户端");

    ui->disconnect->setDisabled(true);

    // 创建监听的服务器对象
    m_tcp = new QTcpSocket(this);


    //检查是否可以接收数据
    connect(m_tcp,&QTcpSocket::readyRead,this,[=]()
    {
        QByteArray data = m_tcp->readAll();
        ui->record->append("server say:"+data);

    });


    connect(m_tcp,&QTcpSocket::disconnected,this,[=]()
    {
        m_tcp->close();
        m_tcp->deleteLater(); //delete m_tcp
        m_status->setPixmap(QPixmap(":/img/e.jpg").scaled(20,20));
        ui->record->append("已服务器和客户端断开连接...");
        ui->connect->setDisabled(false);
        ui->disconnect->setDisabled(true);
    });

    connect(m_tcp,&QTcpSocket::connected,this,[=]()
    {

        m_status->setPixmap(QPixmap(":/img/R.png").scaled(20,20));
        ui->record->append("已成功连接服务器...");
        ui->connect->setDisabled(true);
        ui->disconnect->setDisabled(false);
    });


       //状态栏
       m_status = new QLabel;
       m_status->setPixmap(QPixmap(":/img/e.jpg").scaled(20,20));
       ui->statusbar->addWidget(new QLabel("连接状态："));
       ui->statusbar->addWidget(m_status);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_sendMsg_clicked()
{
//    QString msg = ui->msg->toPlainText();
//    m_tcp->write(msg.toUtf8());
//    ui->record->append("server say:"+msg);

    QString msg = ui->msg->toPlainText();
       if (m_tcp->state() == QTcpSocket::ConnectedState) {
           m_tcp->write(msg.toUtf8());
           ui->record->append("client say: " + msg);
       } else {
           ui->record->append("连接已断开，无法发送消息！");
       }
}

void MainWindow::on_connect_clicked()
{
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();
    m_tcp->connectToHost(QHostAddress(ip),port);
}

void MainWindow::on_disconnect_clicked()
{
    m_tcp->close();
    ui->connect->setDisabled(false);
    ui->disconnect->setDisabled(true);
}
