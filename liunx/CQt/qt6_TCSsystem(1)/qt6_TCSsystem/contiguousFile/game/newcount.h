#ifndef NEWCOUNT_H
#define NEWCOUNT_H

#include <QWidget>

namespace Ui {
class newcount;
}

class newcount : public QWidget
{
    Q_OBJECT

public:
    explicit newcount(QWidget *parent = nullptr);
    ~newcount();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::newcount *ui;
};

#endif // NEWCOUNT_H
