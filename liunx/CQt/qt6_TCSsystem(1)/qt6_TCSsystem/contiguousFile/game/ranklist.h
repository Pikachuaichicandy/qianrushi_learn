#ifndef RANKLIST_H
#define RANKLIST_H

#include <QDialog>
#include <QFont>
#include <QSize>


namespace Ui {
class ranklist;
}

enum Mode{
    mode_common,
    mode_hard,
};

class ranklist : public QDialog
{
    Q_OBJECT

public:
    explicit ranklist(QWidget *parent = nullptr);
    ~ranklist();
    void addItems(int mode);

private slots:
    void on_pushButton_clicked();

private:
    Ui::ranklist *ui;
    QFont font;
    QSize size;
};

#endif // RANKLIST_H
