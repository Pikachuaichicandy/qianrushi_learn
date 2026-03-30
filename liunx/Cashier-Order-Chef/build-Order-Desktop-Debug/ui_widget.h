/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_6;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_3;
    QTableWidget *tableWidget_2;
    QWidget *tab_2;
    QGridLayout *gridLayout;
    QTableWidget *tableWidget_3;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_6;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QLineEdit *lineEdit_2;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_5;
    QLineEdit *lineEdit_3;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QLineEdit *lineEdit_4;
    QSpacerItem *verticalSpacer_7;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QLineEdit *lineEdit;
    QLabel *label_8;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *horizontalSpacer_5;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_5;
    QSpacerItem *horizontalSpacer_9;
    QLabel *label_2;
    QTableWidget *tableWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_3;
    QLCDNumber *lcdNumber;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(1036, 556);
        Widget->setMinimumSize(QSize(4, 0));
        Widget->setMaximumSize(QSize(1300, 700));
        horizontalLayout_9 = new QHBoxLayout(Widget);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        label = new QLabel(Widget);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setFamily(QStringLiteral("AR PL UKai CN"));
        font.setPointSize(17);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        horizontalLayout_5->addWidget(label);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_5);

        tabWidget = new QTabWidget(Widget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setMinimumSize(QSize(2, 0));
        tabWidget->setMaximumSize(QSize(1300, 700));
        QFont font1;
        font1.setFamily(QStringLiteral("AR PL UKai CN"));
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setWeight(75);
        tabWidget->setFont(font1);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_3 = new QGridLayout(tab);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        tableWidget_2 = new QTableWidget(tab);
        tableWidget_2->setObjectName(QStringLiteral("tableWidget_2"));
        QFont font2;
        font2.setFamily(QStringLiteral("Umpush"));
        font2.setPointSize(12);
        font2.setBold(false);
        font2.setWeight(50);
        tableWidget_2->setFont(font2);

        gridLayout_3->addWidget(tableWidget_2, 0, 0, 1, 1);

        QIcon icon;
        icon.addFile(QStringLiteral(":/image/29.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab, icon, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout = new QGridLayout(tab_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tableWidget_3 = new QTableWidget(tab_2);
        tableWidget_3->setObjectName(QStringLiteral("tableWidget_3"));
        tableWidget_3->setFont(font2);

        gridLayout->addWidget(tableWidget_3, 0, 0, 1, 1);

        QIcon icon1;
        icon1.addFile(QStringLiteral(":/image/32.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab_2, icon1, QString());

        verticalLayout->addWidget(tabWidget);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);


        verticalLayout_6->addLayout(gridLayout_2);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_6->addItem(horizontalSpacer_6);


        horizontalLayout_9->addLayout(verticalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalSpacer_4 = new QSpacerItem(56, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_6);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QStringLiteral("label_4"));
        QFont font3;
        font3.setFamily(QStringLiteral("AR PL UKai CN"));
        font3.setPointSize(15);
        font3.setBold(true);
        font3.setWeight(75);
        label_4->setFont(font3);

        horizontalLayout_2->addWidget(label_4);

        lineEdit_2 = new QLineEdit(Widget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        horizontalLayout_2->addWidget(lineEdit_2);


        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(227, 32, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_5 = new QLabel(Widget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setFont(font3);

        horizontalLayout_3->addWidget(label_5);

        lineEdit_3 = new QLineEdit(Widget);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        horizontalLayout_3->addWidget(lineEdit_3);


        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(227, 33, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_7 = new QLabel(Widget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setFont(font3);

        horizontalLayout_8->addWidget(label_7);

        lineEdit_4 = new QLineEdit(Widget);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setStyleSheet(QStringLiteral("color: rgb(32, 74, 135);"));

        horizontalLayout_8->addWidget(lineEdit_4);


        verticalLayout_2->addLayout(horizontalLayout_8);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_7);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setFont(font3);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/image/41.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon2);

        horizontalLayout_4->addWidget(pushButton);

        pushButton_2 = new QPushButton(Widget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setFont(font3);
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/image/40.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_2->setIcon(icon3);

        horizontalLayout_4->addWidget(pushButton_2);


        verticalLayout_4->addLayout(horizontalLayout_4);


        verticalLayout_2->addLayout(verticalLayout_4);

        verticalSpacer_4 = new QSpacerItem(227, 32, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_6 = new QLabel(Widget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setFont(font3);

        horizontalLayout_6->addWidget(label_6);

        lineEdit = new QLineEdit(Widget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setMaximumSize(QSize(70, 16777215));
        lineEdit->setFont(font1);
        lineEdit->setStyleSheet(QStringLiteral("color: rgb(204, 0, 0);"));

        horizontalLayout_6->addWidget(lineEdit);

        label_8 = new QLabel(Widget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setFont(font1);

        horizontalLayout_6->addWidget(label_8);


        verticalLayout_2->addLayout(horizontalLayout_6);

        pushButton_3 = new QPushButton(Widget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setFont(font3);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/image/38.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_3->setIcon(icon4);

        verticalLayout_2->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(Widget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setFont(font3);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/image/42.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_4->setIcon(icon5);

        verticalLayout_2->addWidget(pushButton_4);

        verticalSpacer_5 = new QSpacerItem(227, 33, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_5);


        horizontalLayout_7->addLayout(verticalLayout_2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_5);


        horizontalLayout_9->addLayout(horizontalLayout_7);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_9, 0, 0, 1, 1);

        label_2 = new QLabel(Widget);
        label_2->setObjectName(QStringLiteral("label_2"));
        QFont font4;
        font4.setFamily(QStringLiteral("AR PL UKai CN"));
        font4.setPointSize(16);
        font4.setBold(true);
        font4.setWeight(75);
        label_2->setFont(font4);

        gridLayout_5->addWidget(label_2, 1, 0, 1, 1);

        tableWidget = new QTableWidget(Widget);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        QFont font5;
        font5.setFamily(QStringLiteral("Umpush"));
        font5.setPointSize(12);
        tableWidget->setFont(font5);

        gridLayout_5->addWidget(tableWidget, 2, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_5, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_3 = new QLabel(Widget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font1);

        horizontalLayout->addWidget(label_3);

        lcdNumber = new QLCDNumber(Widget);
        lcdNumber->setObjectName(QStringLiteral("lcdNumber"));
        QFont font6;
        font6.setFamily(QStringLiteral("Ubuntu"));
        font6.setPointSize(11);
        font6.setBold(false);
        font6.setItalic(false);
        font6.setWeight(50);
        lcdNumber->setFont(font6);
        lcdNumber->setStyleSheet(QLatin1String("color: rgb(204, 0, 0);\n"
"font: 11pt \"Ubuntu\";"));

        horizontalLayout->addWidget(lcdNumber);


        gridLayout_4->addLayout(horizontalLayout, 1, 0, 1, 1);


        horizontalLayout_9->addLayout(gridLayout_4);


        retranslateUi(Widget);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        label->setText(QApplication::translate("Widget", "<html><head/><body><p><span style=\" color:#cc0000;\">\350\217\234\345\215\225</span></p></body></html>", Q_NULLPTR));
#ifndef QT_NO_WHATSTHIS
        tabWidget->setWhatsThis(QApplication::translate("Widget", "<html><head/><body><p><br/></p><p><br/></p></body></html>", Q_NULLPTR));
#endif // QT_NO_WHATSTHIS
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Widget", "\344\270\273\351\244\220", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Widget", "\351\245\256\345\223\201", Q_NULLPTR));
        label_4->setText(QApplication::translate("Widget", "\351\244\220\345\223\201\345\272\217\345\217\267", Q_NULLPTR));
        label_5->setText(QApplication::translate("Widget", "\343\200\200\344\273\275\346\225\260\343\200\200", Q_NULLPTR));
        label_7->setText(QApplication::translate("Widget", "\345\244\207\346\263\250", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Widget", "\346\267\273\345\212\240", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("Widget", "\345\210\240\351\231\244", Q_NULLPTR));
        label_6->setText(QApplication::translate("Widget", "<html><head/><body><p align=\"center\"><span style=\" color:#204a87;\">\346\241\214\345\217\267</span></p></body></html>", Q_NULLPTR));
        label_8->setText(QApplication::translate("Widget", "<html><head/><body><p align=\"center\"><span style=\" color:#3465a4;\">(1-20)</span></p></body></html>", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("Widget", "\346\217\220\344\272\244\350\256\242\345\215\225", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("Widget", "\351\200\200\345\207\272\347\202\271\351\244\220", Q_NULLPTR));
        label_2->setText(QApplication::translate("Widget", "<html><head/><body><p><span style=\" color:#204a87;\">\345\267\262\347\202\271\351\244\220\345\223\201</span></p></body></html>", Q_NULLPTR));
        label_3->setText(QApplication::translate("Widget", " \346\200\273\351\242\235 ", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
