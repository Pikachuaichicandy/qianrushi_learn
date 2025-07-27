#ifndef SNAKE_H
#define SNAKE_H

#include <QWidget>
#include <QList>
#include <QVector>
#include <QPoint>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QString>
#include <QTimer>

namespace Ui {
class snake;
}

enum orientation{
    Up,
    Down,
    Left,
    Right
};

class snake : public QWidget
{
    Q_OBJECT
public:
    QList<QPoint> snakeList;

public:
    snake(QString musicStr, int delay, QWidget *parent = nullptr);
    ~snake();
    void paintEvent(QPaintEvent *event) override;
    void snakeUnit();
    void snakeMove();//函数内尾部不删除
    void foodCreate();
    void run();
    void GameOver(QString str1, QString str2);
    void updataRank();



private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void onTimeOut();

    void on_pushButton_5_clicked();

private:
    Ui::snake *ui;
    int snakeOrient;
    int delay;
    QPoint FoodSite;
    int score=0;
    int textLeft;
    QTimer* timer;
};

#endif // SNAKE_H
