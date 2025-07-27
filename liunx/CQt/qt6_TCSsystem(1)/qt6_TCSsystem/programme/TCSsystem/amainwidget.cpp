#include "amainwidget.h"
#include "ui_amainwidget.h"

amainWidget::amainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::amainWidget)
{
    ui->setupUi(this);
}

amainWidget::~amainWidget()
{
    delete ui;
}
