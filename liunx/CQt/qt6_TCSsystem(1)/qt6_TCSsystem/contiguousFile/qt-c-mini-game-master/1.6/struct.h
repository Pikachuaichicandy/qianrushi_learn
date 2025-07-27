#ifndef STRUCT_H
#define STRUCT_H
#include<QPixmap>
#include"config.h"

enum moveDir{    //移动方向
    moveTop,
    moveDown,
    moveLeft,
    moveRight
};

enum gameStatus{   //游戏状态
    gameInit,
    gameStart,
    gameShiftPattern,
    gameOver
};

enum foodType{
    apple,
    banana,
    drug     //扣分的毒药
};

struct Point
{
    int x,y;
    Point(int a,int b):x(a),y(b) {}
};

struct Block
{
    Block()
    {
        x=y=-1;
        pix=QPixmap();
    }
    Block(int a,int b):x(a),y(b){}     //方便创建食物的时候使用
    Block(int a,int b,QPixmap p)
    {
        x=a;y=b;pix=p;
    }
    int x,y;
    QPixmap pix;
};

struct snakeBody:Block
{
    snakeBody(int i,int j,QPixmap b):Block(i,j,b){}
    Point getPosition(){   //方便在算法中节约内存
        return Point(x,y);
    }
};

struct Food : Block
{
    foodType type;
    Food():Block(){}   //获得初始化
    Food(int a,int b,QPixmap p,foodType t):Block(a,b,p),type(t){}
    Food(int a,int b):Block(a,b){}
    bool operator ==(Food& drug)
    {
        if((x==drug.x&&y-1==drug.y) || (x==drug.x&&y+1==drug.y) ||
            (x-1==drug.x&&y==drug.y) || (x+1==drug.x&&y==drug.y))
            return true;
        return false;
    }
    bool operator ==(Point& point)
    {
        if((x==point.x&&y-1==point.y) || (x==point.x&&y+1==point.y) ||
            (x-1==point.x&&y==point.y) || (x+1==point.x&&y==point.y))
            return true;
        return false;
    }
};

#endif // STRUCT_H
