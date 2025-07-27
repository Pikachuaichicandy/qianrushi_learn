#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QWidget>
#include"snake.h"
#include"bgmcontrol.h"
#include<QTimer>
#include<QKeyEvent>
#include<QRandomGenerator>

class MainScene : public QWidget
{
    Q_OBJECT
public:
    explicit MainScene(QWidget *parent = nullptr);

    //开始，暂停，结束游戏
    void start();
    void pause();
    void gameover();
    void setbgroundPix();    //改变背景皮肤

    //开始启动寻找（出现问题时可进行人工干预模式）
    void startFindWay();

    bgmControl* musicControl;   //公共音乐类

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void updateScore(int score);   //跟主界面分数交互
    void startFindWaySignal();
    void gameOverOrStart();

private:
    void init();      //初始化
    void initFood();   //初始化食物
    Food creatFood();     //创造食物

    void painBorder(QPainter* painter);   //画背景边框
    void paintSnakeBody(QPainter* painter);   //画蛇
    void paintFood(QPainter* painter);    //画食物
    void paintGameInit(QPainter* painter);
    void paintGameShiftPattern(QPainter* painter);    //模式切换时提示
    void paintGameOver(QPainter* painter);
    void paintCenterText(QPainter* painter,QString& str);
    void paintBgroundPix(QPainter* painter);

    void timerMove();
    //BFS机器寻找（还需要优化遇到死胡同问题，判断食物是否被包围蛇头不能出来）---深搜（更好处理死路问题，不过时间要多）
    QVector<Point> getSnakePoint();
    QVector<Point> getMovedSnakePoint(QVector<Point> snakePoint,char movedir);
    bool isReverse(char a,char b);      //算法中防止头像逆向走
    bool isCrossSelf(QVector<Point>& snakePoint);  //防止撞到自己（这里可以）
    bool isSurroundFood(Food& food,QVector<Point>& point);  //判断食物WSAD是否被包围，原则上至少要有两条路才能吃
    std::string AlBfsWay();
    void startExeFindWay();
    void shiftPersonPattern();

    QTimer* m_timer;
    QTimer* m_alTimer;
    QPixmap m_bgroundPix;    //背景图片
    Snake m_snake;
    gameStatus m_status;
    Food totalFood[totalNum];    //毒药，苹果，香蕉
    moveDir m_dir;
    int m_score;
    int m_bground;   //储存背景选项
    bool keyFlag,isAlPattern;   //键盘输入标志，al模式

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
