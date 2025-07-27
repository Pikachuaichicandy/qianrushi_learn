#ifndef HEROMANAGERWIDGET_H
#define HEROMANAGERWIDGET_H

#include <QWidget>
#include "sqlite.h"
#include "page_login.h"
#include "dlg_addhero.h"
QT_BEGIN_NAMESPACE
namespace Ui { class HeroManagerWidget; }
QT_END_NAMESPACE

class HeroManagerWidget : public QWidget
{
    Q_OBJECT

public:
    HeroManagerWidget(QWidget *parent = nullptr);
    ~HeroManagerWidget();

private slots:
    void on_btn_exit_clicked();

    void on_btn_simulation_clicked();


    void on_btn_add_clicked();


    void on_btn_del_clicked();

    void on_btn_updata_clicked();


    void on_btn_search_clicked();

private:
    Ui::HeroManagerWidget *ui;
    Page_login m_dlglogin;

    sqlite *m_ptrsqlite;
    Dlg_AddHero m_dlgAddHero;

    void updateTable();
};
#endif // HEROMANAGERWIDGET_H
