/********************************************************************************
** Form generated from reading UI file 'gamewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEWIDGET_H
#define UI_GAMEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameWidget
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLCDNumber *lcdNumber;
    QPushButton *start;
    QPushButton *pause;
    QPushButton *over;
    QListView *listView;

    void setupUi(QWidget *GameWidget)
    {
        if (GameWidget->objectName().isEmpty())
            GameWidget->setObjectName(QString::fromUtf8("GameWidget"));
        GameWidget->resize(1118, 600);
        widget = new QWidget(GameWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(810, 10, 274, 351));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        lcdNumber = new QLCDNumber(widget);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));

        verticalLayout->addWidget(lcdNumber);

        start = new QPushButton(widget);
        start->setObjectName(QString::fromUtf8("start"));

        verticalLayout->addWidget(start);

        pause = new QPushButton(widget);
        pause->setObjectName(QString::fromUtf8("pause"));

        verticalLayout->addWidget(pause);

        over = new QPushButton(widget);
        over->setObjectName(QString::fromUtf8("over"));

        verticalLayout->addWidget(over);

        listView = new QListView(widget);
        listView->setObjectName(QString::fromUtf8("listView"));

        verticalLayout->addWidget(listView);


        retranslateUi(GameWidget);

        QMetaObject::connectSlotsByName(GameWidget);
    } // setupUi

    void retranslateUi(QWidget *GameWidget)
    {
        GameWidget->setWindowTitle(QCoreApplication::translate("GameWidget", "GameWidget", nullptr));
        label->setText(QCoreApplication::translate("GameWidget", "    \345\275\223\345\211\215\345\210\206\346\225\260\357\274\232", nullptr));
        start->setText(QCoreApplication::translate("GameWidget", "\345\274\200\345\247\213", nullptr));
        pause->setText(QCoreApplication::translate("GameWidget", "\346\232\202\345\201\234/\345\274\200\345\247\213", nullptr));
        over->setText(QCoreApplication::translate("GameWidget", "\347\273\223\346\235\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GameWidget: public Ui_GameWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEWIDGET_H
