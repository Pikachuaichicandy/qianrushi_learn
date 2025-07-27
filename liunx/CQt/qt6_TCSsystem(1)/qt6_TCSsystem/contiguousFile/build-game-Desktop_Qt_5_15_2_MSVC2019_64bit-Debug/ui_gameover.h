/********************************************************************************
** Form generated from reading UI file 'gameover.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEOVER_H
#define UI_GAMEOVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gameover
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *widget1;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *gameover)
    {
        if (gameover->objectName().isEmpty())
            gameover->setObjectName(QString::fromUtf8("gameover"));
        gameover->resize(400, 299);
        widget = new QWidget(gameover);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(80, 210, 241, 61));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(0, 50));
        pushButton->setMaximumSize(QSize(16777215, 50));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(0, 50));
        pushButton_2->setMaximumSize(QSize(16777215, 50));

        horizontalLayout->addWidget(pushButton_2);

        widget1 = new QWidget(gameover);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(80, 80, 241, 111));
        verticalLayout = new QVBoxLayout(widget1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget1);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(17);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        label_2 = new QLabel(widget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);


        retranslateUi(gameover);

        QMetaObject::connectSlotsByName(gameover);
    } // setupUi

    void retranslateUi(QDialog *gameover)
    {
        gameover->setWindowTitle(QCoreApplication::translate("gameover", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("gameover", "\351\200\200\345\207\272\346\270\270\346\210\217", nullptr));
        pushButton_2->setText(QCoreApplication::translate("gameover", "\345\206\215\346\235\245\344\270\200\345\261\200", nullptr));
        label->setText(QCoreApplication::translate("gameover", "\346\222\236\345\242\231\346\255\273\344\272\206", nullptr));
        label_2->setText(QCoreApplication::translate("gameover", "\346\234\200\347\273\210\345\276\227\345\210\206:10\345\210\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gameover: public Ui_gameover {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEOVER_H
