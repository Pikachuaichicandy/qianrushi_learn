#ifndef MYLABEL_H
#define MYLABEL_H

#include <QWidget>
//#include <QLabel>

class myLabel : public QWidget
{
    Q_OBJECT
public:
    explicit myLabel(QWidget *parent = nullptr);
   //鼠标进入事件
    void enterEvent(QEvent *event);
    //鼠标离开事件
    void leaveEvent(QEvent *);

signals:

};

#endif // MYLABEL_H
