#include "heromanagerwidget.h"
#include "page_login.h"
#include "sqlite.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HeroManagerWidget w;
//    w.show();

    sqlite sql;
    return a.exec();
}
