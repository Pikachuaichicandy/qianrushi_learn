#include "start.h"
#include "ui_start.h"

start::start(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::start)
{
    ui->setupUi(this);
    this->resize(1200,800);
    //this->setLayout(ui->verticalLayout_2);
}

start::~start()
{
    delete ui;
}

void start::on_pushButton_clicked()
{
    mode=GameDifficulty::common;
}
