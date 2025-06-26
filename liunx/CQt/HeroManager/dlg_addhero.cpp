#include "dlg_addhero.h"
#include "ui_dlg_addhero.h"

#include <QMessageBox>

Dlg_AddHero::Dlg_AddHero(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dlg_AddHero)
{
    ui->setupUi(this);
}

Dlg_AddHero::~Dlg_AddHero()
{
    delete ui;
}

void Dlg_AddHero::on_btn_save_clicked()
{
    HeroInfo info;
    auto ptr = sqlite::getinstance();

    info.id=m_info.id;
    info.name = ui->le_name->text();
    info.career = ui->le_career->text();
    info.attribute = ui->le_attribute->text();
    info.skill = ui->le_skill->text();
    ptr->addHero(info);

    if(m_isAdd)
    {
        ptr->addHero(info);
    }
    else
    {
        ptr->updateHeroInfo(info);
    }

    QMessageBox::information(nullptr,"信息","存储成功");
    this->hide();

}

void Dlg_AddHero::on_btn_cancel_clicked()
{
    this->hide();
}

void Dlg_AddHero::setType(bool isAdd,HeroInfo info)
{
    m_isAdd = isAdd;
    m_info = info;

    ui->le_name->setText(info.name);
    ui->le_career->setText(info.career);
    ui->le_attribute->setText(info.attribute);
    ui->le_skill->setText(info.skill);

}
