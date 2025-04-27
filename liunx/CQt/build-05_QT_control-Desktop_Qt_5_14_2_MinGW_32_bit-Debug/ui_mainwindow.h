/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QRadioButton *rbtnman;
    QRadioButton *rbtnwoman;
    QWidget *cbox;
    QCheckBox *cbok1;
    QCheckBox *cbok2;
    QListWidget *listWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        rbtnman = new QRadioButton(centralwidget);
        rbtnman->setObjectName(QString::fromUtf8("rbtnman"));
        rbtnman->setGeometry(QRect(80, 240, 115, 19));
        rbtnwoman = new QRadioButton(centralwidget);
        rbtnwoman->setObjectName(QString::fromUtf8("rbtnwoman"));
        rbtnwoman->setGeometry(QRect(80, 280, 115, 19));
        cbox = new QWidget(centralwidget);
        cbox->setObjectName(QString::fromUtf8("cbox"));
        cbox->setGeometry(QRect(290, 330, 120, 80));
        cbok1 = new QCheckBox(cbox);
        cbok1->setObjectName(QString::fromUtf8("cbok1"));
        cbok1->setGeometry(QRect(20, 20, 91, 19));
        cbok2 = new QCheckBox(cbox);
        cbok2->setObjectName(QString::fromUtf8("cbok2"));
        cbok2->setGeometry(QRect(20, 50, 91, 19));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(390, 130, 256, 192));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        rbtnman->setText(QCoreApplication::translate("MainWindow", "man", nullptr));
        rbtnwoman->setText(QCoreApplication::translate("MainWindow", "women", nullptr));
        cbok1->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        cbok2->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
