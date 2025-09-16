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


void MainWindow::registerUser(MSGInfo data)
{
    // 先检查数据库中是否已有相同用户名
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM usr WHERE username = :username");
    checkQuery.bindValue(":username", data.name);

    if (!checkQuery.exec())
    {
        qDebug() << "查询失败:" << checkQuery.lastError().text();
        return;
    }

    // 获取查询结果
    checkQuery.next();
    int count = checkQuery.value(0).toInt();

    // 如果用户名已存在，提示用户
    if (count > 0)
    {
        qDebug() << "用户名已存在，请选择其他用户名！";
//        ui->record->append("用户名已存在，请选择其他用户名！");
        return;
    }
    QSqlQuery query;
    QString sql = QString("INSERT INTO usr VALUES(null,'%1', '%2')").arg(data.name).arg(data.pass);
    // 将 SQL 查询预编译
    query.prepare(sql);
    //绑定参数
    query.bindValue(":id",data.name);
    query.bindValue(":id",data.pass);
    if (!query.exec(sql))
    {
        qDebug() << "注册失败:" << query.lastError().text();
//        ui->record->append("注册失败....");
        return;
    }

    // 将数据添加到 msgInfo QVector 中
    msgInfo.append(data);
    qDebug() << "Successfully added a new student:" << data.name<<data.pass;
//    ui->record->append("Successfully added a new student:%s,%s",data.name,data.pass;");

//    this->saveAllStudentInfo();

}

bool MainWindow::loginUser(MSGInfo data, QString &result)
{
    QSqlQuery query;
    QString sql = QString("SELECT * FROM usr WHERE name='%1' AND pass='%2'").arg(data.name).arg(data.pass);
    //绑定参数
    query.bindValue(":name",data.name);
    query.bindValue(":pass",data.pass);
    query.prepare(sql);
    if (query.exec(sql) && query.next()) {
        result = "OK";
        return true;
    } else {
        result = "用户名或密码错误";
        return false;
    }
}




void MainWindow::on_sendMsg_clicked()
{
    QString msg = ui->msg->toPlainText();
    // 确保连接没有被关闭后再发送数据
    if (m_tcp->state() == QTcpSocket::ConnectedState) {
        m_tcp->write(msg.toUtf8());
        ui->record->append("server say: " + msg);
    } else {
        ui->record->append("Connection is closed!");
    }

}

void MainWindow::on_setListen_clicked()
{
    unsigned short port = ui->port->text().toUShort();
    //启动监听
    m_s->listen(QHostAddress::Any,port);
    ui->setListen->setDisabled(true);
    //等待客户端连接

}
