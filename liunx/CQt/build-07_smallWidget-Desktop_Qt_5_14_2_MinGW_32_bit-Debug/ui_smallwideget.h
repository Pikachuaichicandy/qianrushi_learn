/********************************************************************************
** Form generated from reading UI file 'smallwideget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SMALLWIDEGET_H
#define UI_SMALLWIDEGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_smallWideget
{
public:
    QHBoxLayout *horizontalLayout;
    QSpinBox *spinBox;
    QSlider *horizontalSlider;

    void setupUi(QWidget *smallWideget)
    {
        if (smallWideget->objectName().isEmpty())
            smallWideget->setObjectName(QString::fromUtf8("smallWideget"));
        smallWideget->resize(257, 40);
        horizontalLayout = new QHBoxLayout(smallWideget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        spinBox = new QSpinBox(smallWideget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        horizontalLayout->addWidget(spinBox);

        horizontalSlider = new QSlider(smallWideget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider);


        retranslateUi(smallWideget);

        QMetaObject::connectSlotsByName(smallWideget);
    } // setupUi

    void retranslateUi(QWidget *smallWideget)
    {
        smallWideget->setWindowTitle(QCoreApplication::translate("smallWideget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class smallWideget: public Ui_smallWideget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SMALLWIDEGET_H
