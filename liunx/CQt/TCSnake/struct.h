#ifndef STRUCT_H
#define STRUCT_H

#include <QPixmap>

enum moveDir{
    //移动方向
    moveTop,
    moveDown,
    moveLeft,
    moveRight
};

enum gameStatus{
    //游戏状态
    gameInit,
    gameStart,
    gameOver
};

enum foodType{
    apple,
    banana,
    drug //扣分毒药
};

struct Block
{
    Block()
    {
        x=y=-1;
        pix=QPixmap();
    }
    Block(int a, int b):x(a),y(b){} //方便创建实物的时候使用
    Block(int a, int b,QPixmap p)
    {
        x=a;y=b;pix=p;

    }
    int x,y;
    QPixmap pix;
};

struct snakeBody:Block
{
    snakeBody(int i,int j,QPixmap b):Block(i,j,b){}

};

struct Food : Block
{
    foodType type;
    Food():Block(){} //获得初始化
    Food(int a,int b,QPixmap p,foodType t):Block(a,b,p),type(t){}
    Food(int a,int b):Block(a,b){}
};

#endif // STRUCT_H
