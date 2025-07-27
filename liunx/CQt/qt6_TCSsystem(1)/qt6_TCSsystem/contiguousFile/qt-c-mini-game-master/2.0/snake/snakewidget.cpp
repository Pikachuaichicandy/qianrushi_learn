#include "snakewidget.h"
#include<QDebug>
#include <QPainter>

SnakeWidget::SnakeWidget(QWidget *parent)
    : QWidget{parent}
{
    m_timer=new QTimer(this);
    m_timer->setInterval(timerNum);
    connect(m_timer,&QTimer::timeout,this,&SnakeWidget::timerMove);

    setFixedSize(MWidth+10,MHeight+70);

    init();
}

SnakeWidget::~SnakeWidget()
{
    qDebug()<<"snakeWidget析构";
}

void SnakeWidget::init()
{
    //先初始化数据
    keyFlag=forbidKey=false;
    m_LastDir=2;
    for(int i=0;i<userNum;i++)
    {
        QVector<snakeBody> v;
        QVector<QPixmap> p;
        userScore.push_back(0);
        userDir.push_back(2);    //0 1 2 3
        snake.push_back(v);
        username.push_back("");
        bodySkin.push_back(QPixmap());
        headSkin.push_back(p);
    }
    qDebug()<<"vector大小："<<snake.size();
    m_timer->start();
}

void SnakeWidget::recvData(bool isSelf,int posi,Point point,Point food,QString name,int head,int body)
{
    qDebug()<<"接收到自己的数据：posi: "<<posi<<"--"<<name;
    //自己的数据
    if(isSelf)
    {
        m_position=posi;
        m_food.setFood(food);
        m_food.pix=QPixmap(":/images/food/apple.png");
        qDebug()<<"食物坐标："<<m_food.x<<"  "<<m_food.y;
    }
    //设置数据
    QPixmap headLeftPix=setSkin(posi,head,body);
    snake[posi].push_back(snakeBody(point,headLeftPix));
    username[posi]=name;
}

void SnakeWidget::recvDir(int posi, int dir)
{
    qDebug()<<"方向改变："<<posi<<"   "<<dir;
    userDir[posi]=dir;
}

void SnakeWidget::recvUserDeath(int posi)
{
    qDebug()<<"玩家死亡： "<<posi;
    snake[posi].clear();
}

void SnakeWidget::gameOver()
{
    m_timer->stop();
    qDebug()<<"游戏结束";
    emit returnLogin();
}

void SnakeWidget::keyPressEvent(QKeyEvent *event)
{
    if(keyFlag || forbidKey)
        return;
    int dir=userDir[m_position];
    //调整蛇头方向也可以直接换图片，如果旋转的话很短时间内输入两个键可能容易出现转向bug
    if(event->key()==Qt::Key_W || event->key()==Qt::Key_Up)
    {
        if(dir!=1&&dir!=0)
            dir=0;
    }
    else if(event->key()==Qt::Key_S || event->key()==Qt::Key_Down)
    {
        if(dir!=0&&dir!=1)
            dir=1;
    }
    else if(event->key()==Qt::Key_A || event->key()==Qt::Key_Left)
    {
        if(dir!=3&&dir!=2)
            dir=2;
    }
    else if(event->key()==Qt::Key_D || event->key()==Qt::Key_Right)
    {
        if(dir!=2&&dir!=3)
            dir=3;
    }
    if(m_LastDir!=dir)
    {
        //m_LastDir=dir;
        emit changeDir(1,dir,m_position);
    }
    keyFlag=true;
}

void SnakeWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //画蛇
    paintSnake(&painter);
    paintFood(&painter);
    //画边框
    paintBorder(&painter);
}

QPixmap SnakeWidget::setSkin(int posi, int headPix, int bodyPix)
{
    //全部先加载保存
    //上下左右
    QStringList pathList;
    pathList<<QString(":/images/snake/head/head%1/headTop.png").arg(headPix)
             <<QString(":/images/snake/head/head%1/headDown.png").arg(headPix)
             <<QString(":/images/snake/head/head%1/headLeft.png").arg(headPix)
             <<QString(":/images/snake/head/head%1/headRight.png").arg(headPix);
    for(int i=0;i<4;i++)
        headSkin[posi].push_back(QPixmap(pathList[i]).scaled(BWidth,BWidth));

    bodySkin[posi]=QPixmap(QString(":/images/snake/body/body%1.png").arg(bodyPix)).scaled(BWidth,BWidth);

    //返回朝向左边的
    return headSkin[posi][2];
}

void SnakeWidget::paintBorder(QPainter *painter)
{
    painter->setPen(Qt::red);

    painter->drawLine(2,2,MWidth+2,2);
    painter->drawLine(MWidth+2,2,MWidth+2,MHeight+2);
    painter->drawLine(MWidth+2,MHeight+2,2,MHeight+2);
    painter->drawLine(2,MHeight+2,2,2);
}

void SnakeWidget::paintFood(QPainter *painter)
{
    painter->drawPixmap(m_food.x*BWidth,m_food.y*BWidth,m_food.pix);
}

void SnakeWidget::paintSnake(QPainter *painter)
{
    for(int i=0;i<userNum;i++)
    {
        if(snake[i].size()==0)
            continue;
        for(int j=0;j<snake[i].size();j++)
        {
            painter->drawPixmap(snake[i][j].x*BWidth,snake[i][j].y*BWidth,snake[i][j].pix);
        }
    }
}

bool SnakeWidget::isCrossSnake()
{
    int x=snake[m_position][0].x,y=snake[m_position][0].y;
    for(int i=0;i<userNum;i++)
    {
        int j=0;
        if(i==m_position)   //自己的不用判断蛇头
            j=1;
        for(;j<snake[i].size();j++)
        {
            if(x==snake[i][j].x&&y==snake[i][j].y)
                return true;
        }
    }
    return false;
}

bool SnakeWidget::isCrossBorder()
{
    if(snake[m_position][0].x < 0 || snake[m_position][0].x >= B_W_Count ||
        snake[m_position][0].y < 0 || snake[m_position][0].y >= B_H_Count)
    {
        return true;
    }
    return false;
}

void SnakeWidget::isEatFood(Food& food)
{
    for(int i=0;i<userNum;i++)
    {
        if(snake[i].size()==0)
            continue;
        if(snake[i][0].x==food.x&&snake[i][0].y==food.y)
        {
            food.x=-1,food.y=-1;
            userScore[i]++;   //分数加1
            addBody(i);
            break;
        }
    }
}

void SnakeWidget::move(int posi)
{
    //每一个蛇身移动到它的前一位(这里应该从大到小去移动，不然会有bug)
    for(int i=snake[posi].size()-1;i>0;i--)
    {
        snake[posi][i].x=snake[posi][i-1].x;
        snake[posi][i].y=snake[posi][i-1].y;
    }

    int dir=userDir[posi];
    if(m_LastDir!=dir)   //防止重复输入造成的图片资源消耗
    {
        snake[posi][0].pix=headSkin[posi][dir];
        m_LastDir=dir;
    }

    //改变蛇头
    switch (dir) {
    case 0:
        snake[posi][0].y-=1;
        break;
    case 1:
        snake[posi][0].y+=1;
        break;
    case 2:
        snake[posi][0].x-=1;
        break;
    case 3:
        snake[posi][0].x+=1;
        break;
    }
}

void SnakeWidget::addBody(int posi)
{
    int x,y,dir=userDir[posi];
    switch (dir) {
    case 0:
        x = snake[posi].last().x,y=snake[posi].last().y+1;
        break;
    case 1:
        x = snake[posi].last().x,y=snake[posi].last().y-1;
        break;
    case 2:
        x = snake[posi].last().x+1,y=snake[posi].last().y;
        break;
    case 3:
        x = snake[posi].last().x-1,y=snake[posi].last().y;
        break;
    }
    snake[posi].push_back(snakeBody(x,y,bodySkin[posi]));
}

void SnakeWidget::timerMove()
{
    keyFlag=false;
    for(int i=0;i<userNum;i++)
    {
        if(snake[i].size()==0)
            continue;
        move(i);
    }
    isEatFood(m_food);
    if(!forbidKey)
    {
        if(isCrossSnake() || isCrossBorder())
        {
            //发送服务器错误信号
            snake[m_position].clear();
            forbidKey=true;
            emit sendDeath(7,-1,m_position);
        }
    }
    update();
}
