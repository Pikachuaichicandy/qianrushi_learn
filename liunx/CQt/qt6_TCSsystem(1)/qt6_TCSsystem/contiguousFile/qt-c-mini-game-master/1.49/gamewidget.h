#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include"mainscene.h"

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

private slots:
    void on_BMusic_clicked();

    void on_AlBfs_clicked();

private:
    void initToolWidget();
    void initMainScene();
    void initListview();

    bool isAlPattern;      //判断是否是al模式（可用于转换模式）
    Ui::GameWidget *ui;
    MainScene* m_mainScene;
};
#endif // GAMEWIDGET_H
