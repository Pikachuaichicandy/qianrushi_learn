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

    connect(initMainScene,&MainScene::updateScore,this,&GameWidget::updateLcdNum);
    connect(ui->start,&QPushButton::clicked,mainScene,&MainScene::start);
    connect(ui->over,&QPushButton::clicked,mainScene,&MainScene::gameOver);
}

GameWidget::~GameWidget()
{
    delete ui;
}

