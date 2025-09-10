#include "sqldatasolution.h"

sqlDataSolution::sqlDataSolution(QObject *parent)
    : QObject{parent}
{
    initCreateTabel();
}

sqlDataSolution::~sqlDataSolution()  //析构函数
{

}

bool sqlDataSolution::executeSQL(const QString &sql)
{
    // 执行 SQL 语句
    QSqlQuery query;
    if (!query.exec(sql)) {
        qDebug() << "执行 SQL 失败：" << query.lastError().text();
        return false;
    }

    qDebug() << "SQL 执行成功";
    return true;
}



void sqlDataSolution::initCreateTabel()   //初始化创建表格
{
    QString createUsersTableSql = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INT PRIMARY KEY AUTO_INCREMENT,
            student_id VARCHAR(255) NOT NULL,
            name VARCHAR(255) NOT NULL,
            password  VARCHAR(255) NOT NULL,
            role INT NOT NULL
        )
    )";

    QString createStudentInfoTableSql = R"(
        CREATE TABLE IF NOT EXISTS studentinfo (
            id INT PRIMARY KEY AUTO_INCREMENT,
            student_id VARCHAR(255) UNIQUE NOT NULL,
            name    VARCHAR(255) NOT NULL,
            college  VARCHAR(255) NOT NULL,
            major    VARCHAR(255) NOT NULL,
            grade    VARCHAR(255) NOT NULL,
            class     VARCHAR(255) NOT NULL,
            age       VARCHAR(255) NOT NULL,
            education  VARCHAR(500) NOT NULL,
            imagepath VARCHAR(500) NULL
        )
    )";

    if (executeSQL(createUsersTableSql)&&executeSQL(createStudentInfoTableSql)) {
        qDebug() << "用户表,学生信息表创建成功";
    } else {
        qDebug() << "用户表,学生信息表创建失败";
    }

     saveAllStudentInfo();
}


void sqlDataSolution::saveAllStudentInfo()
{
    // 清空当前数据
    StudentInfo.clear();

    // 创建 SQL 查询对象
    QSqlQuery query;

    // 查询所有数据
    QString sql = "SELECT id, student_id, name, college, major, grade, class, age, education,imagepath FROM studentinfo";
    if (!query.exec(sql)) {
        qWarning() << "Failed to execute query:" << query.lastError().text();
        return;
    }

    // 遍历查询结果
    while (query.next()) {
        StudentInformationData student;

        // 从结果集中读取数据并赋值到结构体
        student.serialnumber = query.value("id").toInt();
        student.studentid = query.value("student_id").toString();
        student.name = query.value("name").toString();
        student.college = query.value("college").toString();
        student.major = query.value("major").toString();
        student.grade = query.value("grade").toString();
        student.studentclass = query.value("class").toString();
        student.studentage = query.value("age").toString();
        student.education = query.value("education").toString();
        student.imagepath = query.value("imagepath").toString();


        // 将结构体添加到 QVector 中
        StudentInfo.append(student);
    }

    qDebug() << "Successfully loaded all student information."<<StudentInfo.size();

//    for (const auto& student : StudentInfo) {
//        qDebug() << "Serial Number:" << student.serialnumber
//                 << ", Student ID:" << student.studentid
//                 << ", Name:" << student.name
//                 << ", College:" << student.college
//                 << ", Major:" << student.major
//                 << ", Grade:" << student.grade
//                 << ", Class:" << student.studentclass
//                 << ", Age:" << student.studentage
//                 << ", Education:" << student.education;
//    }



}


void sqlDataSolution::addOneStudentInfo(StudentInformationData data)
{
    // 创建 SQL 查询对象


    QSqlQuery query;

    // 插入数据的 SQL 语句
    QString sql = R"(
        INSERT INTO studentinfo (id,student_id, name, college, major, grade, class, age, education,imagepath)
        VALUES (:id, :student_id, :name, :college, :major, :grade, :class, :age, :education, :imagepath)
    )";

    query.prepare(sql);

    // 绑定参数
    query.bindValue(":id", data.serialnumber);
    query.bindValue(":student_id", data.studentid);
    query.bindValue(":name", data.name);
    query.bindValue(":college", data.college);
    query.bindValue(":major", data.major);
    query.bindValue(":grade", data.grade);
    query.bindValue(":class", data.studentclass);
    query.bindValue(":age", data.studentage);
    query.bindValue(":education", data.education);
    query.bindValue(":imagepath", data.imagepath);

    // 执行查询
    if (!query.exec()) {
        qWarning() << "Failed to insert data:" << query.lastError().text();
        return;
    }

    // 获取插入数据的 ID（自增主键值）
    //data.serialnumber = query.lastInsertId().toInt();

//    QString resetSql = R"(
//        SET @i := 0;
//        UPDATE studentinfo SET id = (@i := @i + 1);
//    )";

    // 将数据添加到 StudentInfo QVector 中
    StudentInfo.append(data);

    qDebug() << "Successfully added a new student:" << data.name<<data.serialnumber;

    this->saveAllStudentInfo();
}

void sqlDataSolution::modifyOneStudentInfo(StudentInformationData data)
{
    QSqlQuery query;

    // 更新数据的 SQL 语句，根据 student_id 进行匹配
    QString sql = R"(
        UPDATE studentinfo
        SET name = :name,
            college = :college,
            major = :major,
            grade = :grade,
            class = :class,
            age = :age,
            education = :education,
            imagepath= :imagepath
        WHERE student_id = :student_id
    )";

    query.prepare(sql);

    // 绑定参数
    query.bindValue(":student_id", data.studentid);
    query.bindValue(":name", data.name);
    query.bindValue(":college", data.college);
    query.bindValue(":major", data.major);
    query.bindValue(":grade", data.grade);
    query.bindValue(":class", data.studentclass);
    query.bindValue(":age", data.studentage);
    query.bindValue(":education", data.education);
    query.bindValue(":imagepath", data.imagepath);


    // 执行更新操作
    if (!query.exec()) {
        qWarning() << "Failed to update data:" << query.lastError().text();
        return;
    }

    this->saveAllStudentInfo();
    // 更新本地数据


}

void sqlDataSolution::deleteOneStudentInfo(QString studentid)
{
    QSqlQuery query;

    // 删除数据的 SQL 语句，根据 student_id 进行匹配
    QString sql = R"(
        DELETE FROM studentinfo
        WHERE student_id = :student_id
    )";

    query.prepare(sql);

    // 绑定参数
    query.bindValue(":student_id", studentid);

    // 执行删除操作
    if (!query.exec()) {
        qWarning() << "Failed to delete student data:" << query.lastError().text();
        return;
    }


    // 更新数据库中剩余学生的 id 以便按顺序重新排序
    QString resetSql = R"(
        SET @i := 0;
        UPDATE studentinfo SET id = (@i := @i + 1);
    )";

    if (!query.exec(resetSql)) {
        qWarning() << "Failed to reset id order:" << query.lastError().text();
    }

    // 重新加载数据库中所有的学生信息，并更新 StudentInfo
    saveAllStudentInfo();  // 重新加载所有学生数据

    // 输出排序后的信息
    qDebug() << "Student list sorted by ID.";

    //删除图片

    QString exePath = QCoreApplication::applicationDirPath();

    // 根据 studentid 拼接文件路径
    QString filePath = QDir(exePath).filePath("studentimage/" + studentid + ".png");

    // 检查文件是否存在
    if (QFile::exists(filePath)) {
        // 删除文件
        if (QFile::remove(filePath)) {
            qDebug() << "image deleted successfully:" << filePath;
        } else {
            qDebug() << "Failed to delete the image:" << filePath;
        }
    } else {
        qDebug() << "image does not exist:" << filePath;
    }
}



StudentInformationData    sqlDataSolution::selectOneStudentInfo(QString  studentid)
{
      StudentInformationData studentInfo;
    // 准备查询语句
    QString queryStr = R"(
        SELECT id, student_id, name, college, major, grade, class, age, education, imagepath
        FROM studentinfo
        WHERE student_id = :studentid
    )";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":studentid", studentid);

    // 执行查询
    if (query.exec() && query.next()) {
        // 填充结果到结构体
        studentInfo.serialnumber = query.value("id").toInt();
        studentInfo.studentid = query.value("student_id").toString();
        studentInfo.name = query.value("name").toString();
        studentInfo.college = query.value("college").toString();
        studentInfo.major = query.value("major").toString();
        studentInfo.grade = query.value("grade").toString();
        studentInfo.studentclass = query.value("class").toString();
        studentInfo.studentage = query.value("age").toString();
        studentInfo.education = query.value("education").toString();
        studentInfo.imagepath = query.value("imagepath").toString();
    } else {
        qDebug()  << "查询失败或无结果：" << query.lastError().text();
    }

    return studentInfo;
}
