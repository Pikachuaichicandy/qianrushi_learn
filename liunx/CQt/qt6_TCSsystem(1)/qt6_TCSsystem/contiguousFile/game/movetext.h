#ifndef MOVETEXT_H
#define MOVETEXT_H

#include <QWidget>
#include <QFont>
#include <QTimer>
#include <QString>

namespace Ui {
class movetext;
}

class movetext : public QWidget
{
    Q_OBJECT

public:
    explicit movetext(QString str, QWidget *parent = nullptr);
    ~movetext();
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::movetext *ui;
    QFont font;//字体
    QTimer* timer;//定时器
    int textLeft;//文本显示的最左侧位置
    QString text;//文本
    int textWidth;//文本宽度

public slots:
    void run();
    void onTimeOut();
};

#endif // MOVETEXT_H
