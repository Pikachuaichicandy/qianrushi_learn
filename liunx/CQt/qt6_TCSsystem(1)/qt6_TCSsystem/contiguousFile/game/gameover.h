#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <QDialog>
#include <QString>

namespace Ui {
class gameover;
}

class gameover : public QDialog
{
    Q_OBJECT

public:
    explicit gameover(QWidget *parent = nullptr);
    ~gameover();
    void setText(QString str1 ,QString str2);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::gameover *ui;
};

#endif // GAMEOVER_H
