#include "smallwideget.h"
#include "ui_smallwideget.h"

smallWideget::smallWideget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::smallWideget)
{
    ui->setupUi(this);
    //QspinBox移动 QSlider跟着移动
    void(QSpinBox::*spSignal)(int) = &QSpinBox::valueChanged;
//    connect(ui->spinBox,&QSpinBox::valueChanged,ui->horizontalSlider,&QSlider::setValue);
    connect(ui->spinBox, spSignal, ui->horizontalSlider, &QSlider::setValue);
    // QSpinBox 移动 QSlider 跟着移动
//        connect(ui->spinBox, SIGNAL(valueChanged(int)),
//                ui->horizontalSlider, SLOT(setValue(int)));

    // QSlider移动 QspinBox跟着移动
    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->spinBox, &QSpinBox::setValue);




}

smallWideget::~smallWideget()
{
    delete ui;
}
void smallWideget::setNum(int num) {
    ui->spinBox->setValue(num);  // 设置数字的函数实现
}

int smallWideget::getNum() {
    return ui->spinBox->value();  // 获取数字的函数实现
}
