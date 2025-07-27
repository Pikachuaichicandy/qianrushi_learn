#include "mainscene.h"
#include"config.h"
#include <QFontMetrics>
#include <QPainter>
#include<queue>
#include<QThread>
#include<QDebug>
#include<iostream>
#include<QSettings>

MainScene::MainScene(QWidget *parent)
    : QWidget{parent}
{
    //初始化类
    musicControl=new bgmControl(this);
    m_timer=new QTimer(this);
    m_timer->setInterval(timerNum);
    m_alTimer=new QTimer(this);
    m_alTimer->setInterval(50);

    init();
    connect(m_timer,&QTimer::timeout,this,&MainScene::timerMove);
    connect(m_alTimer,&QTimer::timeout,this,&MainScene::startExeFindWay);
    connect(this,&MainScene::startFindWaySignal,this,&MainScene::startFindWay);

    //播放背景音
    //musicControl->startBackgroundMusic();
}

void MainScene::start()
{
    init();
    setFocus();   //获得聚焦
    m_status=gameStart;
    emit gameOverOrStart();
    m_alTimer->stop();
    m_timer->start();
}

void MainScene::pause()
{
    if(m_status!=gameStart)
        return;
    if(isAlPattern)
        m_alTimer->isActive()? m_alTimer->stop():m_alTimer->start();
    else
        m_timer->isActive()? m_timer->stop():m_timer->start();

    setFocus();
}

void MainScene::gameover()
{
    m_status=gameOver;
    musicControl->startEndMusic();
    m_timer->stop();
    m_alTimer->stop();
    update();    //这里要手动更新
    emit gameOverOrStart();
    //这里可以加一些listview的记录
}

void MainScene::setbgroundPix()
{
    QSettings set("D:/Qtpro/cmake/snake/config/skin.ini", QSettings::IniFormat);
    m_bground=set.value("bground").toInt();
    if(m_bground==0)
        m_bgroundPix=QPixmap();
    else
        m_bgroundPix=QPixmap(QString(":/images/background/background%1.jpg").arg(m_bground)).scaled(MWidth,MHeight);
    update();
}

std::string MainScene::AlBfsWay()
{
    bool vis[B_W_Count+1][B_H_Count+1];
    for(int i=0;i<B_W_Count+1;i++)
        for(int j=0;j<B_H_Count+1;j++)
            vis[i][j]=false;
    //优化--毒药食物直接不可读
    for(int i=0;i<drugCount;i++)
        vis[totalFood[i].x][totalFood[i].y]=true;

    vis[m_snake.m_snake[0].x][m_snake.m_snake[0].y]=true;
    std::queue<Node> q;

    Node node(m_snake.m_snake[0].x,m_snake.m_snake[0].y,
              std::string(1,fourDir[int(m_dir)]),getSnakePoint());
    //std::cout<<"0 node back: "<<node.s<<"  "<<node.s.size()<< std::endl;
    q.push(node);

    while (!q.empty())
    {
        Node t=q.front();q.pop();
        for(int i=0;i<4;i++)
        {
            int x=t.x+dir[i][0];
            int y=t.y+dir[i][1];

            if(x>=0 && x<B_W_Count && y>=0 && y<B_H_Count && !vis[x][y] &&
                isReverse(t.s[t.s.size()-1],fourDir[i]))
            {
                QVector<Point> point = getMovedSnakePoint(t.snakePoint,fourDir[i]);
                if(isCrossSelf(point))
                    continue;
                //bool flag=true;
                //遍历所有食物是吃到了哪一个
                for(int j=drugCount;j<totalNum;j++)
                {
                    int num = m_snake.isAlEatFood(totalFood[j],x,y);
                    if(num>0)
                    {
                        if(isSurroundFood(totalFood[j],point))
                            continue;
                        return t.s+fourDir[i];
                    }
//                    else if(num<0)
//                    {
//                        flag=false;
//                        break;
//                    }
                }
//                if(!flag)   //这个点存在食物则继续循环
//                    continue;
                Node temp(x,y,t.s+fourDir[i],point);
                vis[x][y]=true;
                q.push(temp);
            }
        }
    }
    std::string s(1,fourDir[int(m_dir)]);
    std::cout<<"empty: "<<fourDir[int(m_dir)]<<std::endl;
    return s;
}

void MainScene::startExeFindWay()
{
    if(ans.size()==1)   //人工干预模式
    {
        shiftPersonPattern();
        return;
    }

    if(ans[ansCount]=='W')
        m_dir=moveTop;
    else if(ans[ansCount]=='S')
        m_dir=moveDown;
    else if(ans[ansCount]=='A')
        m_dir=moveLeft;
    else if(ans[ansCount]=='D')
        m_dir=moveRight;

    if(ansCount==ans.size()-1)
    {
        timerMove();
        m_alTimer->stop();
        update();
        emit startFindWaySignal();
        return;
    }
    timerMove();

    ansCount++;
    update();
}

void MainScene::shiftPersonPattern()
{
    m_alTimer->stop();
    isAlPattern=false;
    m_status=gameShiftPattern;
    update();
    QTimer::singleShot(5000,[&](){
        m_status=gameStart;
        update();
    });
}

void MainScene::startFindWay()
{
    isAlPattern=true;
    if(m_timer->isActive())
        m_timer->stop();
    ansCount=1;
    ans=AlBfsWay();

    std::cout<<"ans: "<<ans<<"---"<<ans.size()<<std::endl;
    m_alTimer->start();
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

void MainScene::paintGameShiftPattern(QPainter *painter)
{
    QString str=QString("五秒钟后开始%1模式，按下暂停/播放 开始").arg(isAlPattern? "机器":"人工");
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

void MainScene::paintBgroundPix(QPainter *painter)
{
    painter->drawPixmap(0,0,MWidth,MHeight,m_bgroundPix);
}

void MainScene::timerMove()
{
    keyFlag=false;
    m_snake.move(m_dir);
    //遍历所有食物是吃到了哪一个
    for(int i=0;i<totalNum;i++)
    {
        int num = m_snake.isEatFood(totalFood[i]);
        if(num==0)
            continue;
        else
        {
            if(num>0)
                musicControl->startAddScoreMusic();
            else
                musicControl->startSubScoreMusic();

            Food food=creatFood();
            totalFood[i].x=food.x,totalFood[i].y=food.y;

            m_snake.addBody(m_dir);

            m_score+=num;
            if(m_score<0)
            {
                gameover();
            }
            updateScore(m_score);
            update();
            return;
        }
    }


    if(m_snake.isCrossBorder() || m_snake.isCrossSelf())
     {
        gameover();
     }
    update();
}

QVector<Point> MainScene::getSnakePoint()
{
    QVector<Point> snakePoint;
    for(int i=0;i<m_snake.m_snake.size();i++)
    {
        snakePoint.push_back(m_snake.m_snake[i].getPosition());
    }
    return snakePoint;
}

QVector<Point> MainScene::getMovedSnakePoint(QVector<Point> snakePoint, char movedir)
{
    //每一个蛇身移动到它的前一位(这里应该从大到小去移动，不然会有bug)
    for(int i=snakePoint.size()-1;i>0;i--)
    {
        snakePoint[i]=snakePoint[i-1];
    }

    //改变蛇头
    if(movedir=='W')
        snakePoint[0].y-=1;
    else if(movedir=='S')
        snakePoint[0].y+=1;
    else if(movedir=='A')
        snakePoint[0].x-=1;
    else if(movedir=='D')
        snakePoint[0].x+=1;
    return snakePoint;
}

bool MainScene::isReverse(char a, char b)
{
    if(a=='W'&&b=='S')
        return false;
    else if(a=='S'&&b=='W')
        return false;
    else if(a=='A'&&b=='D')
        return false;
    else if(a=='D'&&b=='A')
        return false;
    return true;
}

bool MainScene::isCrossSelf(QVector<Point>& snakePoint)
{
    int x=snakePoint[0].x,y=snakePoint[0].y;
    for(int i=1;i<snakePoint.size();i++)
    {
        if(x==snakePoint[i].x&&y==snakePoint[i].y)
            return true;
    }
    return false;
}

bool MainScene::isSurroundFood(Food& food, QVector<Point> &point)
{
    int count=0;
    for(int i=0;i<drugCount;i++)
    {
        if(food==totalFood[i])    //计算障碍物带来的阻碍次数
            count++;
    }
    //判断是否越界
    if(food.x-1<0 || food.x+1>B_W_Count || food.y-1<0 || food.y+1>B_H_Count)
        count++;
    if(count>2)
        return true;
    for(int i=0;i<point.size();i++)
    {
        if(food==point[i])
            count++;
    }
    if(count>2)
        return true;
    return false;
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

    if(m_bground!=0)
        paintBgroundPix(&painter);

    if(m_status==gameInit)
    {
        paintGameInit(&painter);
    }
    else if(m_status==gameStart)
    {
        paintFood(&painter);      //后面绘制可以保证蛇在图象上面
        paintSnakeBody(&painter);
    }
    else if(m_status==gameShiftPattern)
    {
        paintGameShiftPattern(&painter);
    }
    else
        paintGameOver(&painter);
    painBorder(&painter);
}

void MainScene::keyPressEvent(QKeyEvent *event)
{
    if(keyFlag || isAlPattern)    //限制快速输入对头部坐标错误和al模式禁输入
        return;
    //调整蛇头方向也可以直接换图片，如果旋转的话很短时间内输入两个键可能容易出现转向bug
    if(event->key()==Qt::Key_W || event->key()==Qt::Key_Up)
    {
        if(m_dir!=moveDown&&m_dir!=moveTop)
        {
            //m_snake.m_snake[0].pix=head[0];
            m_dir=moveTop;
        }
    }
    else if(event->key()==Qt::Key_S || event->key()==Qt::Key_Down)
    {
        if(m_dir!=moveTop&&m_dir!=moveDown)
        {
            //m_snake.m_snake[0].pix=head[1];
            m_dir=moveDown;
        }
    }
    else if(event->key()==Qt::Key_A || event->key()==Qt::Key_Left)
    {
        if(m_dir!=moveRight&&m_dir!=moveLeft)
        {
            //m_snake.m_snake[0].pix=head[2];
            m_dir=moveLeft;
        }
    }
    else if(event->key()==Qt::Key_D || event->key()==Qt::Key_Right)
    {
        if(m_dir!=moveLeft&&m_dir!=moveRight)
        {
            //调整蛇头方位
            //m_snake.m_snake[0].pix=head[3];
            m_dir=moveRight;
        }
    }
    keyFlag=true;
}

void MainScene::init()
{
    m_status=gameInit;
    m_dir=moveLeft;
    m_score=0;
    emit updateScore(m_score);

    m_snake.init();
    initFood();

    //初始化键盘判断标志,和是否al模式
    keyFlag=isAlPattern=false;

    setbgroundPix();
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
