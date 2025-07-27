#ifndef START_H
#define START_H

#include <QWidget>
#include "snake.h"

namespace Ui {
class start;
}

class start : public QWidget
{
    Q_OBJECT

public:
    explicit start(QWidget *parent = nullptr);
    ~start();
    int getMode();

private slots:
    void on_pushButton_clicked();

private:
    Ui::start *ui;
    int mode;
};

#endif // START_H
