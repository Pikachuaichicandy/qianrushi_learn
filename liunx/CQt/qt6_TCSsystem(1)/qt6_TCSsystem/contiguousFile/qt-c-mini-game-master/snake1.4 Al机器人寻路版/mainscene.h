#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QWidget>
#include"snake.h"
#include"bgmcontrol.h"
#include<QTimer>
#include<QKeyEvent>
#include<QRandomGenerator>
#include<QMutex>
#include<QSettings>

class MainScene : public QWidget
{
    Q_OBJECT
public:
    explicit MainScene(QWidget *parent = nullptr);

    //开始，暂停，结束游戏
    void start();
    void pause();
    void gameover();

    //开始启动寻找
    void startFindWay();

    bgmControl* musicControl;   //公共音乐类

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void updateScore(int score);   //跟主界面分数交互
    void startFindWaySignal();

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
    //BFS机器寻找
    QVector<Point> getSnakePoint();
    QVector<Point> getMovedSnakePoint(QVector<Point> snakePoint,char movedir);
    bool isReverse(char a,char b);      //算法中防止头像逆向走
    bool isCrossSelf(QVector<Point>& snakePoint);  //防止撞到自己（这里可以）
    std::string AlBfsWay();
    void startExeFindWay();

    QTimer* m_timer;
    QTimer* m_alTimer;
    Snake m_snake;
    gameStatus m_status;
    Food totalFood[totalNum];    //毒药，苹果，香蕉
    moveDir m_dir;
    int m_score;
    bool keyFlag;

    //BFS
    int dir[4][2]={{0,-1},{0,1},{-1,0},{1,0}};   //上下左右
    std::string fourDir="WSAD";
    struct Node{
        int x,y;
        std::string s;   //存储路径
        QVector<Point> snakePoint;     //保存相对的蛇的路径坐标，防止相撞
        Node(int a,int b,std::string c,QVector<Point> d)
        {
            x=a,y=b,s=c,snakePoint=d;
        }
    };
    int ansCount;
    std::string ans;
};

#endif // MAINSCENE_H
