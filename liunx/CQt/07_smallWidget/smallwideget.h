#ifndef SMALLWIDEGET_H
#define SMALLWIDEGET_H

#include <QWidget>

namespace Ui {
class smallWideget;
}

class smallWideget : public QWidget
{
    Q_OBJECT

public:
    explicit smallWideget(QWidget *parent = nullptr);
    ~smallWideget();

    void setNum(int num);  // 设置数字的函数声明
        int getNum();          // 获取数字的函数声明
private:
    Ui::smallWideget *ui;
};

#endif // SMALLWIDEGET_H
