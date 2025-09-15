#ifndef PUBLICSTRUCT_H
#define PUBLICSTRUCT_H

#include <QPoint>
#include<QString>

#define N 32
#define R 1   // 用户注册
#define L 2   // 用户登录
#define Q 3   // 单词查询
#define H 4   // 历史记录

struct Configdata
{
    QString   databaseName;
    QString   databasePass;
    int           databasePort;
    QString   mysqlServer;
    QString   databaseUser;
    int   listeningPort;
    int   sendPort;
    QString   serverIp;
    QString   sendserverIp;


};   //配置文件的结构体 包含了各种ip地址，端口号信息

enum class DatabaseType {
    MySQL,
    SQLServer,
    ODBC
};  //目前支持的数据类型

#endif // PUBLICSTRUCT_H
