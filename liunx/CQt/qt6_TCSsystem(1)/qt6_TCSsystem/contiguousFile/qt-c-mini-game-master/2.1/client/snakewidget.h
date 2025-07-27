#ifndef SNAKEWIDGET_H
#define SNAKEWIDGET_H

#include <QWidget>
#include<QVector>
#include<QKeyEvent>
#include<QTimer>
#include"struct.h"
#include"userwidget.h"

class SnakeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SnakeWidget(QWidget *parent = nullptr);
    ~SnakeWidget();

    //重置游戏
    void init();
    //接收用户数据
    void recvData(bool isSelf,int posi,Point point,Point food,QString name,int head,int body);
    //接收方向数据
    void recvDir(int posi,int dir);
    //接收玩家死亡信息
    void recvUserDeath(int posi);
    //游戏结束
    void gameOver();
    //接收食物坐标
    void recvFoodPoint(Point p);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event);

signals:
    void changeDir(int head,int dir,int posi);
    void sendDeath(int head,int data,int posi);    //发送死亡信息
    void returnLogin();    //返回登陆页面

private:
    QPixmap setSkin(int posi,int headPix,int bodyPix);     //初始化皮肤，和返回一个蛇头皮肤
    void paintBorder(QPainter *painter);
    void paintFood(QPainter *painter);
    void paintSnake(QPainter *painter);

    bool isCrossSnake();       //判断自己是否撞到自己或者别人
    bool isCrossBorder();     //是否越过边界
    void isEatFood(Food& food);    //是否吃到食物
    void move(int posi);   //移动
    void addBody(int posi);  //根据方向调整添加的尾部
    void timerMove();        //定时器移动函数

    bool keyFlag,forbidKey;     //防止键盘快速输入
    int m_position;    //对应的自己的坐标
    int m_LastDir;     //如果是上一次的移动方向则不进行发送处理
    int totalUser;    //总人数
    Food m_food;       //食物坐标

    QTimer* m_timer;
    //显示用户窗口类
    UserWidget* m_userWidget[userNum];

    QVector<int> userScore;     //分数
    QVector<int> userDir;       //对应的方向
    QVector<QVector<snakeBody>> snake;
    QVector<QString> username;  //用户名
    QVector<QVector<QPixmap>> headSkin;    //每个用户对应四个蛇头
    QVector<QPixmap> bodySkin;      //因为目前只有蛇头发送过来，所以蛇身皮肤要保存
};

#endif // SNAKEWIDGET_H
