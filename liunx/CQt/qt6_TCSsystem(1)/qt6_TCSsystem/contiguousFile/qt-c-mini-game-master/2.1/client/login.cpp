#include "login.h"
#include<QVBoxLayout>
#include<QMessageBox>
#include<QDebug>

Login::Login(QWidget *parent)
    : QWidget{parent}
{
    m_gameWidget=nullptr;
    m_snakeWidget=nullptr;

    //创建子线程
    m_netWork=new NetWork();
    QThread* subThread=new QThread;
    m_netWork->moveToThread(subThread);
    subThread->start();

    init();

    qDebug()<<"login线程："<<QThread::currentThreadId();

    //关于线程的资源释放
    connect(this,&Login::destroyed,[=](){
        subThread->quit();
        subThread->wait();
        subThread->deleteLater();
    });
    connect(subThread,&QThread::finished,m_netWork,&NetWork::deleteLater);
    //连接服务器
    connect(this,&Login::startConnectServer,m_netWork,&NetWork::startConnectServer);
    emit startConnectServer();
}

Login::~Login()
{
    if(m_gameWidget!=nullptr)
        delete m_gameWidget;
    if(m_snakeWidget!=nullptr)
        delete m_snakeWidget;
    qDebug()<<"login析构";
}

void Login::returnLogin()
{
    m_snakeWidget->hide();
    this->show();
}

void Login::init()
{
    // 创建垂直布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_name=new QLineEdit(this);
    m_roomIp=new QLineEdit(this);
    m_stand_alone=new QPushButton(this);
    m_online=new QPushButton(this);

    m_name->setFixedSize(280,40);
    m_name->setPlaceholderText("用户名（中文<=6，英文<=18）");
    m_roomIp->setFixedSize(280,40);
    m_roomIp->setPlaceholderText("房间号（4位）");

    m_stand_alone->setFixedHeight(40);
    m_stand_alone->setText("单机游戏");
    m_online->setFixedHeight(40);
    m_online->setText("网络游戏");

    //测试
    m_name->setText("鲜活11s");
    m_roomIp->setText("1111");

    setFixedSize(m_name->width()+20,300);

    layout->addWidget(m_name);
    layout->addWidget(m_roomIp);
    layout->addWidget(m_stand_alone);
    layout->addWidget(m_online);
    //设置布局
    setLayout(layout);

    initConnect();
}

void Login::initConnect()
{
    connect(m_stand_alone,&QPushButton::clicked,this,&Login::start_stand_alone);
    connect(m_online,&QPushButton::clicked,this,&Login::start_online);

    //连接m_netWork网络函数
    connect(this,&Login::send,m_netWork,&NetWork::send);
}

void Login::start_stand_alone()
{
    m_gameWidget=new GameWidget;
    this->hide();
    m_gameWidget->show();
}

void Login::start_online()
{
    if(verify())
    {
        m_netWork->m_roomIp=m_roomIp->text().toInt();
        m_netWork->m_name=m_name->text();
        if(m_snakeWidget==nullptr)
        {
            m_snakeWidget=new SnakeWidget;

            connect(m_netWork,&NetWork::snakeData,m_snakeWidget,&SnakeWidget::recvData);
            connect(m_snakeWidget,&SnakeWidget::changeDir,m_netWork,&NetWork::send);
            connect(m_netWork,&NetWork::changeDir,m_snakeWidget,&SnakeWidget::recvDir);
            connect(m_snakeWidget,&SnakeWidget::sendDeath,m_netWork,&NetWork::send);
            connect(m_netWork,&NetWork::sendUserDeath,m_snakeWidget,&SnakeWidget::recvUserDeath);
            connect(m_netWork,&NetWork::gameOver,m_snakeWidget,&SnakeWidget::gameOver);
            connect(m_snakeWidget,&SnakeWidget::returnLogin,this,&Login::returnLogin);
            connect(m_netWork,&NetWork::foodPoint,m_snakeWidget,&SnakeWidget::recvFoodPoint);
        }
        else
            m_snakeWidget->init();

        m_snakeWidget->show();
        this->hide();
        emit send(3,m_roomIp->text().toInt());

        return;
    }
    QMessageBox::critical(this,"出现问题","网络未连接/用户名长度错误/房间号位数错误！");
}

bool Login::verify()
{
    if(m_netWork->isConnectNetWork() && m_name->text().size()<=18 &&
        m_name->text().size()>0 && m_roomIp->text().size()==4)
        return true;
    return false;
}
