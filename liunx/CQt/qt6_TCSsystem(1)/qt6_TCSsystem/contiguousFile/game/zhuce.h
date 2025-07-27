#ifndef ZHUCE_H
#define ZHUCE_H

#include <QDialog>

namespace Ui {
class ZhuCe;
}

class ZhuCe : public QDialog
{
    Q_OBJECT

public:
    explicit ZhuCe(QWidget *parent = nullptr);
    ~ZhuCe();
    void dialogShow(QString str);

private slots:
    void on_pushButton_2_clicked();//返回按钮

    void on_pushButton_clicked();//注册按钮

private:
    Ui::ZhuCe *ui;
};

#endif // ZHUCE_H
