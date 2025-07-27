#include "login.h"
#include"struct.h"
#include <QApplication>
#include <QMetaType>

int main(int argc, char *argv[])
{
    // 在主函数中注册 'Point' 类型
    qRegisterMetaType<Point>("Point");
    QApplication a(argc, argv);
    Login w;
    w.show();
    return a.exec();
}
