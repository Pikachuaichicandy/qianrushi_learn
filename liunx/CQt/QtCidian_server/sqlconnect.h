#ifndef SQLCONNECT_H
#define SQLCONNECT_H

#include <QObject>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QJsonObject>
#include"publicStruct.h"
#include<QLabel>
class sqlconnect : public QObject
{
    Q_OBJECT
public:
    explicit sqlconnect(QObject *parent = nullptr);
    ~sqlconnect();
    void initsqlconnect(const Configdata &mysqldata,int Num);   //数据库初始化
    void clearTable(const QString &tableName);  //清空表的内容

signals:



private:
    QSqlDatabase  m_db;

};

#endif // SQLCONNECT_H
