/********************************************************************************
** Form generated from reading UI file 'movetext.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOVETEXT_H
#define UI_MOVETEXT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_movetext
{
public:

    void setupUi(QWidget *movetext)
    {
        if (movetext->objectName().isEmpty())
            movetext->setObjectName(QString::fromUtf8("movetext"));
        movetext->resize(1004, 98);
        movetext->setMinimumSize(QSize(1004, 98));
        movetext->setMaximumSize(QSize(1004, 98));

        retranslateUi(movetext);

        QMetaObject::connectSlotsByName(movetext);
    } // setupUi

    void retranslateUi(QWidget *movetext)
    {
        movetext->setWindowTitle(QCoreApplication::translate("movetext", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class movetext: public Ui_movetext {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOVETEXT_H
