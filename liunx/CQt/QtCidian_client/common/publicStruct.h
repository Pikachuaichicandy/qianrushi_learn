#ifndef PUBLICSTRUCT_H
#define PUBLICSTRUCT_H

#include <QPoint>
#include<QString>

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
