#include "mainscene.h"
#include"config.h"
#include <QFontMetrics>
#include <QPainter>
#include<QDebug>

MainScene::MainScene(QWidget *parent)
    : QWidget{parent}
{
    m_timer=new QTimer(this);
    m_timer->setInterval(timerNum);
    init();
    connect(m_timer,&QTimer::timeout,this,&MainScene::timerMove);
}

void MainScene::start()
{
    init();
    setFocus();   //获得聚焦
    m_status=gameStart;
    m_timer->start();
}

void MainScene::pause()
{
    if(m_status!=gameStart)
        return;
    if(m_timer->isActive())
        m_timer->stop();
    else
    {
        m_timer->start();
        setFocus();
    }
}

void MainScene::gameover()
{
    m_status=gameOver;
    m_timer->stop();
    update();    //这里要手动更新
    //这里可以加一些listview的记录
}

void MainScene::painBorder(QPainter *painter)
{
    painter->setPen(Qt::red);

    painter->drawLine(0,0,MWidth-1,0);
    painter->drawLine(MWidth-1,0,MWidth-1,MHeight-1);
    painter->drawLine(MWidth-1,MHeight-1,0,MHeight-1);
    painter->drawLine(0,MHeight-1,0,0);
}

void MainScene::paintSnakeBody(QPainter *painter)
{
    for(int i=0;i<m_snake.m_snake.size();i++)
    {
        painter->drawPixmap(m_snake.m_snake[i].x*BWidth,m_snake.m_snake[i].y*BWidth,m_snake.m_snake[i].pix);
    }
}

void MainScene::paintFood(QPainter *painter)
{
    for(int i=0;i<totalNum;i++)
    {
        painter->drawPixmap(totalFood[i].x*BWidth,totalFood[i].y*BWidth,totalFood[i].pix);
    }
}

void MainScene::paintGameInit(QPainter *painter)
{
    QString str="苹果（10分），香蕉（20分），毒药/障碍物（-30分） 分数小于0时死亡/失败！";
    paintCenterText(painter,str);
}

void MainScene::paintGameOver(QPainter *painter)
{
    QString str=QString("游戏结束，最终分数为：%1").arg(m_score);
    paintCenterText(painter,str);
}

void MainScene::paintCenterText(QPainter *painter, QString &str)
{
    QFont font;
    font.setPixelSize(BWidth);
    painter->setFont(font);
    //QFontMetrics提供了在给定字体和字号下测量文本的方法。
    QFontMetrics me(font);
    painter->drawText((MWidth/2)-(me.horizontalAdvance(str)/2),MHeight/2, str);
}

void MainScene::timerMove()
{
    currTime++;
    //遍历所有食物是吃到了哪一个
    for(int i=0;i<totalNum;i++)
    {
        int num = m_snake.isEatFood(totalFood[i]);
        if(num==0)
            continue;
        else
        {
            Food food=creatFood();
            totalFood[i].x=food.x,totalFood[i].y=food.y;

            m_snake.addBody(m_dir);

            m_score+=num;
            if(m_score<0)
            {
                m_status=gameOver;
                m_timer->stop();
            }
            updateScore(m_score);
            update();
            return;
        }
    }
    m_snake.move(m_dir);

    if(m_snake.isCrossBorder() || m_snake.isCrossSelf())
     {
        m_status=gameOver;
        m_timer->stop();
     }
    update();
}

Food MainScene::creatFood()
{
    //这里可以适当加条件判断是否全部格子都满了

    bool flag=true;
    int x,y;
    while(flag)
    {
        flag=false;
        x=QRandomGenerator::global()->bounded(B_W_Count);
        y=QRandomGenerator::global()->bounded(B_H_Count);
        for(auto t:m_snake.m_snake)
        {
            if(t.x==x&&t.y==y)
            {
                flag=true;
                break;
            }
        }

        if(flag)
            continue;

        for(auto t:totalFood)
        {
            if(t.x==x&&t.y==y)
            {
                flag=true;
                break;
            }
        }
    }
    Food food(x,y);
    return food;
}

void MainScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(m_status==gameInit)
    {
        paintGameInit(&painter);
    }
    else if(m_status==gameStart)
    {
        paintFood(&painter);      //后面绘制可以保证蛇在图象上面
        paintSnakeBody(&painter);
    }
    else
        paintGameOver(&painter);
    painBorder(&painter);
}

void MainScene::keyPressEvent(QKeyEvent *event)
{
    if(currTime==lastTime)    //限制快速输入对头部坐标错误
        return;
    //调整蛇头方向也可以直接换图片，如果旋转的话很短时间内输入两个键可能容易出现转向bug
    if(event->key()==Qt::Key_W || event->key()==Qt::Key_Up)
    {
        if(m_dir!=moveDown)
        {
            m_snake.m_snake[0].pix=head[0];
            m_dir=moveTop;
        }
    }
    else if(event->key()==Qt::Key_S || event->key()==Qt::Key_Down)
    {
        if(m_dir!=moveTop)
        {
            m_snake.m_snake[0].pix=head[1];
            m_dir=moveDown;
        }
    }
    else if(event->key()==Qt::Key_A || event->key()==Qt::Key_Left)
    {
        if(m_dir!=moveRight)
        {
            m_snake.m_snake[0].pix=head[2];
            m_dir=moveLeft;
        }
    }
    else if(event->key()==Qt::Key_D || event->key()==Qt::Key_Right)
    {
        if(m_dir!=moveLeft)
        {
            //调整蛇头方位
            m_snake.m_snake[0].pix=head[3];
            m_dir=moveRight;
        }
    }
    lastTime=currTime;
}

void MainScene::init()
{
    m_status=gameInit;
    m_dir=moveLeft;
    m_score=0;
    emit updateScore(m_score);

    m_snake.init();
    initFood();

    //初始化时间
    lastTime=currTime=0;

    //初始化图片
    head[0]=QPixmap(":/images/snake/common/headTop.png");
    head[1]=QPixmap(":/images/snake/common/headDown.png");
    head[2]=QPixmap(":/images/snake/common/headLeft.png");
    head[3]=QPixmap(":/images/snake/common/headRight.png");
}

void MainScene::initFood()
{
    QPixmap temp=QPixmap(":/images/food/drug.png").scaled(BWidth,BWidth);
    for(int i=0;i<drugCount;i++)
    {
        totalFood[i]=creatFood();
        totalFood[i].pix=temp;
        totalFood[i].type=foodType::drug;
    }

    temp=QPixmap(":/images/food/apple.png").scaled(BWidth,BWidth);
    for(int i=drugCount;i<appleCount+drugCount;i++)
    {
        totalFood[i]=creatFood();
        totalFood[i].pix=temp;
        totalFood[i].type=foodType::apple;
    }

    temp=QPixmap(":/images/food/banana.png").scaled(BWidth,BWidth);
    for(int i=appleCount+drugCount;i<totalNum;i++)
    {
        totalFood[i]=creatFood();
        totalFood[i].pix=temp;
        totalFood[i].type=foodType::banana;
    }
}
