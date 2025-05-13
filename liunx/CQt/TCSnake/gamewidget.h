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


private:

    void initToolWidget();
    void initMainScene();
    void initListview();

    Ui::GameWidget *ui;
    MainScene* mainScene;
};
#endif // GAMEWIDGET_H
