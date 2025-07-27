#include "heromanagerwidget.h"
#include "ui_heromanagerwidget.h"
#include <QTreeWidget>
#include <QString>
#include <QMessageBox>

HeroManagerWidget::HeroManagerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HeroManagerWidget)
    ,m_ptrsqlite(nullptr)
{
    ui->setupUi(this);

    m_dlglogin.show();
//    this->hide();

//    auto f=[&](){
//        this->show();
//    };
//    connect(&m_dlglogin,&Page_login::sendLoginSuccess,this,f);

    connect(&m_dlglogin, &Page_login::sendLoginSuccess, this, [this]() {
        this->show();  // 登录成功后显示当前界面
    });

    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(1);

//    QStringList l;
//    l<<"王者荣耀信息管理系统";
//    QTreeWidget *pf = new QTreeWidgetItem(ui->treeWidget,l);
//    ui->treeWidget->addTopLevelItem(pf);

//    l.clear();
//    l<<"用户管理";
//    QTreeWidget *p1 = new QTreeWidgetItem(ui->treeWidget,l);
//    ui->treeWidget->addTopLevelItem(p1);

//    l.clear();
//    l<<"管理员管理";
//    QTreeWidget *p2 = new QTreeWidgetItem(ui->treeWidget,l);
//    ui->treeWidget->addTopLevelItem(p2);

//    pf->addChild(p1);
//    pf->addChild(p2);



    QStringList l;
    l << "王者荣耀信息管理系统";
    QTreeWidgetItem *pf = new QTreeWidgetItem(ui->treeWidget, l);
    ui->treeWidget->addTopLevelItem(pf);

    l.clear();
    l << "用户管理";
    QTreeWidgetItem *p1 = new QTreeWidgetItem(ui->treeWidget, l);
    ui->treeWidget->addTopLevelItem(p1);

    l.clear();
    l << "管理员管理";
    QTreeWidgetItem *p2 = new QTreeWidgetItem(ui->treeWidget, l);
    ui->treeWidget->addTopLevelItem(p2);

    pf->addChild(p1);  // 这里添加子项
    pf->addChild(p2);

    ui->treeWidget->expandAll();
    ui->stackedWidget->setCurrentIndex(0);

    m_ptrsqlite = sqlite::getinstance();
//    m_ptrsqlite->init();

}

HeroManagerWidget::~HeroManagerWidget()
{
    delete ui;
}


void HeroManagerWidget::on_btn_exit_clicked()
{
    exit(0);
}


void HeroManagerWidget::on_btn_simulation_clicked()
{
    HeroInfo info;

    // 制作1个英雄数据
    for(int i = 0; i < 1; i++)
    {
        info.name = QString("夏%1").arg(i);
        info.career = QString("战士");
        info.attribute = QString("物理");
        info.skill = QString("七星斩");
    }

    m_ptrsqlite->addHero(info);
//    updateTable();

}




void HeroManagerWidget::on_btn_add_clicked()
{
    m_dlgAddHero.setType(true);
    m_dlgAddHero.exec();
    updateTable();
}

void HeroManagerWidget::updateTable()
{
    //重写表头
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(5);
    QStringList l;
    l<<"序号"<<"英雄"<<"职业"<<"属性"<<"技能";
    //只选中行
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableWidget->setSelectionModel(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setHorizontalHeaderLabels(l);

    auto uicount = m_ptrsqlite->getHeroCount();
    QList<HeroInfo> lHeros = m_ptrsqlite->getHeroesByPage(0,uicount);

    ui->tableWidget->setRowCount(uicount);
    for (int i=0; i<lHeros.size();i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(i)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(lHeros[i].id)));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(lHeros[i].name));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(lHeros[i].career));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(lHeros[i].attribute));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(lHeros[i].skill));
    }

}

void HeroManagerWidget::on_btn_del_clicked()
{
    int i = ui->tableWidget->currentRow();
    if(i>=0)
    {
        int id = ui->tableWidget->item(i,1)->text().toUInt();
        m_ptrsqlite->deleteHero(id);
        updateTable();
        QMessageBox::information(nullptr,"信息","删除成功");
    }

}

void HeroManagerWidget::on_btn_updata_clicked()
{
    HeroInfo info;
    int i = ui->tableWidget->currentRow();
    if(i>=0)
    {
        info.id = ui->tableWidget->item(i,0)->text().toUInt();
        info.name = ui->tableWidget->item(i,1)->text();
        info.career = ui->tableWidget->item(i,2)->text();
        info.attribute = ui->tableWidget->item(i,3)->text();
        info.skill = ui->tableWidget->item(i,4)->text();

        m_dlgAddHero.setType(false,info);
        m_dlgAddHero.exec();
    }
    updateTable();
}


void HeroManagerWidget::on_btn_search_clicked()
{
    QString strFilter = ui->le_search->text();
    if(strFilter.isEmpty())
    {
        QMessageBox::information(nullptr,"名字","筛选为空");
        updateTable();
        return;
    }

    //重写表头
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(5);
    QStringList l;
    l<<"序号"<<"英雄"<<"职业"<<"属性"<<"技能";
    //只选中行
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableWidget->setSelectionModel(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setHorizontalHeaderLabels(l);

    auto uicount = m_ptrsqlite->getHeroCount();
    QList<HeroInfo> lHeros = m_ptrsqlite->getHeroesByPage(0,uicount);


    ui->tableWidget->setRowCount(uicount);
    int index = 0;

    for (int i=0; i<lHeros.size();i++)
    {
        if(!lHeros[i].name.contains(strFilter))
        {
            continue;
        }
        index++;
        ui->tableWidget->setItem(index,0,new QTableWidgetItem(QString::number(index+1)));
        ui->tableWidget->setItem(index,1,new QTableWidgetItem(QString::number(lHeros[i].id)));
        ui->tableWidget->setItem(index,2,new QTableWidgetItem(lHeros[i].name));
        ui->tableWidget->setItem(index,3,new QTableWidgetItem(lHeros[i].career));
        ui->tableWidget->setItem(index,4,new QTableWidgetItem(lHeros[i].attribute));
        ui->tableWidget->setItem(index,5,new QTableWidgetItem(lHeros[i].skill));

    }
}
