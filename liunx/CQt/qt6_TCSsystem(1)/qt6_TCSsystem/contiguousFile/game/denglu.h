#ifndef DENGLU_H
#define DENGLU_H

#include <QWidget>
#include <QFile>
#include <QString>
#include "dialog.h"
#include "zhuce.h"


namespace Ui {
class denglu;
}

class denglu : public QWidget
{
    Q_OBJECT
public:
    bool success;
    QFile file;

public:
    explicit denglu(QWidget *parent = nullptr);
    ~denglu();
    void dialogShow(QString str);

private slots:
    void on_pushButton_clicked();//历史输出记录，功能还没做

    void on_pushButton_3_clicked();//切换密码可视性

    void on_pushButton_2_clicked();//登录

    void on_pushButton_4_clicked();//注册

private:
    Ui::denglu *ui;
    ZhuCe* zhuceWidget;//注册的对话窗口

};

#endif // DENGLU_H
