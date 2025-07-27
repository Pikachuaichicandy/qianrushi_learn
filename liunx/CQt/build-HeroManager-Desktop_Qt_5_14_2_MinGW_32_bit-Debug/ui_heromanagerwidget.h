/********************************************************************************
** Form generated from reading UI file 'heromanagerwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HEROMANAGERWIDGET_H
#define UI_HEROMANAGERWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HeroManagerWidget
{
public:
    QLabel *label;
    QPushButton *btn_exit;
    QTreeWidget *treeWidget;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout;
    QPushButton *btn_updata;
    QCheckBox *checkBox;
    QPushButton *btn_del;
    QPushButton *btn_search;
    QTableWidget *tableWidget;
    QPushButton *btn_add;
    QLineEdit *le_search;
    QWidget *page_2;
    QLabel *label_3;
    QPushButton *btn_simulation;

    void setupUi(QWidget *HeroManagerWidget)
    {
        if (HeroManagerWidget->objectName().isEmpty())
            HeroManagerWidget->setObjectName(QString::fromUtf8("HeroManagerWidget"));
        HeroManagerWidget->resize(800, 600);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/pikachuya.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        HeroManagerWidget->setWindowIcon(icon);
        label = new QLabel(HeroManagerWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(340, 20, 131, 16));
        btn_exit = new QPushButton(HeroManagerWidget);
        btn_exit->setObjectName(QString::fromUtf8("btn_exit"));
        btn_exit->setGeometry(QRect(680, 10, 93, 28));
        treeWidget = new QTreeWidget(HeroManagerWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setGeometry(QRect(0, 129, 161, 421));
        stackedWidget = new QStackedWidget(HeroManagerWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(160, 90, 591, 471));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        gridLayout = new QGridLayout(page);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        btn_updata = new QPushButton(page);
        btn_updata->setObjectName(QString::fromUtf8("btn_updata"));

        gridLayout->addWidget(btn_updata, 0, 4, 1, 1);

        checkBox = new QCheckBox(page);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        btn_del = new QPushButton(page);
        btn_del->setObjectName(QString::fromUtf8("btn_del"));

        gridLayout->addWidget(btn_del, 0, 5, 1, 1);

        btn_search = new QPushButton(page);
        btn_search->setObjectName(QString::fromUtf8("btn_search"));

        gridLayout->addWidget(btn_search, 0, 2, 1, 1);

        tableWidget = new QTableWidget(page);
        if (tableWidget->columnCount() < 5)
            tableWidget->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        if (tableWidget->rowCount() < 1)
            tableWidget->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setItem(0, 0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setItem(0, 1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setItem(0, 2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setItem(0, 3, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget->setItem(0, 4, __qtablewidgetitem10);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->verticalHeader()->setVisible(false);

        gridLayout->addWidget(tableWidget, 2, 0, 1, 6);

        btn_add = new QPushButton(page);
        btn_add->setObjectName(QString::fromUtf8("btn_add"));

        gridLayout->addWidget(btn_add, 0, 3, 1, 1);

        le_search = new QLineEdit(page);
        le_search->setObjectName(QString::fromUtf8("le_search"));

        gridLayout->addWidget(le_search, 0, 1, 1, 1);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        stackedWidget->addWidget(page_2);
        label_3 = new QLabel(HeroManagerWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(571, 20, 91, 20));
        btn_simulation = new QPushButton(HeroManagerWidget);
        btn_simulation->setObjectName(QString::fromUtf8("btn_simulation"));
        btn_simulation->setGeometry(QRect(220, 560, 93, 28));

        retranslateUi(HeroManagerWidget);

        QMetaObject::connectSlotsByName(HeroManagerWidget);
    } // setupUi

    void retranslateUi(QWidget *HeroManagerWidget)
    {
        HeroManagerWidget->setWindowTitle(QCoreApplication::translate("HeroManagerWidget", "HeroManagerWidget", nullptr));
#if QT_CONFIG(tooltip)
        HeroManagerWidget->setToolTip(QCoreApplication::translate("HeroManagerWidget", "\350\277\231\344\270\252\346\230\257\347\216\213\350\200\205\350\215\243\350\200\200\347\256\241\347\220\206\347\263\273\347\273\237", nullptr));
#endif // QT_CONFIG(tooltip)
        label->setText(QCoreApplication::translate("HeroManagerWidget", "\347\216\213\350\200\205\350\215\243\350\200\200\347\256\241\347\220\206\347\263\273\347\273\237", nullptr));
        btn_exit->setText(QCoreApplication::translate("HeroManagerWidget", "\351\200\200\345\207\272", nullptr));
        btn_updata->setText(QCoreApplication::translate("HeroManagerWidget", "\344\277\256\346\224\271", nullptr));
        checkBox->setText(QCoreApplication::translate("HeroManagerWidget", "\345\205\250\351\200\211", nullptr));
        btn_del->setText(QCoreApplication::translate("HeroManagerWidget", "\345\210\240\351\231\244", nullptr));
        btn_search->setText(QCoreApplication::translate("HeroManagerWidget", "\346\220\234\347\264\242", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("HeroManagerWidget", "\345\272\217\345\217\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("HeroManagerWidget", "\350\213\261\351\233\204", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("HeroManagerWidget", "\350\201\214\344\270\232", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("HeroManagerWidget", "\345\261\236\346\200\247", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("HeroManagerWidget", "\346\212\200\350\203\275", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("HeroManagerWidget", "1", nullptr));

        const bool __sortingEnabled = tableWidget->isSortingEnabled();
        tableWidget->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->item(0, 0);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("HeroManagerWidget", "1", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->item(0, 1);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("HeroManagerWidget", "      \345\244\217\346\264\233\347\211\271", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->item(0, 2);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("HeroManagerWidget", "        \346\210\230\345\243\253", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->item(0, 3);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("HeroManagerWidget", "        \347\211\251\347\220\206", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget->item(0, 4);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("HeroManagerWidget", "     \344\270\203\346\230\237\346\226\251", nullptr));
        tableWidget->setSortingEnabled(__sortingEnabled);

        btn_add->setText(QCoreApplication::translate("HeroManagerWidget", "\345\242\236\345\212\240", nullptr));
        label_3->setText(QCoreApplication::translate("HeroManagerWidget", "\347\224\250\346\210\267\345\220\215admin", nullptr));
        btn_simulation->setText(QCoreApplication::translate("HeroManagerWidget", "\346\250\241\346\213\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HeroManagerWidget: public Ui_HeroManagerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEROMANAGERWIDGET_H
