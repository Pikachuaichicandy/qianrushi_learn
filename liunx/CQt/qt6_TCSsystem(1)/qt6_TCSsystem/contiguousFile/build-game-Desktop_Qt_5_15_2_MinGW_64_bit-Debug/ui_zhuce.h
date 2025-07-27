/********************************************************************************
** Form generated from reading UI file 'zhuce.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZHUCE_H
#define UI_ZHUCE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ZhuCe
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *ZhuCe)
    {
        if (ZhuCe->objectName().isEmpty())
            ZhuCe->setObjectName(QString::fromUtf8("ZhuCe"));
        ZhuCe->resize(1004, 800);
        widget = new QWidget(ZhuCe);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 1011, 801));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(30);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(300, 40));
        lineEdit->setMaximumSize(QSize(350, 40));
        QFont font;
        font.setPointSize(15);
        lineEdit->setFont(font);
        lineEdit->setFocusPolicy(Qt::StrongFocus);

        verticalLayout->addWidget(lineEdit);

        lineEdit_2 = new QLineEdit(widget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setMinimumSize(QSize(300, 40));
        lineEdit_2->setMaximumSize(QSize(350, 40));
        lineEdit_2->setFont(font);
        lineEdit_2->setFocusPolicy(Qt::StrongFocus);

        verticalLayout->addWidget(lineEdit_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(0, 40));

        horizontalLayout->addWidget(pushButton_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(0, 40));

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);


        retranslateUi(ZhuCe);

        QMetaObject::connectSlotsByName(ZhuCe);
    } // setupUi

    void retranslateUi(QDialog *ZhuCe)
    {
        ZhuCe->setWindowTitle(QCoreApplication::translate("ZhuCe", "Dialog", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("ZhuCe", "\350\264\246\345\217\267(\345\217\252\350\203\275\344\270\272\346\225\260\345\255\227)", nullptr));
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("ZhuCe", "\345\257\206\347\240\201(\345\217\252\350\203\275\344\270\272\346\225\260\345\255\227\343\200\201\345\255\227\346\257\215\343\200\201_\345\222\214@ )", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ZhuCe", "\350\277\224\345\233\236", nullptr));
        pushButton->setText(QCoreApplication::translate("ZhuCe", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ZhuCe: public Ui_ZhuCe {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZHUCE_H
