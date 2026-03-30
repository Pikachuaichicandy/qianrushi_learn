#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QLabel label("LCD Test - Hello World!");
    label.setStyleSheet("QLabel { background-color: white; color: black; font-size: 20px; }");
    label.resize(480, 272);
    label.show();
    
    return app.exec();
}