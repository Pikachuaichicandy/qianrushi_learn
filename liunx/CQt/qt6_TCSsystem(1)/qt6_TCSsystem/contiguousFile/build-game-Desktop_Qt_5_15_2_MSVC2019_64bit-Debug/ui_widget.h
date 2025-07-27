/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_8;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_9;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_5;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1004, 800);
        Widget->setMinimumSize(QSize(1004, 800));
        Widget->setMaximumSize(QSize(1004, 800));
        Widget->setStyleSheet(QString::fromUtf8("Widget{\n"
"border-width:2px;\n"
"border-color: rgb(0, 0, 0);\n"
"border-style:solid;\n"
"}\n"
""));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 98, 1004, 604));
        label->setStyleSheet(QString::fromUtf8("QLabel{\n"
"border-width:2px;\n"
"border-color: rgb(0, 0, 0);\n"
"border-style:solid;\n"
"background-color: rgb(255, 235, 205);\n"
"}"));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 0, 1004, 98));
        label_2->setStyleSheet(QString::fromUtf8("QLabel{\n"
"border-width:2px;\n"
"border-color: rgb(0, 0, 0);\n"
"border-style:solid;\n"
"border-bottom:none;\n"
"background-color:rgb(227, 227, 227);\n"
"}"));
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(0, 702, 1004, 98));
        label_3->setStyleSheet(QString::fromUtf8("QLabel{\n"
"border-width:2px;\n"
"border-color: rgb(0, 0, 0);\n"
"border-style:solid;\n"
"border-top:none;\n"
"background-color:rgb(227, 227, 227);\n"
"}"));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(110, 730, 331, 41));
        QFont font;
        font.setPointSize(17);
        label_4->setFont(font);
        label_4->setStyleSheet(QString::fromUtf8("QLabel{\n"
"color:red;\n"
"}"));
        label_5 = new QLabel(Widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(570, 730, 121, 41));
        QFont font1;
        font1.setPointSize(17);
        font1.setBold(true);
        font1.setWeight(75);
        label_5->setFont(font1);
        label_6 = new QLabel(Widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(450, 740, 71, 21));
        label_6->setFont(font1);
        label_8 = new QLabel(Widget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(0, 0, 1200, 800));
        label_8->setMinimumSize(QSize(1200, 800));
        label_8->setMaximumSize(QSize(1200, 800));
        label_8->setStyleSheet(QString::fromUtf8("QLabel{background-color:snow}"));
        layoutWidget = new QWidget(Widget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 1006, 801));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalSpacer_2 = new QSpacerItem(20, 150, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_4->addItem(verticalSpacer_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_5 = new QSpacerItem(450, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        label_9 = new QLabel(layoutWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(100, 50));
        label_9->setMaximumSize(QSize(100, 50));
        QFont font2;
        font2.setPointSize(25);
        font2.setBold(true);
        font2.setWeight(75);
        label_9->setFont(font2);
        label_9->setStyleSheet(QString::fromUtf8("QLabel{color:green;}"));

        horizontalLayout_4->addWidget(label_9);

        horizontalSpacer_6 = new QSpacerItem(450, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);


        verticalLayout_4->addLayout(horizontalLayout_4);

        verticalSpacer_4 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_4->addItem(verticalSpacer_4);


        verticalLayout_3->addLayout(verticalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(200, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(0, 60));
        QFont font3;
        font3.setPointSize(15);
        font3.setBold(true);
        font3.setWeight(75);
        pushButton->setFont(font3);
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color: rgb(255, 235, 205);\n"
"border-style:solid;\n"
"border-color:black;\n"
"border-width:1px;\n"
"}\n"
"\n"
"QPushButton:hover{background-color: rgb(255, 181, 8);}"));

        verticalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(layoutWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(0, 60));
        pushButton_2->setFont(font3);
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color: rgb(255, 235, 205);\n"
"border-style:solid;\n"
"border-color:black;\n"
"border-width:1px;\n"
"}\n"
"\n"
"QPushButton:hover{background-color: rgb(255, 181, 8);}"));

        verticalLayout->addWidget(pushButton_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(20);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_3 = new QPushButton(layoutWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setMinimumSize(QSize(0, 60));
        pushButton_3->setFont(font3);
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color: rgb(255, 235, 205);\n"
"border-style:solid;\n"
"border-color:black;\n"
"border-width:1px;\n"
"}\n"
"\n"
"QPushButton:hover{background-color: rgb(255, 181, 8);}"));

        horizontalLayout->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(layoutWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setMinimumSize(QSize(0, 60));
        pushButton_4->setFont(font3);
        pushButton_4->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color: rgb(255, 235, 205);\n"
"border-style:solid;\n"
"border-color:black;\n"
"border-width:1px;\n"
"}\n"
"\n"
"QPushButton:hover{background-color: rgb(255, 181, 8);}"));

        horizontalLayout->addWidget(pushButton_4);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);

        horizontalSpacer_2 = new QSpacerItem(200, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalSpacer_5 = new QSpacerItem(20, 300, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer_5);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label->setText(QString());
        label_2->setText(QString());
        label_3->setText(QString());
        label_4->setText(QString());
        label_5->setText(QString());
        label_6->setText(QString());
        label_8->setText(QString());
        label_9->setText(QCoreApplication::translate("Widget", "\350\264\252\345\220\203\350\233\207", nullptr));
        pushButton->setText(QCoreApplication::translate("Widget", "\346\231\256\351\200\232\346\250\241\345\274\217", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Widget", "\345\233\260\351\232\276\346\250\241\345\274\217", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Widget", "\351\200\200\345\207\272\346\270\270\346\210\217", nullptr));
        pushButton_4->setText(QCoreApplication::translate("Widget", "\346\216\222\350\241\214\346\246\234", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
