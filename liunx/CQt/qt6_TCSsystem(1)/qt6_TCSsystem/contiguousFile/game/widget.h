#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include "snake.h"
#include "movetext.h"
#include "moneyup.h"
#include "denglu.h"
#include "ranklist.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

enum GameDifficulty{
    common,
    hard,
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void deleteStart();
    void GameWidgetShow();
    void GameWidgetClose();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void onTimeOut();

private:
    Ui::Widget *ui;
    QTimer* timer;//定时器
    int mode;//游戏模式
    denglu* dengluWidget;
    snake* snakeWidget;
    movetext* movetextWidget;
    moneyUp*  moneyUpWidget;
    ranklist* ranklistWidget;

};
#endif // WIDGET_H
