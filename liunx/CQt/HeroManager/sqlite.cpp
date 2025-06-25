#include "sqlite.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QObject>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QCoreApplication>

sqlite::sqlite(QObject *parent) : QObject(parent)
{
//    if (QSqlDatabase::drivers().isEmpty())
//        QMessageBox::information(this, tr("No database drivers found"),
//                                 tr("This demo requires at least one Qt database driver. "
//                                    "Please check the documentation how to build the "
//                                    "Qt SQL plugins."));

//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "F:\32keil_chenxundaimakaifa\liunx\CQt\HeroManager\data.db");

//    db.setDatabaseName("data");
//    if (!db.open())
//        QMessageBox::warning(nullptr, tr("Unable to open database"), tr("An error occurred while "
//                                                                     "opening the connection: ") + db.lastError().text());

//    QSqlQuery q("", db);
////    q.exec("INSERT INTO "Hero" VALUES (NULL, '夏洛特','战士', '物理', '七星斩');");
//    q.exec("INSERT INTO Hero VALUES (NULL, '夏洛特', '战士', '物理', '七星斩');");


//sqll连接
//    if (QSqlDatabase::drivers().isEmpty()) {
//        QMessageBox::information(nullptr, tr("No database drivers found"),
//                                 tr("This demo requires at least one Qt database driver. "
//                                    "Please check the documentation how to build the "
//                                    "Qt SQL plugins."));
//    }

//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("F:/32keil_chenxundaimakaifa/liunx/CQt/HeroManager/data.db");

//    if (!db.open()) {
//        QMessageBox::warning(nullptr, tr("Unable to open database"),
//                             tr("An error occurred while opening the connection: ") + db.lastError().text());
//        return;
//    }
    init();
//    QSqlQuery q(db);
//    q.exec("INSERT INTO Hero VALUES (NULL, '夏洛特', '战士', '物理', '七星斩');");

//    HeroInfo s;
//    s.name = "夏洛特";
//    for(int i=0;i<2;i++)
//    {
//    addHero(s);
//    }

//    getHeroCount();
//    getHeroesByPage(2,3);
//    deleteHero(18);

//    HeroInfo heroInfo;
//    updateHeroInfo(heroInfo);

    UserInfo info;
    info.username = "pikachu";
    info.password = "123456";
    info.auth = "admin";
    addUser(info);
    auto m =getAllUsers();

}

//void sqlite::init()
//{
//    if (QSqlDatabase::drivers().isEmpty())
//    qDebug()<<"No database drivers found";

//    m_db = QSqlDatabase::addDatabase("QSQLITE");

//    //获取当前路径
//    auto str = QCoreApplication::applicationDirPath();
//    qDebug()<<str;
//    m_db.setDatabaseName("F:/32keil_chenxundaimakaifa/liunx/CQt/HeroManager/data.db");
//    if (!m_db.open()) {
//        QMessageBox::warning(nullptr, tr("Unable to open database"),
//                             tr("An error occurred while opening the connection: ") + db.lastError().text());
//        return;
//    }

//}

void sqlite::init()
{
    if (QSqlDatabase::drivers().isEmpty()) {
        qDebug() << "No database drivers found";
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE");

    // 获取当前路径
#if 0
    auto str = QCoreApplication::applicationDirPath()+"data.db";
    qDebug() << str;  // 正确输出 QString 类型
#endif
    m_db.setDatabaseName("F:/32keil_chenxundaimakaifa/liunx/CQt/HeroManager/data.db");

    if (!m_db.open()) {
//        QMessageBox::warning(nullptr, tr("Unable to open database"),
//                             tr("An error occurred while opening the connection: ") + m_db.lastError().text());
       qDebug() << "db not open";
        return;
    }
}



quint32 sqlite::getHeroCount()
{
    QSqlQuery sql(m_db);
     sql.exec("select count(id) from Hero");
     quint32 uicount = 0;
     while (sql.next())
     {
        uicount = sql.value(0).toUInt();

     }
     return uicount;


}

QList<HeroInfo> sqlite::getHeroesByPage(quint32 page, quint32 uicount)
{
    QList<HeroInfo> l;
    QSqlQuery sql(m_db);
    QString strsql = QString("select * from Hero order by id limit %1 offset %2").arg(uicount).arg(page*uicount);
     sql.exec(strsql);

     HeroInfo info;
     while (sql.next())
     {
        info.id = sql.value(0).toUInt();
        info.name = sql.value(1).toString();
        info.career = sql.value(2).toString();
        info.attribute = sql.value(3).toString();
        info.skill = sql.value(4).toString();
        l.push_back(info);

     }
     return l;


}

bool sqlite::addHero(HeroInfo info)
{
    QSqlQuery sql(m_db);
    QString strSql = QString("insert into Hero values(null,'%1','%2','%3','%4')").
            arg(info.name).
            arg(info.career).
            arg(info.attribute).
            arg(info.skill);
    return sql.exec(strSql);
}

bool sqlite::deleteHero(int heroid)
{
    QSqlQuery sql(m_db);
     sql.exec(QString("delete from Hero where id = %1").arg(heroid));
     quint32 uicount = 0;
     while (sql.next())
     {
        uicount = sql.value(0).toUInt();

     }
     return uicount;
}

bool sqlite::clearHeroTable()
{
    QSqlQuery sql(m_db);
    return sql.exec("delete from Hero ");
}

bool sqlite::updateHeroInfo(HeroInfo info)
{
    QSqlQuery sql(m_db);
    QString strSql = QString("update Hero set name = '%1', career ='%2', attribute ='%3' ,skill ='%4', where id=%5;").
            arg(info.name).
            arg(info.career).
            arg(info.attribute).
            arg(info.skill).arg(info.id);
    return sql.exec(strSql);
}

QList<UserInfo> sqlite::getAllUsers()
{
    QList<UserInfo> l;
    QSqlQuery sql(m_db);
    sql.exec("select * from username");
    UserInfo info;
    while (sql.next())
    {
       info.username = sql.value(0).toString();
       info.password = sql.value(1).toString();
       info.auth = sql.value(2).toString();
       l.push_back(info);
    }
    return l;
}

bool sqlite::doesUserExist(QString struser)
{
    QSqlQuery sql(m_db);
//    sql.exec(QString(select *from username where username='1%').arg(struser));
    // 使用占位符和 arg 方法格式化 SQL 查询
    sql.exec(QString("SELECT * FROM username WHERE username LIKE '1%'").arg(struser));
    return sql.next();


}

bool sqlite::updataUserAuth(UserInfo info)
{
    QSqlQuery sql(m_db);
    QString strSql = QString("update username set password='1%',auth='2%' where username='3%'").
            arg(info.username).
            arg(info.password).
            arg(info.auth);
    return sql.exec(strSql);
}

bool sqlite::addUser(UserInfo info)
{
//    QSqlQuery sql(m_db);
//    QString strSql = QString("insert into username values ('1%', '2%', '3%')").
//            arg(info.username).
//            arg(info.password).
//            arg(info.auth);
    QSqlQuery sql(m_db);
    QString strSql = QString("insert into username values(null,'%1','%2','%3')").
            arg(info.username).
            arg(info.password).
            arg(info.auth);
    return sql.exec(strSql);
}

bool sqlite::deleteUser(QString strusername)
{
    QSqlQuery sql(m_db);
    return sql.exec(QString("delete from username where username='1%'").arg(strusername));

}

