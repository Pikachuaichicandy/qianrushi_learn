#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include"mainscene.h"
#include"store.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GameWidget; }
QT_END_NAMESPACE

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

    void updateLcdNum(int num);

signals:
    //void setSkin(int head,int body,int bground);

private slots:
    void on_BMusic_clicked();

    void on_AlBfs_clicked();

private:
    void initToolWidget();
    void initMainScene();
    void initStore();
    void initListview();

    bool isAlPattern;      //判断是否是al模式（可用于转换模式）
    Ui::GameWidget *ui;
    MainScene* m_mainScene;
    Store* m_store;
};
#endif // GAMEWIDGET_H
