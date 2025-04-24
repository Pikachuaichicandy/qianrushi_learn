#include "mywidget.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWidget w;
    mainwindow Y;
    Y.show();
//    w.show();

    return a.exec();
}
