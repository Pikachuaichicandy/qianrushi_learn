#ifndef DLG_ADDHERO_H
#define DLG_ADDHERO_H

#include <QDialog>
#include "sqlite.h"
namespace Ui {
class Dlg_AddHero;
}

class Dlg_AddHero : public QDialog
{
    Q_OBJECT

public:
    explicit Dlg_AddHero(QWidget *parent = nullptr);
    ~Dlg_AddHero();

    void setType(bool isAdd, HeroInfo info={ });
private slots:
    void on_btn_save_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::Dlg_AddHero *ui;
    bool m_isAdd;
    HeroInfo m_info;
};

#endif // DLG_ADDHERO_H
