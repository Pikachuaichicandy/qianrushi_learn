#ifndef CD_WORK_H
#define CD_WORK_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QMap>
#include <QLabel>
#include "projectstruct.h"
#include "sqldatasolution.h"
//#include <mainwindow.h>

namespace Ui {
class cd_work;
}

class cd_work : public QWidget
{
    Q_OBJECT

public:
    explicit cd_work(QWidget *parent = nullptr);
    ~cd_work();

private slots:
    void on_exit_clicked();

    void on_query_word_clicked();
public:
    void searchword(MSGInfo *msg);
private:
    Ui::cd_work *ui;
};

#endif // CD_WORK_H
