#ifndef SQLDATASOLUTION_H
#define SQLDATASOLUTION_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QVector>
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include"projectstruct.h"
class sqlDataSolution : public QObject
{
    Q_OBJECT
public:
    explicit sqlDataSolution(QObject *parent = nullptr);
    ~sqlDataSolution();

    inline  QVector<StudentInformationData>  getStudentInfo()   const {  return   StudentInfo;}
    inline  int  getAllStudentInfoNum() const {int num;     num=StudentInfo.size();return  num;}

    //创建一张表，并查看是否创建成功
    bool executeSQL(const QString &sql);

    //存储所有的学生数据到数据结构
    void  saveAllStudentInfo();

    //增加一条信息
    void  addOneStudentInfo(StudentInformationData data);

    //修改一条信息
    void  modifyOneStudentInfo(StudentInformationData data);

    //删除一条信息
    void  deleteOneStudentInfo(QString  studentid);

    //查询一条信息
   StudentInformationData    selectOneStudentInfo(QString  studentid);

private:
     QVector<StudentInformationData>  StudentInfo;
     void   initCreateTabel();  //初始化


signals:



};

#endif // SQLDATASOLUTION_H
