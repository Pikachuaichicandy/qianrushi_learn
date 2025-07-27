#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QWidget>
#include"snake.h"
#include<QTimer>
#include<QKeyEvent>
#include<QRandomGenerator>
#include<QMutex>

class MainScene : public QWidget
{
    Q_OBJECT
public:
    explicit MainScene(QWidget *parent = nullptr);

    //开始，暂停，结束游戏
    void start();
    void pause();
    void gameover();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void updateScore(int score);   //跟主界面分数交互

private:
    void init();      //初始化
    void initFood();   //初始化食物
    Food creatFood();     //创造食物

    void painBorder(QPainter* painter);   //画背景边框
    void paintSnakeBody(QPainter* painter);   //画蛇
    void paintFood(QPainter* painter);    //画食物
    void paintGameInit(QPainter* painter);
    void paintGameOver(QPainter* painter);
    void paintCenterText(QPainter* painter,QString& str);

    void timerMove();

    QTimer* m_timer;
    Snake m_snake;
    gameStatus m_status;
    Food totalFood[totalNum];    //毒药，苹果，香蕉
    moveDir m_dir;
    int m_score;
    int lastTime,currTime;
    QPixmap head[4];
};

#endif // MAINSCENE_H
