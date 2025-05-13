#include "mainscene.h"
#include "config.h"
//#include "struct.h"
#include <QFontMetrics>
#include <QPainter>
#include <QDebug>
#include <QString>

MainScene::MainScene(QWidget *parent):QWidget{parent} //构造函数定义
{
    //可以进行一些初始化
    m_timer = new QTimer(this);
    m_timer->setInterval(timerNum);
    init();
    connect(m_timer,&QTimer::timeout,this,&MainScene::timerMove);
}

void MainScene::start()
{
    init();
    setFocus();  //获得聚焦
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
    update();   //这里要手动更新
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
    for(int i = 0;i<totalNum;i++)
    {
        painter->drawPixmap(totalFood[i].x*BWidth,totalFood[i].y*BWidth,totalFood[i].pix);
    }
}

void MainScene::paintGameInit(QPainter *painter)
{
    QString str = "苹果（10分），香蕉（20分），毒药/障碍物（-30分） 分数小于0时死亡/失败！";
    PaintCenterText(painter,str);
}

void MainScene::PainterGameOver(QPainter *painter)
{
    QString str = QString("游戏结束，最终分数为：1%").arg(m_score);
    PaintCenterText(painter,str);
}

void MainScene::PaintCenterText(QPainter *painter,QString &str)
{
    QFont font;
    font.setPixelSize(BWidth);
    painter->setFont(font);
    //QFontMetrics提供了在给定字体和字号下测量文本的方法
    QFontMetrics me(font);
    painter->drawText((MWidth/2)-(me.horizontalAdvance(str)/2),MHeight/2,str);

}

void MainScene::timerMove()
{
    currTime++;

}

