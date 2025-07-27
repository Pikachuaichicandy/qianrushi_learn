#include "snake.h"
#include<QSettings>
#include<QDebug>

Snake::Snake(QObject *parent)
    : QObject{parent}
{
    init();
}

void Snake::init()
{
    m_lastDir=moveLeft;
    m_snake.clear();
    setSkin();
}

int Snake::isEatFood(Food& food)
{
    if(food.x==m_snake.first().x&&food.y==m_snake.first().y)
    {
        //不同类别加分不同
        switch (food.type) {
        case foodType::apple:

            return 10;
        case foodType::banana:

            return 20;
        case foodType::drug:

            return -30;
        }
    }
    return 0;
}

int Snake::isAlEatFood(Food &food,int x,int y)
{
    if(food.x==x&&food.y==y)
    {
        //不同类别加分不同
        if (food.type==foodType::apple || food.type==foodType::banana)
        {
            return 1;
        }
        else
            return -1;
    }
    return 0;
}

bool Snake::isCrossBorder()
{
    if(m_snake[0].x < 0 || m_snake[0].x >= B_W_Count || m_snake[0].y < 0 || m_snake[0].y >= B_H_Count)
    {
        return true;
    }
    return false;
}

bool Snake::isCrossSelf()
{
    int x=m_snake[0].x,y=m_snake[0].y;
    for(int i=1;i<m_snake.size();i++)
    {
        if(x==m_snake[i].x&&y==m_snake[i].y)
            return true;
    }
    return false;
}

void Snake::move(moveDir dir)
{
    //每一个蛇身移动到它的前一位(这里应该从大到小去移动，不然会有bug)
    for(int i=m_snake.size()-1;i>0;i--)
    {
        m_snake[i].x=m_snake[i-1].x;
        m_snake[i].y=m_snake[i-1].y;
    }

    if(m_lastDir!=dir)   //防止重复输入造成的图片资源消耗
    {
        m_snake[0].pix=head[int(dir)];
        m_lastDir=dir;
    }

    //改变蛇头
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
    //qDebug()<<m_snake.size();
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
        x = m_snake.last().x-1,y=m_snake.last().y;
        break;
    }
    m_snake.push_back({x,y,body});
}

void Snake::setSkin()
{
    QSettings set("D:/Qtpro/cmake/snake/config/skin.ini", QSettings::IniFormat);
    m_head=set.value("head").toInt();
    m_body=set.value("body").toInt();

    //初始化图片
    head[0]=QPixmap(QString(":/images/snake/head/head%1/headTop.png").arg(m_head)).scaled(BWidth,BWidth);
    head[1]=QPixmap(QString(":/images/snake/head/head%1/headDown.png").arg(m_head)).scaled(BWidth,BWidth);
    head[2]=QPixmap(QString(":/images/snake/head/head%1/headLeft.png").arg(m_head)).scaled(BWidth,BWidth);
    head[3]=QPixmap(QString(":/images/snake/head/head%1/headRight.png").arg(m_head)).scaled(BWidth,BWidth);
    //目前暂定一条皮肤
    body=QPixmap(QString(":/images/snake/body/body%1.png").arg(m_body)).scaled(BWidth,BWidth);
    m_snake.push_back({B_W_Count/2,B_H_Count/2,head[2]});
    m_snake.push_back({B_W_Count/2+1,B_H_Count/2,body});
}
