#ifndef STRUCT_H
#define STRUCT_H

#include <QObject>
#include <QString>

struct HeroInfo
{
    int id;
    QString name;       // 修改 Qstring -> QString
    QString career;
    QString attribute;
    QString skill;
};

struct UserInfo
{
    QString username;
    QString password;
    QString auth;
};

//class DatabaseHandler : public QObject  // 修改 struct -> DatabaseHandler
//{
//    Q_OBJECT
//public:
//    explicit DatabaseHandler(QObject *parent = nullptr);

//    // 查询所有英雄数量
//    quint32 getHeroCount();

//    // 查询第几页英雄数据
//    QList<HeroInfo> getHeroesByPage(quint32 page,quint32 uicount);

//    // 增加英雄
//    bool addHero( HeroInfo );

//    // 删除英雄
//    bool deleteHero(int heroId);

//    // 清空英雄表
//    bool clearHeroTable();

//    // 修改英雄信息
//    void updateHero( HeroInfo info);

//    // 查询所有用户
//    QList<UserInfo> getAllUsers();

//    // 查询用户名是否存在
//    bool doesUserExist( QString struser);

//    // 修改用户权限
//    void changeUserAuth( QString str);

//    // 添加单个用户
//    bool addUser( UserInfo info);

//    // 删除单个用户
//    void deleteUser( QString strusername);

//signals:
////    void heroAdded(const HeroInfo &hero);  // 信号举例
////    void userAdded(const UserInfo &user);
//};

#endif // STRUCT_H

