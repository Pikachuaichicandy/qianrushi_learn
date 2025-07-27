#ifndef MONEYUP_H
#define MONEYUP_H

#include <QWidget>

namespace Ui {
class moneyUp;
}

class moneyUp : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int money READ money WRITE updataMoney)

public:
    explicit moneyUp(QWidget *parent = nullptr);
    ~moneyUp();
    int money() const;
    void updataMoney(int value);
    void run();

private:
    Ui::moneyUp *ui;
    int ownMoney;
};

#endif // MONEYUP_H
