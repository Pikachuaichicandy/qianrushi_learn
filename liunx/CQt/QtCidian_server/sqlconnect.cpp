#include "sqlconnect.h"
#include<QDebug>
#include<QSqlError>
sqlconnect::sqlconnect(QObject *parent)
    : QObject{parent}
{
     qDebug()<<"目前可以连接的数据库：0为mysql 1为odbc  2为sql sever  注：mysql需自己安装驱动 ";

}

sqlconnect::~sqlconnect()
{

}

void sqlconnect::initsqlconnect(const Configdata &mysqldata, int Num)
{

    if(Num==0)
    {
        m_db = QSqlDatabase::addDatabase("QMYSQL");
        m_db.setHostName(mysqldata.mysqlServer);                             //mysqlServer
        m_db.setDatabaseName(mysqldata.databaseName);//建立数据库    //databaseName
    }
    else if(Num==1)
    {
        m_db = QSqlDatabase::addDatabase("QODBC");
        m_db.setHostName(mysqldata.mysqlServer);                             //mysqlServer
        m_db.setDatabaseName(mysqldata.databaseName);//建立数据库    //databaseName
    }
    else if (Num==2)
    {
        m_db = QSqlDatabase::addDatabase("QODBC");
        QString dsn = QString("DRIVER={SQL SERVER};SERVER=%1;DATABASE=%2").arg(mysqldata.mysqlServer).arg(mysqldata.databaseName);
        m_db.setDatabaseName(dsn);
    }
    m_db.setPort(mysqldata.databasePort);                                                //databasePort
    m_db.setUserName(mysqldata.databaseUser);                                      //databaseUser
    m_db.setPassword(mysqldata.databasePass);

    if(m_db.open() == true)
    {
        qDebug()<<" sql connect success";
    }
    else if (m_db.open() == false)
    {
        qDebug()<<m_db.lastError().text();
          return;
    }

}

void sqlconnect::clearTable(const QString &tableName)
{
    // 执行清空数据表操作
    QSqlQuery query;
    QString queryString = "DELETE FROM " + tableName;
    query.exec(queryString);

    if (query.lastError().isValid()) {
          qDebug() << "Failed to clear table:" << query.lastError().text();
    } else {
          qDebug() << "Table" << tableName << "cleared successfully.";
    }
}








