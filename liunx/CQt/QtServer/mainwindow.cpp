#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->port->setText("8899");
    setWindowTitle("TCP - 服务器");
        // 创建 QTcpServer 对象
        m_s = new QTcpServer(this);
        // 检测是否有新的客户端连接
        connect(m_s, &QTcpServer::newConnection, this, [=]()
        {
            QTcpSocket *m_tcp = m_s->nextPendingConnection();

            m_status->setPixmap(QPixmap(":/img/R.png").scaled(20,20));

            //检查是否可以接收数据
            connect(m_tcp,&QTcpSocket::readyRead,this,[=]()
            {
                QByteArray data = m_tcp->readAll();
                ui->record->append("client say:"+data);

            });

            connect(m_tcp,&QTcpSocket::disconnected,this,[=]()
            {
                m_tcp->close();
                m_tcp->deleteLater(); //delete m_tcp
            });
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


void MainWindow::on_setListen_clicked()
{
    unsigned short port = ui->port->text().toUShort();
    //启动监听
    m_s->listen(QHostAddress::Any,port);
    ui->setListen->setDisabled(true);
    //等待客户端连接
}

void MainWindow::on_sendMsg_clicked()
{
//    QString msg = ui->msg->toPlainText();
//    m_tcp->write(msg.toUtf8());
//     ui->record->append("server say:"+msg);

     QString msg = ui->msg->toPlainText();
         // 确保连接没有被关闭后再发送数据
        // if (m_tcp->state() == QTcpSocket::ConnectedState) {
             m_tcp->write(msg.toUtf8());
             ui->record->append("server say: " + msg);
//         } else {
//             ui->record->append("Connection is closed!");
//         }
}
