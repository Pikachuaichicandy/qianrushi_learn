#ifndef SNAKE_H
#define SNAKE_H

#include <QObject>
#include"struct.h"
#include"config.h"
#include<QVector>
#include<QPixmap>

class Snake : public QObject
{
    Q_OBJECT
public:
    explicit Snake(QObject *parent = nullptr);

    void init();

    int isEatFood(Food& food);    //是否吃到食物

    int isAlEatFood(Food& food,int x,int y);   //用于Al判断

    bool isCrossBorder();     //是否越过边界

    bool isCrossSelf();      //是否碰到自己

    void move(moveDir dir);   //移动

    void addBody(moveDir dir);  //根据方向调整添加的尾部

    QVector<snakeBody> m_snake;   //蛇的全部

signals:


private:
    void setSkin();

    moveDir m_lastDir;
    int m_head,m_body;
    QPixmap body;
    QPixmap head[4];
};

#endif // SNAKE_H
