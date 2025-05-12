#include "snake.h"
#include <QDebug>


Snake::Snake(QObject *parent):QObject{parent}
{
    init();
}

void Snake::init()
{
    m_snake.clear();
    //目前暂定一条皮肤
    body = QPixmap(":/images/snake/common/body1.png").scaled(BWidth,BWidth);
    m_snake.push_back({B_W_Count/2,B_H_Count/2,QPixmap(":/images/snake/common/headLeft.pgn").scaled(BWidth,BWidth)});
    m_snake.push_back({B_W_Count/2+1,B_H_Count/2,body});
}

int Snake::isEatFood(Food &food)
{
    if(food.x==m_snake.first().x&&food.y==m_snake.first().y)
    {
        switch (food.type) {
        case foodType::apple:
            return 10;
        case foodType::banana:
            return 20;
        case foodType::banana:
            return -30;

        }
    }
    return 0;

}

bool Snake::isCrossBorder()
{
    if(m_snake[0].x<0||m_snake[0].x>=B_W_Count||m_snake[0].y<0||m_snake>=B_H_Count)
    {
        return true;
    }
    return false;
}

bool Snake::isCrossSelf()
{
    int x=m_snake[0].x,y=m_snake[0].y;
    for (int i=1; i<m_snake.size();i++)
    {
        if(x==m_snake[i].x&&y==m_snake[i].y)
            return true;
    }
}


void Snake::move(moveDir dir)
{
    //每一个蛇身移动到它的前一位（这里应该从大到小去移动）
    for(int i=m_snake.size()-1;i>0;i--)
    {
        m_snake[i].x=m_snake[i-1].x;
        m_snake[i].y=m_snake[i-1].y;
    }

    //改变移动方向
    switch (dir) {
    case moveDir::moveTop:
        m_snake[0].y-=1;
         break;
    case moveDir::moveDown:
        m_snake[0].y+=1;
         break;
    case moveDir::moveLeft:
        m_snake[0].x-=1;
         break;
    case moveDir::moveRight:
        m_snake[0].x+=1;
        break;
    }
}

void Snake::addBody(moveDir dir)
{
//    qDebug<<m_snake.size();
    int x,y;
    switch (dir) {
    case moveDir::moveTop:
        x = m_snake.last().x,y=m_snake.last().y+1;
        break;
    case moveDir::moveDown:
        x = m_snake.last().x,y=m_snake.last().y-1;
        break;
    case moveDir::moveLeft:
        x = m_snake.last().x+1,y=m_snake.last().y;
        break;
    case moveDir::moveRight:
        x = m_snake.last().x-1,y=m_snake.last().y+1;
        break;

    default:
        break;
    }
}


