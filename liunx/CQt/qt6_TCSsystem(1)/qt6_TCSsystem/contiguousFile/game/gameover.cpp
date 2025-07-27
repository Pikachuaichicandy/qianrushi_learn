#include "gameover.h"
#include "ui_gameover.h"

gameover::gameover(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gameover)
{
    ui->setupUi(this);
}

gameover::~gameover()
{
    delete ui;
}

void gameover::setText(QString str1, QString str2)
{
    ui->label->setText(str1);
    ui->label_2->setText(str2);
}

void gameover::on_pushButton_clicked()
{
    QCoreApplication::exit();
}

void gameover::on_pushButton_2_clicked()
{
    this->close();
}
