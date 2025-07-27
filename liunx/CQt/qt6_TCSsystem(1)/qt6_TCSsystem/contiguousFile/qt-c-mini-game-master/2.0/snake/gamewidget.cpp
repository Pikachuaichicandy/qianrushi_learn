#include "gamewidget.h"
#include "./ui_gamewidget.h"
#include"config.h"
#include <QFont>
#include<QStandardItemModel>
#include<QListView>
#include<QDebug>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWidget)
{
    ui->setupUi(this);

    //初始化类
    isAlPattern=false;

    setFixedSize(10+MWidth+190,10+MHeight+10);

    initStore();
    initToolWidget();
    initMainScene();
    initListview();

    connect(m_mainScene,&MainScene::updateScore,this,&GameWidget::updateLcdNum);
    connect(m_mainScene,&MainScene::gameOverOrStart,this,[&](){
        isAlPattern=false;
    });
    connect(ui->start,&QPushButton::clicked,m_mainScene,&MainScene::start);
    connect(ui->pause,&QPushButton::clicked,m_mainScene,&MainScene::pause);
    connect(ui->over,&QPushButton::clicked,m_mainScene,&MainScene::gameover);
    connect(ui->store,&QPushButton::clicked,this,[&](){
        m_store->show();
    });
    connect(m_store,&Store::updatePaint,m_mainScene,&MainScene::setbgroundPix);
}

GameWidget::~GameWidget()
{
    delete m_store;
    delete ui;
    qDebug()<<"gamewidget析构";
}

void GameWidget::updateLcdNum(int num)
{
    ui->lcdNumber->display(num);
}

void GameWidget::initToolWidget()
{
    ui->widget->setGeometry(10+MWidth+10,10,180,MHeight);

    int h=40;
    QFont font;
    font.setBold(true);
    font.setPointSize(12);
    ui->label->setFont(font);

    ui->lcdNumber->setFixedHeight(h);
    ui->lcdNumber->setStyleSheet("color: red;");
    //ui->lcdNumber->display(33);

    ui->start->setFixedHeight(h);
    ui->pause->setFixedHeight(h);
    ui->over->setFixedHeight(h);
    ui->store->setFixedHeight(h);
    ui->store->setIcon(QIcon(":/images/buttonIcon/store.png"));
    ui->BMusic->setFixedHeight(h);
    ui->BMusic->setIcon(QIcon(":/images/buttonIcon/pause.png"));
    ui->AlBfs->setFixedHeight(h);

}

void GameWidget::initMainScene()
{
    m_mainScene=new MainScene(this);
    m_mainScene->setGeometry(10,10,MWidth,MHeight);
    //qDebug()<<mainScene->width();
}

void GameWidget::initStore()
{
    m_store=new Store;
}

void GameWidget::initListview()
{
    QStringList data;
    data.append("目前贪吃蛇1.4 Al寻路版");
    data.append("贪吃蛇1.0 普通版");
    data.append("贪吃蛇1.1 稳定版");
    data.append("贪吃蛇1.2 加音乐版");
    data.append("贪吃蛇1.4 Al寻路版");
    data.append("贪吃蛇1.6 商店版");
    data.append("贪吃蛇2.0 TCP联机版");

    QStandardItemModel* model=new QStandardItemModel(this);
    //model.setRowCount(4);

    foreach (const QString& text, data) {
        QStandardItem *item=new QStandardItem(text);
        model->appendRow(item);
    }
    //设置模型
    ui->listView->setModel(model);
}





void GameWidget::on_BMusic_clicked()
{
    if(m_mainScene->musicControl->isStartPlayBG())
    {
        m_mainScene->musicControl->stopBackgroundMusic();
        ui->BMusic->setIcon(QIcon(":/images/buttonIcon/pause.png"));
    }
    else
    {
        m_mainScene->musicControl->startBackgroundMusic();
        ui->BMusic->setIcon(QIcon(":/images/buttonIcon/play.png"));
    }
    m_mainScene->setFocus();
}


void GameWidget::on_AlBfs_clicked()
{
    if(!isAlPattern)
    {
        m_mainScene->start();
        isAlPattern=true;
    }
    m_mainScene->startFindWay();
    m_mainScene->setFocus();
}

