/********************************************************************************
** Form generated from reading UI file 'ranklist.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RANKLIST_H
#define UI_RANKLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ranklist
{
public:
    QPushButton *pushButton;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_5;
    QListWidget *listWidget_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *label_4;
    QListWidget *listWidget;

    void setupUi(QDialog *ranklist)
    {
        if (ranklist->objectName().isEmpty())
            ranklist->setObjectName(QString::fromUtf8("ranklist"));
        ranklist->resize(1004, 800);
        pushButton = new QPushButton(ranklist);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(450, 720, 131, 61));
        QFont font;
        font.setPointSize(17);
        font.setBold(true);
        font.setWeight(75);
        pushButton->setFont(font);
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color: rgb(255, 235, 205);\n"
"border-style:solid;\n"
"border-color:black;\n"
"border-width:1px;\n"
"}\n"
"\n"
"QPushButton:hover{background-color: rgb(255, 181, 8);}"));
        layoutWidget = new QWidget(ranklist);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(670, 30, 258, 691));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setSpacing(20);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);

        verticalLayout_2->addWidget(label_5);

        listWidget_2 = new QListWidget(layoutWidget);
        listWidget_2->setObjectName(QString::fromUtf8("listWidget_2"));

        verticalLayout_2->addWidget(listWidget_2);

        widget = new QWidget(ranklist);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(90, 30, 258, 691));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);

        verticalLayout->addWidget(label_4);

        listWidget = new QListWidget(widget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);


        retranslateUi(ranklist);

        QMetaObject::connectSlotsByName(ranklist);
    } // setupUi

    void retranslateUi(QDialog *ranklist)
    {
        ranklist->setWindowTitle(QCoreApplication::translate("ranklist", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("ranklist", "\345\205\263\351\227\255", nullptr));
        label_5->setText(QCoreApplication::translate("ranklist", "\345\233\260\351\232\276\346\250\241\345\274\217\346\216\222\350\241\214\346\246\234\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("ranklist", "\346\231\256\351\200\232\346\250\241\345\274\217\346\216\222\350\241\214\346\246\234\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ranklist: public Ui_ranklist {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RANKLIST_H
