/********************************************************************************
** Form generated from reading UI file 'amainwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AMAINWIDGET_H
#define UI_AMAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_amainWidget
{
public:

    void setupUi(QWidget *amainWidget)
    {
        if (amainWidget->objectName().isEmpty())
            amainWidget->setObjectName("amainWidget");
        amainWidget->resize(800, 600);

        retranslateUi(amainWidget);

        QMetaObject::connectSlotsByName(amainWidget);
    } // setupUi

    void retranslateUi(QWidget *amainWidget)
    {
        amainWidget->setWindowTitle(QCoreApplication::translate("amainWidget", "amainWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class amainWidget: public Ui_amainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AMAINWIDGET_H
