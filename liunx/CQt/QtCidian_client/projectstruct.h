#ifndef PROJECTSTRUCT_H
#define PROJECTSTRUCT_H

#include<QString>
#include <QMetaType>
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




#endif // PROJECTSTRUCT_H
