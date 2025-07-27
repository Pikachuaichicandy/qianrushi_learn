/********************************************************************************
** Form generated from reading UI file 'moneyup.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MONEYUP_H
#define UI_MONEYUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_moneyUp
{
public:
    QLabel *label;

    void setupUi(QWidget *moneyUp)
    {
        if (moneyUp->objectName().isEmpty())
            moneyUp->setObjectName(QString::fromUtf8("moneyUp"));
        moneyUp->resize(150, 40);
        moneyUp->setMinimumSize(QSize(150, 40));
        moneyUp->setMaximumSize(QSize(150, 40));
        label = new QLabel(moneyUp);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 150, 40));
        label->setMinimumSize(QSize(150, 40));
        label->setMaximumSize(QSize(150, 40));
        QFont font;
        font.setPointSize(17);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("QLabel{color:yellow}"));

        retranslateUi(moneyUp);

        QMetaObject::connectSlotsByName(moneyUp);
    } // setupUi

    void retranslateUi(QWidget *moneyUp)
    {
        moneyUp->setWindowTitle(QCoreApplication::translate("moneyUp", "Form", nullptr));
        label->setText(QCoreApplication::translate("moneyUp", "0\344\270\207\345\205\203", nullptr));
    } // retranslateUi

};

namespace Ui {
    class moneyUp: public Ui_moneyUp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MONEYUP_H
