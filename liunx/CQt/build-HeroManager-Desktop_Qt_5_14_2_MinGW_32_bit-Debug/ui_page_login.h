/********************************************************************************
** Form generated from reading UI file 'page_login.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGE_LOGIN_H
#define UI_PAGE_LOGIN_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Page_login
{
public:
    QLabel *label_3;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_login;
    QPushButton *btn_exit;

    void setupUi(QWidget *Page_login)
    {
        if (Page_login->objectName().isEmpty())
            Page_login->setObjectName(QString::fromUtf8("Page_login"));
        Page_login->resize(400, 300);
        Page_login->setMaximumSize(QSize(400, 300));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/pikachuya.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        Page_login->setWindowIcon(icon);
        Page_login->setStyleSheet(QString::fromUtf8(""));
        label_3 = new QLabel(Page_login);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(140, 20, 121, 16));
        widget = new QWidget(Page_login);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(80, 70, 240, 66));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMaxLength(20);
        lineEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        lineEdit_2 = new QLineEdit(widget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setMaxLength(20);
        lineEdit_2->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(lineEdit_2, 1, 1, 1, 1);

        widget_2 = new QWidget(Page_login);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(110, 160, 210, 46));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btn_login = new QPushButton(widget_2);
        btn_login->setObjectName(QString::fromUtf8("btn_login"));

        horizontalLayout->addWidget(btn_login);

        btn_exit = new QPushButton(widget_2);
        btn_exit->setObjectName(QString::fromUtf8("btn_exit"));

        horizontalLayout->addWidget(btn_exit);


        retranslateUi(Page_login);

        QMetaObject::connectSlotsByName(Page_login);
    } // setupUi

    void retranslateUi(QWidget *Page_login)
    {
        Page_login->setWindowTitle(QCoreApplication::translate("Page_login", "\347\231\273\345\275\225", nullptr));
        label_3->setText(QCoreApplication::translate("Page_login", "\347\216\213\350\200\205\350\213\261\351\233\204\347\256\241\347\220\206\347\263\273\347\273\237", nullptr));
        label->setText(QCoreApplication::translate("Page_login", "\347\224\250\346\210\267\345\220\215", nullptr));
        label_2->setText(QCoreApplication::translate("Page_login", "\345\257\206\347\240\201", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("Page_login", "\350\257\267\350\276\223\345\205\245", nullptr));
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("Page_login", "\350\257\267\350\276\223\345\205\245", nullptr));
        btn_login->setText(QCoreApplication::translate("Page_login", "\347\231\273\345\275\225", nullptr));
        btn_exit->setText(QCoreApplication::translate("Page_login", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Page_login: public Ui_Page_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGE_LOGIN_H
