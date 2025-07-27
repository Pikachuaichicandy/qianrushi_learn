#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>
#include<QLabel>
#include<QPixmap>

class UserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserWidget(int w,int h,QWidget *parent = nullptr);
    ~UserWidget();

    void setVariable(QString name,int score,QPixmap& head,QPixmap& body);
    void setFontColor();
    void setScore(int score);

signals:

private:
    QLabel* m_userName;
    QLabel* m_score;
    QLabel* m_head;
    QLabel* m_body;
};

#endif // USERWIDGET_H
