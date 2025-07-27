#include "movetext.h"
#include "ui_movetext.h"
#include <QPainter>
#include <QPen>
#include <QRectF>
#include <QFontMetrics>
#include <QDebug>

movetext::movetext(QString str, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::movetext),
    text(str)

{
    //初始化
    ui->setupUi(this);
    font.setPixelSize(40);
    textLeft=1002;
    //获取文本长度
    QFontMetrics fontMetrics(font);
    textWidth = fontMetrics.width(text);
    //开启定时器，run
    run();
}

movetext::~movetext()
{
    delete ui;
}

void movetext::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QPen pen(QColor("red"));
    painter.setPen(pen);
    painter.setFont(font);

    QRectF rectF(this->rect());
    rectF.setLeft(textLeft);
    //画文本
    painter.drawText(rectF, Qt::AlignVCenter, text);
}

void movetext::run()
{
    timer=new QTimer(this);
    timer->start(6);
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
}

void movetext::onTimeOut()
{
    //更新文本显示的最左侧位置
    textLeft--;
    if(textLeft <= (-textWidth+2))
        textLeft=1002;
    this->update();
}
