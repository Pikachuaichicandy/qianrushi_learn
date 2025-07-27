#include "moneyup.h"
#include "ui_moneyup.h"
#include <QString>
#include <QPropertyAnimation>

moneyUp::moneyUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::moneyUp)
{
    ui->setupUi(this);
    run();
}

moneyUp::~moneyUp()
{
    delete ui;
}

int moneyUp::money() const
{
    return ownMoney;
}

void moneyUp::updataMoney(int value)
{
    ownMoney=value;
    QString str = "$"+QString::number(ownMoney);
    ui->label->setText(str);
}

void moneyUp::run()
{
    //通过动画更新money属性宏的值,会自动调用"money"的updataMoney(int value)
    QPropertyAnimation* carton=new QPropertyAnimation(this, "money", this);
    carton->setStartValue(100);
    carton->setEndValue(100000000);
    carton->setDuration(6000);
    //变化曲线
    carton->setEasingCurve(QEasingCurve::InCubic);
    //循环播放
    carton->setLoopCount(-1);
    carton->start();
}
