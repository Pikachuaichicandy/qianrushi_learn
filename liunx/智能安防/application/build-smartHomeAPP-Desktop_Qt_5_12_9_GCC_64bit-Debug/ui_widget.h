/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *wendu;
    QLabel *shidu;
    QLabel *shifouyouren;
    QLabel *juli;
    QLabel *dengzhuangtai;
    QLabel *fengshanzhuangtai;
    QPushButton *deng;
    QPushButton *fengshan;
    QLabel *guangzhao;
    QPushButton *close;
    QLabel *video;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(801, 475);
        QFont font;
        font.setFamily(QString::fromUtf8("Abyssinica SIL"));
        Widget->setFont(font);
        wendu = new QLabel(Widget);
        wendu->setObjectName(QString::fromUtf8("wendu"));
        wendu->setGeometry(QRect(230, 360, 161, 51));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Ubuntu"));
        font1.setPointSize(22);
        wendu->setFont(font1);
        wendu->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #1E90FF, \n"
"        stop:1 #87CEFA\n"
"    );\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}"));
        wendu->setAlignment(Qt::AlignCenter);
        shidu = new QLabel(Widget);
        shidu->setObjectName(QString::fromUtf8("shidu"));
        shidu->setGeometry(QRect(400, 360, 151, 51));
        shidu->setFont(font1);
        shidu->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #1E90FF, \n"
"        stop:1 #87CEFA\n"
"    );\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}"));
        shidu->setAlignment(Qt::AlignCenter);
        shifouyouren = new QLabel(Widget);
        shifouyouren->setObjectName(QString::fromUtf8("shifouyouren"));
        shifouyouren->setGeometry(QRect(600, 360, 201, 51));
        shifouyouren->setFont(font1);
        shifouyouren->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #1E90FF, \n"
"        stop:1 #87CEFA\n"
"    );\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}"));
        shifouyouren->setAlignment(Qt::AlignCenter);
        juli = new QLabel(Widget);
        juli->setObjectName(QString::fromUtf8("juli"));
        juli->setGeometry(QRect(600, 420, 201, 51));
        juli->setFont(font1);
        juli->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #1E90FF, \n"
"        stop:1 #87CEFA\n"
"    );\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}"));
        juli->setAlignment(Qt::AlignCenter);
        dengzhuangtai = new QLabel(Widget);
        dengzhuangtai->setObjectName(QString::fromUtf8("dengzhuangtai"));
        dengzhuangtai->setGeometry(QRect(0, 360, 201, 51));
        dengzhuangtai->setFont(font1);
        dengzhuangtai->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #1E90FF, \n"
"        stop:1 #87CEFA\n"
"    );\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}"));
        dengzhuangtai->setAlignment(Qt::AlignCenter);
        fengshanzhuangtai = new QLabel(Widget);
        fengshanzhuangtai->setObjectName(QString::fromUtf8("fengshanzhuangtai"));
        fengshanzhuangtai->setGeometry(QRect(0, 420, 201, 51));
        fengshanzhuangtai->setFont(font1);
        fengshanzhuangtai->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #1E90FF, \n"
"        stop:1 #87CEFA\n"
"    );\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}"));
        fengshanzhuangtai->setAlignment(Qt::AlignCenter);
        deng = new QPushButton(Widget);
        deng->setObjectName(QString::fromUtf8("deng"));
        deng->setGeometry(QRect(650, 10, 151, 51));
        QFont font2;
        font2.setBold(true);
        font2.setWeight(75);
        deng->setFont(font2);
        deng->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                               stop:0 #f7b2bd, stop:1 #e91e63);\n"
"    border: 2px solid #e91e63;\n"
"    border-radius: 10px;\n"
"    color: white;\n"
"    font-size: 22px;\n"
"    font-weight: bold;\n"
"    padding: 8px 16px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                               stop:0 #f8c2cc, stop:1 #f06292);\n"
"    border: 2px solid #f06292;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                               stop:0 #e91e63, stop:1 #c2185b);\n"
"}"));
        fengshan = new QPushButton(Widget);
        fengshan->setObjectName(QString::fromUtf8("fengshan"));
        fengshan->setGeometry(QRect(650, 80, 151, 51));
        fengshan->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                               stop:0 #f7b2bd, stop:1 #e91e63);\n"
"    border: 2px solid #e91e63;\n"
"    border-radius: 10px;\n"
"    color: white;\n"
"    font-size: 22px;\n"
"    font-weight: bold;\n"
"    padding: 8px 16px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                               stop:0 #f8c2cc, stop:1 #f06292);\n"
"    border: 2px solid #f06292;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                               stop:0 #e91e63, stop:1 #c2185b);\n"
"}"));
        guangzhao = new QLabel(Widget);
        guangzhao->setObjectName(QString::fromUtf8("guangzhao"));
        guangzhao->setGeometry(QRect(230, 420, 321, 51));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Ubuntu"));
        font3.setPointSize(20);
        guangzhao->setFont(font3);
        guangzhao->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #1E90FF, \n"
"        stop:1 #87CEFA\n"
"    );\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}"));
        guangzhao->setAlignment(Qt::AlignCenter);
        close = new QPushButton(Widget);
        close->setObjectName(QString::fromUtf8("close"));
        close->setGeometry(QRect(650, 150, 151, 51));
        close->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                               stop:0 #f7b2bd, stop:1 #e91e63);\n"
"    border: 2px solid #e91e63;\n"
"    border-radius: 10px;\n"
"    color: white;\n"
"    font-size: 22px;\n"
"    font-weight: bold;\n"
"    padding: 8px 16px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                               stop:0 #f8c2cc, stop:1 #f06292);\n"
"    border: 2px solid #f06292;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                               stop:0 #e91e63, stop:1 #c2185b);\n"
"}"));
        video = new QLabel(Widget);
        video->setObjectName(QString::fromUtf8("video"));
        video->setGeometry(QRect(0, 0, 641, 351));
        QFont font4;
        font4.setPointSize(36);
        video->setFont(font4);
        video->setAlignment(Qt::AlignCenter);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        wendu->setText(QApplication::translate("Widget", "0", nullptr));
        shidu->setText(QApplication::translate("Widget", "0", nullptr));
        shifouyouren->setText(QApplication::translate("Widget", "\346\227\240\344\272\272", nullptr));
        juli->setText(QApplication::translate("Widget", "0", nullptr));
        dengzhuangtai->setText(QApplication::translate("Widget", "\347\201\257\347\212\266\346\200\201:\347\201\255", nullptr));
        fengshanzhuangtai->setText(QApplication::translate("Widget", "\351\243\216\346\211\207\347\212\266\346\200\201:\345\201\234\346\255\242", nullptr));
        deng->setText(QApplication::translate("Widget", "\345\274\200\347\201\257", nullptr));
        fengshan->setText(QApplication::translate("Widget", "\345\274\200\351\243\216\346\211\207", nullptr));
        guangzhao->setText(QApplication::translate("Widget", "0", nullptr));
        close->setText(QApplication::translate("Widget", "\344\270\200\351\224\256\345\205\263\351\227\255", nullptr));
        video->setText(QApplication::translate("Widget", "\345\233\276\345\203\217\345\214\272\345\237\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
