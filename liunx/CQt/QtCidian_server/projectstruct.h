#ifndef PROJECTSTRUCT_H
#define PROJECTSTRUCT_H

#include<QString>
#include <QMetaType>

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QVector>
#include <QCoreApplication>
#include <QFile>
#include <QDir>
struct  StudentInformationData
{
    int          serialnumber;  //序号
    QString  studentid;  //学号
    QString  name;//姓名
    QString  college;//学院
    QString  major;//专业
    QString  grade;//年级
    QString  studentclass;//班级
    QString  studentage;  //年龄
    QString  education; //教育经历
    QString  imagepath; //图片地址
};

struct MSGInfo{
    int type;               // 消息类型（R/L/Q/H）
//    char name[N];           // 用户名
//    char data[256];         // 数据（密码/单词/查询结果）
    QString name;
    int pass;
};




#endif // PROJECTSTRUCT_H
