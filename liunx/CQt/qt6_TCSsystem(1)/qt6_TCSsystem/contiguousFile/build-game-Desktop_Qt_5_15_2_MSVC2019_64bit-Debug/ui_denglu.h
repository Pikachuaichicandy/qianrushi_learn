/********************************************************************************
** Form generated from reading UI file 'denglu.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DENGLU_H
#define UI_DENGLU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_denglu
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout_5;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButton_4;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer_6;

    void setupUi(QWidget *denglu)
    {
        if (denglu->objectName().isEmpty())
            denglu->setObjectName(QString::fromUtf8("denglu"));
        denglu->resize(1004, 800);
        denglu->setMinimumSize(QSize(411, 600));
        denglu->setMaximumSize(QSize(16777215, 16777215));
        denglu->setStyleSheet(QString::fromUtf8("QWidget{background-color:rgb(234, 254, 255)}"));
        layoutWidget = new QWidget(denglu);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 1211, 801));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_5 = new QSpacerItem(350, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalSpacer_2 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_2);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(80, 30));
        label->setMaximumSize(QSize(80, 30));
        QFont font;
        font.setPointSize(15);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(100, 30));
        label_2->setMaximumSize(QSize(100, 30));
        label_2->setFont(font);

        horizontalLayout_2->addWidget(label_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_4->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, 0, -1, 0);
        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy);
        lineEdit->setMinimumSize(QSize(0, 30));
        lineEdit->setMaximumSize(QSize(16777215, 30));
        QFont font1;
        font1.setPointSize(18);
        font1.setUnderline(false);
        lineEdit->setFont(font1);
        lineEdit->setFocusPolicy(Qt::ClickFocus);
        lineEdit->setStyleSheet(QString::fromUtf8("QLineEdit{border:none}"));
        lineEdit->setMaxLength(10);

        horizontalLayout_3->addWidget(lineEdit);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(20, 15));
        pushButton->setMaximumSize(QSize(20, 15));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton{border-image:url(:/images/down.png)}"));

        horizontalLayout_3->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_3);

        widget = new QWidget(layoutWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(0, 2));
        widget->setMaximumSize(QSize(16777215, 2));
        widget->setStyleSheet(QString::fromUtf8("QWidget{background:rgb(176, 176, 176)}"));

        verticalLayout_2->addWidget(widget);


        verticalLayout_4->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, 0, -1, 0);
        lineEdit_2 = new QLineEdit(layoutWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        sizePolicy.setHeightForWidth(lineEdit_2->sizePolicy().hasHeightForWidth());
        lineEdit_2->setSizePolicy(sizePolicy);
        lineEdit_2->setMinimumSize(QSize(0, 30));
        lineEdit_2->setMaximumSize(QSize(16777215, 30));
        lineEdit_2->setFont(font1);
        lineEdit_2->setFocusPolicy(Qt::ClickFocus);
        lineEdit_2->setStyleSheet(QString::fromUtf8("QLineEdit{border:none}"));
        lineEdit_2->setMaxLength(12);
        lineEdit_2->setEchoMode(QLineEdit::Password);

        horizontalLayout_4->addWidget(lineEdit_2);

        pushButton_2 = new QPushButton(layoutWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(30, 30));
        pushButton_2->setMaximumSize(QSize(30, 30));
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton{border-image:url(:/images/forward.png)}"));

        horizontalLayout_4->addWidget(pushButton_2);


        verticalLayout_3->addLayout(horizontalLayout_4);

        widget_2 = new QWidget(layoutWidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setMinimumSize(QSize(0, 2));
        widget_2->setMaximumSize(QSize(16777215, 2));
        widget_2->setStyleSheet(QString::fromUtf8("QWidget{background:rgb(176, 176, 176)}"));

        verticalLayout_3->addWidget(widget_2);


        verticalLayout_4->addLayout(verticalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        pushButton_3 = new QPushButton(layoutWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setLayoutDirection(Qt::LeftToRight);
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton{}"));

        horizontalLayout_5->addWidget(pushButton_3);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        pushButton_4 = new QPushButton(layoutWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        horizontalLayout_5->addWidget(pushButton_4);


        verticalLayout_4->addLayout(horizontalLayout_5);


        verticalLayout_5->addLayout(verticalLayout_4);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_3);


        horizontalLayout_6->addLayout(verticalLayout_5);

        horizontalSpacer_6 = new QSpacerItem(600, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);


        retranslateUi(denglu);

        QMetaObject::connectSlotsByName(denglu);
    } // setupUi

    void retranslateUi(QWidget *denglu)
    {
        denglu->setWindowTitle(QCoreApplication::translate("denglu", "Form", nullptr));
        label->setText(QCoreApplication::translate("denglu", "\350\205\276\350\256\257QQ", nullptr));
        label_2->setText(QCoreApplication::translate("denglu", "\344\271\220\345\234\250\346\262\237\351\200\232", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("denglu", "\350\264\246\345\217\267", nullptr));
        pushButton->setText(QString());
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("denglu", "\345\257\206\347\240\201", nullptr));
        pushButton_2->setText(QString());
        pushButton_3->setText(QCoreApplication::translate("denglu", " \346\230\276\347\244\272\345\257\206\347\240\201", nullptr));
        pushButton_4->setText(QCoreApplication::translate("denglu", "\346\263\250\345\206\214\350\264\246\345\217\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class denglu: public Ui_denglu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DENGLU_H
