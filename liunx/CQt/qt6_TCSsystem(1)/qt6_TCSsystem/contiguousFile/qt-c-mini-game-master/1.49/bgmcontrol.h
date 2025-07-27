#ifndef BGMCONTROL_H
#define BGMCONTROL_H

#include <QObject>
#include<QMediaPlayer>
#include<QMediaPlaylist>
#include<QVector>

//如果存在同时音效的时候，可以通过计时器延迟播放一下

class bgmControl : public QObject
{
    Q_OBJECT
public:
    explicit bgmControl(QObject *parent = nullptr);

    //这里的choice是为了方便商城系统选择
    void startBackgroundMusic(int choice=0,int volume=100);   //播放/暂停背景音
    void stopBackgroundMusic();
    bool isStartPlayBG();

    void startAddScoreMusic();     //播放吃到食物/扣分
    void startSubScoreMusic();

    void startEndMusic(int choice=0);

signals:

private:
    //同时可以考虑QSound 是一个简单的类（轻量级），主要用于播放短暂的音频效果。
    void initPlayList();
    QVector<QMediaPlayer*> m_player;        //对应不同的播放器（例如：1.背景，2.游戏内音效,3.结束音效）
    QVector<QMediaPlaylist*> m_playerlist;   //对应不同的播放器列表（例如：背景，游戏内音效）
};

#endif // BGMCONTROL_H
