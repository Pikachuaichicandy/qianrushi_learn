/********************************************************************************
** Form generated from reading UI file 'snake.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SNAKE_H
#define UI_SNAKE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_snake
{
public:
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *pushButton_5;

    void setupUi(QWidget *snake)
    {
        if (snake->objectName().isEmpty())
            snake->setObjectName(QString::fromUtf8("snake"));
        snake->resize(1000, 600);
        snake->setStyleSheet(QString::fromUtf8(""));
        label = new QLabel(snake);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(800, 0, 200, 600));
        label->setStyleSheet(QString::fromUtf8("QLabel{\n"
"background-color:rgb(227, 227, 227);\n"
"border-style:solid;\n"
"border-width:2px;\n"
"border-right:none;\n"
"border-top:none;\n"
"border-bottom:none;\n"
"}"));
        pushButton = new QPushButton(snake);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(860, 300, 80, 50));
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        font.setWeight(75);
        pushButton->setFont(font);
        pushButton->setFocusPolicy(Qt::StrongFocus);
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color: rgb(255, 235, 205);\n"
"border-style:solid;\n"
"border-color:black;\n"
"border-width:1px;\n"
"}\n"
"\n"
"QPushButton:hover{background-color: rgb(255, 181, 8);}"));
        pushButton_2 = new QPushButton(snake);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(860, 430, 80, 50));
        pushButton_2->setFont(font);
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color: rgb(255, 235, 205);\n"
"border-style:solid;\n"
"border-color:black;\n"
"border-width:1px;\n"
"}\n"
"\n"
"QPushButton:hover{background-color: rgb(255, 181, 8);}"));
        pushButton_3 = new QPushButton(snake);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(940, 350, 50, 80));
        pushButton_3->setFont(font);
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color: rgb(255, 235, 205);\n"
"border-style:solid;\n"
"border-color:black;\n"
"border-width:1px;\n"
"}\n"
"\n"
"QPushButton:hover{background-color: rgb(255, 181, 8);}"));
        pushButton_4 = new QPushButton(snake);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(810, 350, 50, 80));
        pushButton_4->setFont(font);
        pushButton_4->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color: rgb(255, 235, 205);\n"
"border-style:solid;\n"
"border-color:black;\n"
"border-width:1px;\n"
"}\n"
"\n"
"QPushButton:hover{background-color: rgb(255, 181, 8);}"));
        label_2 = new QLabel(snake);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(840, 60, 130, 50));
        QFont font1;
        font1.setPointSize(17);
        font1.setBold(true);
        font1.setWeight(75);
        label_2->setFont(font1);
        label_2->setStyleSheet(QString::fromUtf8("QLabel{background-color: transparent}"));
        label_3 = new QLabel(snake);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(880, 110, 60, 40));
        label_3->setFont(font1);
        label_3->setStyleSheet(QString::fromUtf8("QLabel{background-color: transparent}"));
        pushButton_5 = new QPushButton(snake);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(860, 550, 75, 41));
        pushButton_5->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color: rgb(255, 235, 205);\n"
"border-style:solid;\n"
"border-color:black;\n"
"border-width:1px;\n"
"}\n"
"\n"
"QPushButton:hover{background-color: rgb(255, 181, 8);}"));

        retranslateUi(snake);

        QMetaObject::connectSlotsByName(snake);
    } // setupUi

    void retranslateUi(QWidget *snake)
    {
        snake->setWindowTitle(QCoreApplication::translate("snake", "Form", nullptr));
        label->setText(QString());
        pushButton->setText(QCoreApplication::translate("snake", "\344\270\212", nullptr));
        pushButton_2->setText(QCoreApplication::translate("snake", "\344\270\213", nullptr));
        pushButton_3->setText(QCoreApplication::translate("snake", "\345\217\263", nullptr));
        pushButton_4->setText(QCoreApplication::translate("snake", "\345\267\246", nullptr));
        label_2->setText(QCoreApplication::translate("snake", "\345\275\223\345\211\215\345\276\227\345\210\206\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("snake", "0", nullptr));
        pushButton_5->setText(QCoreApplication::translate("snake", "\347\273\223\346\235\237\346\270\270\346\210\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class snake: public Ui_snake {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SNAKE_H
