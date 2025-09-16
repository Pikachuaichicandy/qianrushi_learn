#include "cd_server.h"
#include "ui_cd_server.h"

cd_server::cd_server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cd_server)
{
    ui->setupUi(this);
}

cd_server::~cd_server()
{
    delete ui;
}
