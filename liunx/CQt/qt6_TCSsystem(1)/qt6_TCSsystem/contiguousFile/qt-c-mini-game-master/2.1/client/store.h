#ifndef STORE_H
#define STORE_H

#include <QWidget>
#include <QComboBox>
#include<QPushButton>
#include"config.h"

namespace Ui {
class Store;
}

class Store : public QWidget
{
    Q_OBJECT

public:
    explicit Store(QWidget *parent = nullptr);
    ~Store();

signals:
    void updatePaint();

private:
    void init(QList<QStringList> &stringLists);
    void saveSettings();
    void changeSkin();

    int m_head,m_body,m_bground;
    int headCount,bodyCount,bgroundCount;
    QComboBox* m_box[columnNum];
    QPushButton* m_button;
    Ui::Store *ui;
};

#endif // STORE_H
