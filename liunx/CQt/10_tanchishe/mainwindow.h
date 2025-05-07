//#ifndef MAINWINDOW_H
//#define MAINWINDOW_H
//#include <QKeyEvent>
//#include <QMainWindow>
//#include <QList>
//#include <QRectF>

//QT_BEGIN_NAMESPACE
//namespace Ui { class MainWindow; }
//QT_END_NAMESPACE

//enum Direct
//{
//    LEFT,
//    RIGHT,
//    UP,
//    DOWN
//};

//class MainWindow : public QMainWindow
//{
//    Q_OBJECT

//public:
//    MainWindow(QWidget *parent = nullptr);
//    ~MainWindow();

//    //按键处理
//    void keyPressEvent(QKeyEvent *event);

//    int moveFlag = UP;
//    bool gameStar = false;

//    //定时器
//    QTimer *timer;


//private:
//    Ui::MainWindow *ui;
//    QList<QRectF> snake;  // 声明存储 QRectF 的 QList
//protected slots:
//    void timeout();
//protected:
//    void addTop();
//    void addDown();
//    void addRight();
//    void addLeft();
//    void deleteLast();
//    void addNewReward();
//    bool checkContact();
//    void updateScoreDisplay();
//    void restartGame();
//    //蛇
////    QList <QRectF> snake;
//    //小方块
//    int nodeWidth = 20;
//    int nodeHeight = 20;
//    //渲染
//    void paintEvent(QPaintEvent *);
//    //奖品
//    QRectF rewardNode;

//};
//#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QTime>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QRectF>
#include <QList>
#include <QPushButton>
#include <QFont>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void timeout();

private:
    void  initsnake();
    void  initfood();
    void addTop();
    void addDown();
    void addLeft();
    void addRight();
    void deleteLast();
    void addNewReward();
    void updateScoreDisplay();
    bool checkContact();
    void restartGame();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QList<QRectF> snake;
    QRectF rewardNode;
    QTime lastKeyPressTime;
    int moveSpeed;
    int score=0;
    bool gameStar;
    enum Direction { UP, DOWN, LEFT, RIGHT };
    Direction moveFlag;
    int nodeWidth = 20;
    int nodeHeight = 20;
    QTime gameTime; // 声明 gameTime 变量
    QPushButton *btn;
};

#endif // MAINWINDOW_H

