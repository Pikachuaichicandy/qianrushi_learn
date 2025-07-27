#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void timerEvent(QTimerEvent *);
    int id1; //定时器标志1
    int id2; //定时器标志2
    ~Widget();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
