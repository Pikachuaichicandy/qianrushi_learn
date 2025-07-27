#include "bgmcontrol.h"
#include<QFile>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>

bgmControl::bgmControl(QObject *parent)
    : QObject{parent}
{
    for(int i=0;i<3;i++)
    {
        QMediaPlayer* player=new QMediaPlayer(this);
        QMediaPlaylist* list=new QMediaPlaylist(this);
        player->setPlaylist(list);
        player->setVolume(100);
        m_player.push_back(player);
        m_playerlist.push_back(list);
    }
    m_playerlist[0]->setPlaybackMode(QMediaPlaylist::Loop);
    m_playerlist[1]->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);  //只播放一次
    m_playerlist[2]->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);  //只播放一次
    initPlayList();
}

void bgmControl::startBackgroundMusic(int choice,int volume)
{
    m_playerlist[0]->setCurrentIndex(choice);
    m_player[0]->setVolume(volume);
    m_player[0]->play();
}

void bgmControl::stopBackgroundMusic()
{
    m_player[0]->stop();
}

bool bgmControl::isStartPlayBG()
{
    if(m_player[0]->state()==QMediaPlayer::PlayingState)
        return true;
    return false;
}

void bgmControl::startAddScoreMusic()
{
    m_playerlist[1]->setCurrentIndex(1);
    m_player[1]->play();
}

void bgmControl::startSubScoreMusic()
{
    m_playerlist[1]->setCurrentIndex(0);
    m_player[1]->play();
}

void bgmControl::startEndMusic(int choice)
{
    m_playerlist[2]->setCurrentIndex(choice);
    m_player[2]->play();
}

void bgmControl::initPlayList()
{
    QStringList list;
    list<<"backgroundMusic"<<"snakeMusic"<<"endMusic";
    //读json文件
    QFile file(":/config/playList.json");
    file.open(QFile::ReadOnly);
    QByteArray json=file.readAll();
    file.close();

    //解析json文件
    QJsonDocument doc=QJsonDocument::fromJson(json);
    //转为json对象
    QJsonObject obj=doc.object();

    for(int i=0;i<list.size();i++)
    {
        QString index=list[i];
        QJsonArray array=obj.value(index).toArray();
        for(int j=0;j<array.size();j++)
        {
            m_playerlist[i]->addMedia(QMediaContent(QUrl(array[j].toString())));
        }
    }
    qDebug()<<"json读取成功！";
}
