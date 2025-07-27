#include "snake.h"
#include "ui_snake.h"
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QDebug>
#include <QTimer>
#include <QRandomGenerator>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QString>
#include "gameover.h"
#include <QFile>



snake::snake(QString musicStr, int delay, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::snake)
    , delay(delay)
{
    ui->setupUi(this);
    this->resize(1004,600);
    //播放背景音乐
    QMediaPlayer* music = new QMediaPlayer(this);
    QMediaPlaylist *musicList = new QMediaPlaylist(music);
    musicList->addMedia(QMediaContent(QUrl::fromLocalFile(musicStr)));
    musicList->setPlaybackMode(QMediaPlaylist::Loop);
    music->setPlaylist(musicList);
    if(musicStr == "F:/qt6project/qt6_TCSsystem/contiguousFile/game/hard.MP3")
        music->setVolume(100);
    else
        music->setVolume(30);
    music->play();

    run();
}

snake::~snake()
{
    delete ui;
}

void snake::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    //蛇头用红色,蛇身用绿色
    QBrush brushGreen(QColor("green"));
    QBrush brushRed(QColor("red"));
    QBrush brushYellow(QColor("yellow"));

    //依据snakeList画出蛇
    int size=snakeList.size();
    for(int i=0;i<size;i++){
        if(i==0){
            painter.setBrush(brushRed);
            painter.drawEllipse(snakeList[i],10,10);
            painter.setBrush(brushGreen);
            continue;
        }
        painter.drawEllipse(snakeList[i],10,10);
    }
    //画food
    painter.setBrush(brushYellow);
    painter.drawEllipse(FoodSite,10,10);
}

void snake::snakeUnit()
{
    //蛇身体初始化
    QPoint point(190,250);
    snakeList.push_back(point);
    QPoint point2(170,250);
    snakeList.push_back(point2);
    QPoint point3(150,250);
    snakeList.push_back(point3);
    this->update();
    snakeOrient=orientation::Right;
}

void snake::snakeMove()
{
    //蛇的移动，链表头部+1，尾部暂时不改变。
    int x=snakeList[0].rx();
    int y=snakeList[0].ry();
    QPoint point(x,y);
    switch (snakeOrient){
        case orientation::Up:
            point.setY(y-20);
        break;
        case orientation::Down:
            point.setY(y+20);
        break;
        case orientation::Left:
            point.setX(x-20);
        break;
        case orientation::Right:
            point.setX(x+20);
        break;
    }
    snakeList.push_front(point);
    //snakeList.pop_back();
}

void snake::foodCreate()
{
    bool siteJudgment=false;
    //判断food给出的位置是否合理：不能与蛇重叠。直到选出一个合适的位置跳出循环。
    while(!siteJudgment){
        siteJudgment=true;
        int x = QRandomGenerator::global()->bounded(1,41);
        int y = QRandomGenerator::global()->bounded(1,31);
        FoodSite.setX(x*20-10);
        FoodSite.setY(y*20-10);
        QList<QPoint>::iterator zhen=snakeList.begin();
        for(;zhen != snakeList.end();zhen++){
            if(*zhen == FoodSite){
                siteJudgment=false;
                break;
            }
        }
    }
}

void snake::GameOver(QString str1, QString str2)
{
    //中断定时器，打开gameover的对话窗口
    delete timer;
    gameover* gameoverWidget=new gameover(this);
    gameoverWidget->setText(str1,str2);
    gameoverWidget->show();
    gameoverWidget->exec();
    delete gameoverWidget;
    //恢复初始化
    snakeList.clear();
    score=0;
    ui->label_3->setText(QString::number(score));
}

void snake::updataRank()
{
    QVector<QString> scoreList;
    QFile file;
    if(delay == 300)
        file.setFileName("F:/qt6project/qt6_TCSsystem/contiguousFile/game/scoreCommon.txt");
    else
        file.setFileName("F:/qt6project/qt6_TCSsystem/contiguousFile/game/scoreHard.txt");
    //只读打开文件
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"文件打开失败"<<endl;
        return;
    }
    QString str;
    //读取排行榜文件中的内容
    for(int i=0;i<10;i++){
        str=file.readLine();
        str.remove("\n");
        if(str.length()==0)
            break;
        scoreList.push_back(str);
    }
    file.close();

    //更新排行榜->把新分数插进入
    str=QString::number(score);
    int num=scoreList.size();
    int insertID=num;
    for(int j=0;j<num;j++){
        if(str >= scoreList[j]){
            insertID=j;
            break;
        }
    }
    //找到插入位置后，插入新分数
    if(insertID == num)
        scoreList.push_back(str);
    else{
        scoreList.push_back(scoreList[num-1]);
        for(int k=num-1;k>insertID;k--){
            scoreList[k]=scoreList[k-1];
        }
        scoreList[insertID]=str;
    }
    //最多10个排名，多余的出栈。
    if(num == 10)
        scoreList.pop_back();
    QString newscore;
    for(int n=0;n<scoreList.size();n++){
        newscore=newscore+scoreList[n]+"\n";
    }
    //只写方式打开文件，把新的排行榜写进去
    if(!file.open(QIODevice::WriteOnly)){
        qDebug()<<"文件打开失败"<<endl;
        return;
    }
    file.write(newscore.toUtf8());
    file.close();
}

void snake::run()
{
    snakeUnit();
    foodCreate();
    timer=new QTimer(this);
    timer->start(delay);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

void snake::on_pushButton_clicked()
{
    if(snakeOrient != orientation::Down)
        snakeOrient=orientation::Up;
}

void snake::on_pushButton_2_clicked()
{
    if(snakeOrient != orientation::Up)
        snakeOrient=orientation::Down;
}

void snake::on_pushButton_4_clicked()
{
    if(snakeOrient != orientation::Right)
        snakeOrient=orientation::Left;
}

void snake::on_pushButton_3_clicked()
{
    if(snakeOrient != orientation::Left)
        snakeOrient=orientation::Right;
}

void snake::onTimeOut()
{
    snakeMove();
    //判断越界
    if(snakeList[0].x()<0 || snakeList[0].x()>800 || snakeList[0].y()<0 || snakeList[0].y()>600){
        //删除非法蛇头，不画出来
        snakeList.pop_front();
        //更新排行榜
        updataRank();
        QString str1="头撞墙里死了";
        QString str2="最终得分:"+QString::number(score)+"分";
        GameOver(str1,str2);
        //此局游戏结束，再来一局重新run
        run();
        return;
    }
    //判断吃自己
    QList<QPoint>::iterator head=snakeList.begin();
    QList<QPoint>::iterator zhen=snakeList.begin();
    zhen++;
    for(;zhen != snakeList.end();zhen++){
        if(*zhen == *head){
            snakeList.pop_front();
            //更新排行榜
            updataRank();
            QString str1="吃自己嘎了";
            QString str2="最终得分:"+QString::number(score)+"分";
            GameOver(str1,str2);
            //此局游戏结束，再来一局重新run
            run();
            return;
        }
    }
    //吃到食物则更新分数，同时创造新食物
    if(snakeList[0] == FoodSite){
        score++;
        ui->label_3->setText(QString::number(score));
        foodCreate();
    }
    //没吃到，蛇链表尾部出栈
    else
       snakeList.pop_back();
    //更新画面
    this->update();
}


void snake::on_pushButton_5_clicked()
{
    QCoreApplication::exit();
}
