#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "config.h"
#include <QFont>
#include <QStandardItemModel>
#include <QListView>
#include <QDebug>
#include <QStringList>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWidget)
{
    ui->setupUi(this);

    setFixedSize(10+MWidth+190,10+MWidth+10);

    initToolWidget();
    initListview();
    initMainScene();

    connect(mainScene,&MainScene::updateScore,this,&GameWidget::updateLcdNum);
    connect(ui->start,&QPushButton::clicked,mainScene,&MainScene::start);
    connect(ui->pause,&QPushButton::clicked,mainScene,&MainScene::pause);
    connect(ui->over,&QPushButton::clicked,mainScene,&MainScene::gameover);
}

GameWidget::~GameWidget()
{
    delete ui;
}

void GameWidget::updateLcdNum(int num)
{
    ui->lcdNumber->display(num);
}

void GameWidget::initToolWidget()
{
    ui->widget->setGeometry(10+MWidth+10,10,180,MWidth);

    QFont font;
    font.setBold(true);
    font.setPointSize(12);
    ui->label->setFont(font);

    ui->lcdNumber->setFixedHeight(40);
    ui->lcdNumber->setStyleSheet("color:red;");

    int h = 40;
    ui->start->setFixedHeight(h);
    ui->pause->setFixedHeight(h);
    ui->over->setFixedHeight(h);


}

void GameWidget::initMainScene()
{
    mainScene=new MainScene(this);
    mainScene->setGeometry(10,10,MWidth,MHeight);
}

void GameWidget::initListview()
{
    QString data;
    data.append("目前贪吃蛇1.1稳定版");

    QStandardItemModel* model = new QStandardItemModel(this);

    foreach (const QString& text, data) {
        QStandardItem *item=new QStandardItem(text);
        model->appendRow(item);
    }
    //设置模型
    ui->listView->setModel(model);
}
