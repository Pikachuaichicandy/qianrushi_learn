/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGroupBox *environmentGroup;
    QGridLayout *gridLayout;
    QLabel *temperatureLabel;
    QLabel *humidityLabel;
    QLabel *lightLabel;
    QGroupBox *safetyGroup;
    QGridLayout *gridLayout_2;
    QLabel *distanceLabel;
    QLabel *attitudeLabel;
    QLabel *systemStatusLabel;
    QGroupBox *controlGroup;
    QGridLayout *gridLayout_3;
    QPushButton *startConveyor;
    QGroupBox *statusGroup;
    QLabel *conveyorStateLabel;
    QLabel *gateStateLabel;
    QLabel *videoLb;
    QLabel *headpicLb;
    QWidget *widget_3;
    QLabel *titleLb_6;
    QLabel *headLb_6;
    QWidget *layoutWidget_6;
    QVBoxLayout *verticalLayout_6;
    QWidget *widget_24;
    QHBoxLayout *horizontalLayout_23;
    QLabel *label_26;
    QSpacerItem *horizontalSpacer_23;
    QLineEdit *numberEdit_6;
    QWidget *widget_25;
    QHBoxLayout *horizontalLayout_24;
    QLabel *label_27;
    QSpacerItem *horizontalSpacer_24;
    QLineEdit *nameEdit_6;
    QWidget *widget_26;
    QHBoxLayout *horizontalLayout_25;
    QLabel *label_28;
    QSpacerItem *horizontalSpacer_25;
    QLineEdit *departmentEdit_6;
    QWidget *widget_27;
    QHBoxLayout *horizontalLayout_26;
    QLabel *label_29;
    QSpacerItem *horizontalSpacer_26;
    QLineEdit *timeEdit_6;
    QWidget *widgetLb;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLabel *label_4;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1030, 787);
        environmentGroup = new QGroupBox(Widget);
        environmentGroup->setObjectName(QString::fromUtf8("environmentGroup"));
        environmentGroup->setGeometry(QRect(640, 0, 241, 241));
        gridLayout = new QGridLayout(environmentGroup);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        temperatureLabel = new QLabel(environmentGroup);
        temperatureLabel->setObjectName(QString::fromUtf8("temperatureLabel"));
        QFont font;
        font.setFamily(QString::fromUtf8("Ubuntu"));
        font.setPointSize(22);
        temperatureLabel->setFont(font);
        temperatureLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #1E90FF, \n"
"        stop:1 #87CEFA\n"
"    );\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}"));
        temperatureLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(temperatureLabel, 0, 0, 1, 1);

        humidityLabel = new QLabel(environmentGroup);
        humidityLabel->setObjectName(QString::fromUtf8("humidityLabel"));
        humidityLabel->setFont(font);
        humidityLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #1E90FF, \n"
"        stop:1 #87CEFA\n"
"    );\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}"));
        humidityLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(humidityLabel, 1, 0, 1, 1);

        lightLabel = new QLabel(environmentGroup);
        lightLabel->setObjectName(QString::fromUtf8("lightLabel"));
        lightLabel->setFont(font);
        lightLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #1E90FF, \n"
"        stop:1 #87CEFA\n"
"    );\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}"));
        lightLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lightLabel, 2, 0, 1, 1);

        safetyGroup = new QGroupBox(Widget);
        safetyGroup->setObjectName(QString::fromUtf8("safetyGroup"));
        safetyGroup->setGeometry(QRect(640, 250, 241, 151));
        gridLayout_2 = new QGridLayout(safetyGroup);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        distanceLabel = new QLabel(safetyGroup);
        distanceLabel->setObjectName(QString::fromUtf8("distanceLabel"));
        distanceLabel->setFont(font);
        distanceLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #1E90FF, \n"
"        stop:1 #87CEFA\n"
"    );\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}"));
        distanceLabel->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(distanceLabel, 1, 0, 1, 1);

        attitudeLabel = new QLabel(safetyGroup);
        attitudeLabel->setObjectName(QString::fromUtf8("attitudeLabel"));
        attitudeLabel->setFont(font);
        attitudeLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #1E90FF, \n"
"        stop:1 #87CEFA\n"
"    );\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}"));
        attitudeLabel->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(attitudeLabel, 0, 0, 1, 1);

        systemStatusLabel = new QLabel(Widget);
        systemStatusLabel->setObjectName(QString::fromUtf8("systemStatusLabel"));
        systemStatusLabel->setGeometry(QRect(290, 490, 271, 51));
        systemStatusLabel->setFont(font);
        systemStatusLabel->setAutoFillBackground(false);
        systemStatusLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #1E90FF, \n"
"        stop:1 #87CEFA\n"
"    );\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}"));
        systemStatusLabel->setAlignment(Qt::AlignCenter);
        controlGroup = new QGroupBox(Widget);
        controlGroup->setObjectName(QString::fromUtf8("controlGroup"));
        controlGroup->setGeometry(QRect(650, 420, 211, 91));
        gridLayout_3 = new QGridLayout(controlGroup);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        startConveyor = new QPushButton(controlGroup);
        startConveyor->setObjectName(QString::fromUtf8("startConveyor"));
        startConveyor->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                               stop:0 #f7b2bd, stop:1 #e91e63);\n"
"    border: 2px solid #e91e63;\n"
"    border-radius: 10px;\n"
"    color: white;\n"
"    font-size: 22px;\n"
"    font-weight: bold;\n"
"    padding: 8px 16px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                               stop:0 #f8c2cc, stop:1 #f06292);\n"
"    border: 2px solid #f06292;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                               stop:0 #e91e63, stop:1 #c2185b);\n"
"}"));

        gridLayout_3->addWidget(startConveyor, 1, 0, 1, 1);

        statusGroup = new QGroupBox(Widget);
        statusGroup->setObjectName(QString::fromUtf8("statusGroup"));
        statusGroup->setGeometry(QRect(0, 360, 271, 151));
        conveyorStateLabel = new QLabel(statusGroup);
        conveyorStateLabel->setObjectName(QString::fromUtf8("conveyorStateLabel"));
        conveyorStateLabel->setGeometry(QRect(10, 40, 251, 41));
        conveyorStateLabel->setFont(font);
        conveyorStateLabel->setAutoFillBackground(false);
        conveyorStateLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #1E90FF, \n"
"        stop:1 #87CEFA\n"
"    );\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}"));
        conveyorStateLabel->setAlignment(Qt::AlignCenter);
        gateStateLabel = new QLabel(statusGroup);
        gateStateLabel->setObjectName(QString::fromUtf8("gateStateLabel"));
        gateStateLabel->setGeometry(QRect(10, 90, 251, 51));
        gateStateLabel->setFont(font);
        gateStateLabel->setAutoFillBackground(false);
        gateStateLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #1E90FF, \n"
"        stop:1 #87CEFA\n"
"    );\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}"));
        gateStateLabel->setAlignment(Qt::AlignCenter);
        videoLb = new QLabel(Widget);
        videoLb->setObjectName(QString::fromUtf8("videoLb"));
        videoLb->setGeometry(QRect(10, 20, 271, 301));
        headpicLb = new QLabel(Widget);
        headpicLb->setObjectName(QString::fromUtf8("headpicLb"));
        headpicLb->setGeometry(QRect(40, 50, 181, 171));
        headpicLb->setStyleSheet(QString::fromUtf8("border-image: url(:/crc.png);"));
        widget_3 = new QWidget(Widget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setGeometry(QRect(280, 0, 321, 481));
        widget_3->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	border:1px solid #313236;\n"
"	border-radius:4px;\n"
"	background-color: rgb(37, 40, 49,0.9);\n"
"}\n"
"\n"
"QWidget#titleLb{\n"
"	\n"
"	font: 24pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"	color: rgb(255, 0, 0);\n"
"}\n"
"\n"
"QWidget#headLb{\n"
"	border-radius:75px;\n"
"}"));
        titleLb_6 = new QLabel(widget_3);
        titleLb_6->setObjectName(QString::fromUtf8("titleLb_6"));
        titleLb_6->setGeometry(QRect(0, 10, 320, 61));
        titleLb_6->setAlignment(Qt::AlignCenter);
        headLb_6 = new QLabel(widget_3);
        headLb_6->setObjectName(QString::fromUtf8("headLb_6"));
        headLb_6->setGeometry(QRect(80, 70, 150, 150));
        layoutWidget_6 = new QWidget(widget_3);
        layoutWidget_6->setObjectName(QString::fromUtf8("layoutWidget_6"));
        layoutWidget_6->setGeometry(QRect(10, 220, 301, 251));
        verticalLayout_6 = new QVBoxLayout(layoutWidget_6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        widget_24 = new QWidget(layoutWidget_6);
        widget_24->setObjectName(QString::fromUtf8("widget_24"));
        widget_24->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	border:1px solid #313236;\n"
"	border-radius:4px;\n"
"	background-color: rgb(37, 40, 49,0.9);\n"
"}\n"
"\n"
"QLabel{\n"
"	border:none;\n"
"	color: rgb(125, 100, 127);\n"
"	\n"
"	font: 16pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"}\n"
"\n"
"QLineEdit{\n"
"	border:none;\n"
"	\n"
"	color: rgb(255, 0, 0);\n"
"	font: 16pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"}"));
        horizontalLayout_23 = new QHBoxLayout(widget_24);
        horizontalLayout_23->setObjectName(QString::fromUtf8("horizontalLayout_23"));
        horizontalLayout_23->setContentsMargins(0, 0, 0, 0);
        label_26 = new QLabel(widget_24);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        horizontalLayout_23->addWidget(label_26);

        horizontalSpacer_23 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_23->addItem(horizontalSpacer_23);

        numberEdit_6 = new QLineEdit(widget_24);
        numberEdit_6->setObjectName(QString::fromUtf8("numberEdit_6"));
        numberEdit_6->setEnabled(false);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(numberEdit_6->sizePolicy().hasHeightForWidth());
        numberEdit_6->setSizePolicy(sizePolicy);

        horizontalLayout_23->addWidget(numberEdit_6);


        verticalLayout_6->addWidget(widget_24);

        widget_25 = new QWidget(layoutWidget_6);
        widget_25->setObjectName(QString::fromUtf8("widget_25"));
        widget_25->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	border:1px solid #313236;\n"
"	border-radius:4px;\n"
"	background-color: rgb(37, 40, 49,0.9);\n"
"}\n"
"\n"
"QLabel{\n"
"	border:none;\n"
"	color: rgb(125, 100, 127);\n"
"	\n"
"	font: 16pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"}\n"
"\n"
"QLineEdit{\n"
"	border:none;\n"
"	\n"
"	color: rgb(255, 0, 0);\n"
"	font: 16pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"}"));
        horizontalLayout_24 = new QHBoxLayout(widget_25);
        horizontalLayout_24->setObjectName(QString::fromUtf8("horizontalLayout_24"));
        horizontalLayout_24->setContentsMargins(0, 0, 0, 0);
        label_27 = new QLabel(widget_25);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        horizontalLayout_24->addWidget(label_27);

        horizontalSpacer_24 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_24->addItem(horizontalSpacer_24);

        nameEdit_6 = new QLineEdit(widget_25);
        nameEdit_6->setObjectName(QString::fromUtf8("nameEdit_6"));
        nameEdit_6->setEnabled(false);
        sizePolicy.setHeightForWidth(nameEdit_6->sizePolicy().hasHeightForWidth());
        nameEdit_6->setSizePolicy(sizePolicy);

        horizontalLayout_24->addWidget(nameEdit_6);


        verticalLayout_6->addWidget(widget_25);

        widget_26 = new QWidget(layoutWidget_6);
        widget_26->setObjectName(QString::fromUtf8("widget_26"));
        widget_26->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	border:1px solid #313236;\n"
"	border-radius:4px;\n"
"	background-color: rgb(37, 40, 49,0.9);\n"
"}\n"
"\n"
"QLabel{\n"
"	border:none;\n"
"	color: rgb(125, 100, 127);\n"
"	\n"
"	font: 16pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"}\n"
"\n"
"QLineEdit{\n"
"	border:none;\n"
"	\n"
"	color: rgb(255, 0, 0);\n"
"	font: 16pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"}"));
        horizontalLayout_25 = new QHBoxLayout(widget_26);
        horizontalLayout_25->setObjectName(QString::fromUtf8("horizontalLayout_25"));
        horizontalLayout_25->setContentsMargins(0, 0, 0, 0);
        label_28 = new QLabel(widget_26);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        horizontalLayout_25->addWidget(label_28);

        horizontalSpacer_25 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_25->addItem(horizontalSpacer_25);

        departmentEdit_6 = new QLineEdit(widget_26);
        departmentEdit_6->setObjectName(QString::fromUtf8("departmentEdit_6"));
        departmentEdit_6->setEnabled(false);
        sizePolicy.setHeightForWidth(departmentEdit_6->sizePolicy().hasHeightForWidth());
        departmentEdit_6->setSizePolicy(sizePolicy);

        horizontalLayout_25->addWidget(departmentEdit_6);


        verticalLayout_6->addWidget(widget_26);

        widget_27 = new QWidget(layoutWidget_6);
        widget_27->setObjectName(QString::fromUtf8("widget_27"));
        widget_27->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	border:1px solid #313236;\n"
"	border-radius:4px;\n"
"	background-color: rgb(37, 40, 49,0.9);\n"
"}\n"
"\n"
"QLabel{\n"
"	border:none;\n"
"	color: rgb(125, 100, 127);\n"
"	\n"
"	font: 16pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"}\n"
"\n"
"QLineEdit{\n"
"	border:none;\n"
"	\n"
"	color: rgb(255, 0, 0);\n"
"	font: 16pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"}"));
        horizontalLayout_26 = new QHBoxLayout(widget_27);
        horizontalLayout_26->setObjectName(QString::fromUtf8("horizontalLayout_26"));
        horizontalLayout_26->setContentsMargins(0, 0, 0, 0);
        label_29 = new QLabel(widget_27);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        horizontalLayout_26->addWidget(label_29);

        horizontalSpacer_26 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_26->addItem(horizontalSpacer_26);

        timeEdit_6 = new QLineEdit(widget_27);
        timeEdit_6->setObjectName(QString::fromUtf8("timeEdit_6"));
        timeEdit_6->setEnabled(false);
        sizePolicy.setHeightForWidth(timeEdit_6->sizePolicy().hasHeightForWidth());
        timeEdit_6->setSizePolicy(sizePolicy);

        horizontalLayout_26->addWidget(timeEdit_6);


        verticalLayout_6->addWidget(widget_27);

        widgetLb = new QWidget(Widget);
        widgetLb->setObjectName(QString::fromUtf8("widgetLb"));
        widgetLb->setGeometry(QRect(40, 260, 181, 61));
        widgetLb->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgba(49, 50, 54,69);"));
        horizontalLayout_2 = new QHBoxLayout(widgetLb);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(widgetLb);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(31, 21));
        label_3->setMaximumSize(QSize(31, 21));
        label_3->setStyleSheet(QString::fromUtf8("border-image: url(:/yes.png);"));

        horizontalLayout_2->addWidget(label_3);

        label_4 = new QLabel(widgetLb);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setStyleSheet(QString::fromUtf8("font: 25 24pt \"\345\276\256\350\275\257\351\233\205\351\273\221 Light\";\n"
"border:none;\n"
"background-color: rgba(49, 50, 54,0);\n"
"color: rgb(255, 255, 255);"));

        horizontalLayout_2->addWidget(label_4);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        environmentGroup->setTitle(QApplication::translate("Widget", "\347\216\257\345\242\203\347\233\221\346\265\213", nullptr));
        temperatureLabel->setText(QApplication::translate("Widget", "0", nullptr));
        humidityLabel->setText(QApplication::translate("Widget", "0", nullptr));
        lightLabel->setText(QApplication::translate("Widget", "0", nullptr));
        safetyGroup->setTitle(QApplication::translate("Widget", "\345\256\211\345\205\250\347\233\221\346\265\213", nullptr));
        distanceLabel->setText(QApplication::translate("Widget", "0", nullptr));
        attitudeLabel->setText(QApplication::translate("Widget", "0", nullptr));
        systemStatusLabel->setText(QString());
        controlGroup->setTitle(QApplication::translate("Widget", "\350\256\276\345\244\207\346\216\247\345\210\266", nullptr));
        startConveyor->setText(QApplication::translate("Widget", "\345\220\257\345\212\250\347\224\265\346\234\272", nullptr));
        statusGroup->setTitle(QApplication::translate("Widget", "\350\256\276\345\244\207\347\212\266\346\200\201", nullptr));
        conveyorStateLabel->setText(QString());
        gateStateLabel->setText(QApplication::translate("Widget", "\351\227\270\346\255\243\345\270\270", nullptr));
        videoLb->setText(QString());
        headpicLb->setText(QString());
        titleLb_6->setText(QApplication::translate("Widget", "\344\272\272\350\204\270\350\257\206\345\210\253\350\200\203\345\213\244\347\263\273\347\273\237", nullptr));
        headLb_6->setText(QString());
        label_26->setText(QApplication::translate("Widget", "\345\267\245\345\217\267", nullptr));
        numberEdit_6->setText(QApplication::translate("Widget", "0001", nullptr));
        label_27->setText(QApplication::translate("Widget", "\345\247\223\345\220\215", nullptr));
        nameEdit_6->setText(QApplication::translate("Widget", "0001", nullptr));
        label_28->setText(QApplication::translate("Widget", "\351\203\250\351\227\250", nullptr));
        departmentEdit_6->setText(QApplication::translate("Widget", "0001", nullptr));
        label_29->setText(QApplication::translate("Widget", "\346\227\266\351\227\264", nullptr));
        timeEdit_6->setText(QApplication::translate("Widget", "0001", nullptr));
        label_3->setText(QString());
        label_4->setText(QApplication::translate("Widget", "\350\256\244\350\257\201\346\210\220\345\212\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
