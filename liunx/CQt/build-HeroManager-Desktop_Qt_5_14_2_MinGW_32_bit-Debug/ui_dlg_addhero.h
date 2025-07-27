/********************************************************************************
** Form generated from reading UI file 'dlg_addhero.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLG_ADDHERO_H
#define UI_DLG_ADDHERO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dlg_AddHero
{
public:
    QWidget *widget;
    QGridLayout *gridLayout;
    QPushButton *btn_save;
    QPushButton *btn_cancel;
    QWidget *widget_2;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QLineEdit *le_name;
    QLabel *label_2;
    QLineEdit *le_career;
    QLabel *label_3;
    QLineEdit *le_attribute;
    QLabel *label_4;
    QLineEdit *le_skill;

    void setupUi(QDialog *Dlg_AddHero)
    {
        if (Dlg_AddHero->objectName().isEmpty())
            Dlg_AddHero->setObjectName(QString::fromUtf8("Dlg_AddHero"));
        Dlg_AddHero->resize(400, 300);
        widget = new QWidget(Dlg_AddHero);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(100, 160, 210, 46));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        btn_save = new QPushButton(widget);
        btn_save->setObjectName(QString::fromUtf8("btn_save"));

        gridLayout->addWidget(btn_save, 0, 0, 1, 1);

        btn_cancel = new QPushButton(widget);
        btn_cancel->setObjectName(QString::fromUtf8("btn_cancel"));

        gridLayout->addWidget(btn_cancel, 0, 1, 1, 1);

        widget_2 = new QWidget(Dlg_AddHero);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(80, 30, 231, 120));
        gridLayout_2 = new QGridLayout(widget_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(widget_2);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        le_name = new QLineEdit(widget_2);
        le_name->setObjectName(QString::fromUtf8("le_name"));

        gridLayout_2->addWidget(le_name, 0, 1, 1, 2);

        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 1, 0, 1, 2);

        le_career = new QLineEdit(widget_2);
        le_career->setObjectName(QString::fromUtf8("le_career"));

        gridLayout_2->addWidget(le_career, 1, 2, 1, 1);

        label_3 = new QLabel(widget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 2, 0, 1, 1);

        le_attribute = new QLineEdit(widget_2);
        le_attribute->setObjectName(QString::fromUtf8("le_attribute"));

        gridLayout_2->addWidget(le_attribute, 2, 1, 1, 2);

        label_4 = new QLabel(widget_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 3, 0, 1, 1);

        le_skill = new QLineEdit(widget_2);
        le_skill->setObjectName(QString::fromUtf8("le_skill"));

        gridLayout_2->addWidget(le_skill, 3, 1, 1, 2);

        QWidget::setTabOrder(le_name, le_career);
        QWidget::setTabOrder(le_career, le_attribute);
        QWidget::setTabOrder(le_attribute, le_skill);
        QWidget::setTabOrder(le_skill, btn_save);
        QWidget::setTabOrder(btn_save, btn_cancel);

        retranslateUi(Dlg_AddHero);

        QMetaObject::connectSlotsByName(Dlg_AddHero);
    } // setupUi

    void retranslateUi(QDialog *Dlg_AddHero)
    {
        Dlg_AddHero->setWindowTitle(QCoreApplication::translate("Dlg_AddHero", "Dialog", nullptr));
        btn_save->setText(QCoreApplication::translate("Dlg_AddHero", "\344\277\235\345\255\230", nullptr));
        btn_cancel->setText(QCoreApplication::translate("Dlg_AddHero", "\345\217\226\346\266\210", nullptr));
        label->setText(QCoreApplication::translate("Dlg_AddHero", "\350\213\261\351\233\204", nullptr));
        label_2->setText(QCoreApplication::translate("Dlg_AddHero", "\350\201\214\344\270\232", nullptr));
        label_3->setText(QCoreApplication::translate("Dlg_AddHero", "\345\261\236\346\200\247", nullptr));
        label_4->setText(QCoreApplication::translate("Dlg_AddHero", "\346\212\200\350\203\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dlg_AddHero: public Ui_Dlg_AddHero {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLG_ADDHERO_H
