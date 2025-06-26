#ifndef SQLITE_H
#define SQLITE_H
#include "struct.h"
#include <QObject>
#include <QSqlDatabase>
class sqlite : public QObject
{
    Q_OBJECT
public:
    static sqlite *ptrsqlite;
    static sqlite *getinstance()
    {
        if(nullptr == ptrsqlite)
        {
            ptrsqlite = new sqlite;
        }
        return ptrsqlite;
    }
    explicit sqlite(QObject *parent = nullptr);

    void init();

    // 查询所有英雄数量
    quint32 getHeroCount();

    // 查询第几页英雄数据
    QList<HeroInfo> getHeroesByPage(quint32 page,quint32 uicount);

    // 增加英雄
    bool addHero( HeroInfo info);

    // 删除英雄
    bool deleteHero(int heroid);

    // 清空英雄表
    bool clearHeroTable();

    // 修改英雄信息
    bool updateHeroInfo( HeroInfo info);

    // 查询所有用户
    QList<UserInfo> getAllUsers();

    // 查询用户名是否存在
    bool doesUserExist( QString struser);

    // 更新用户信息
    bool updataUserAuth( UserInfo info);

    // 添加单个用户
    bool addUser( UserInfo info);

    // 删除单个用户
    bool deleteUser( QString strusername);


signals:

private:
    QSqlDatabase m_db;


};

#endif // SQLITE_H
