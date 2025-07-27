#ifndef AMAINWIDGET_H
#define AMAINWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class amainWidget;
}
QT_END_NAMESPACE

class amainWidget : public QWidget
{
    Q_OBJECT

public:
    amainWidget(QWidget *parent = nullptr);
    ~amainWidget();

private:
    Ui::amainWidget *ui;
};
#endif // AMAINWIDGET_H
