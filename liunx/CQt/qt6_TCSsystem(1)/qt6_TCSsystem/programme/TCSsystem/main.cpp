#include "amainwidget.h"
#include "gamewidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // amainWidget w;
    // w.show();
    GameWidget w;
    w.show();
    return a.exec();
}
