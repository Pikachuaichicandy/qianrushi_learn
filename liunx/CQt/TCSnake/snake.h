#ifndef SNAKE_H
#define SNAKE_H

#include "QObject"
#include "QPixmap"
#include "QVector"
#include "config.h"
#include "struct.h"

class Snake :public QObject
{
    Q_OBJECT
public:
    explicit Snake(QObject *parent = nullptr);

    void init();

    int isEatFood(Food& food); //是否吃到食物

    bool isCrossBorder(); //是否越过边界

    bool isCrossSelf(); //是否碰到自己

    void move(moveDir dir); //移动

    void addBody(moveDir dir); //跟据方向调整添加的尾部

    QVector<snakeBody> m_snake; //蛇的全部

signals:

private:

    QPixmap body;


};

#endif // SNAKE_H
