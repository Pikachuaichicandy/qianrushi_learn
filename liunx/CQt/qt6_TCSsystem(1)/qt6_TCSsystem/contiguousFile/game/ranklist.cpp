#include "ranklist.h"
#include "ui_ranklist.h"
#include <QFile>
#include <QDebug>
#include <QString>


ranklist::ranklist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ranklist)
{
    ui->setupUi(this);
    //设置Item的字体和大小
    font.setPixelSize(15);
    size.setWidth(100);
    size.setHeight(50);
    //为listWidget添加item
    addItems(Mode::mode_common);
    addItems(Mode::mode_hard);

}

ranklist::~ranklist()
{
    delete ui;
}

void ranklist::addItems(int mode)
{
    QString path;
    if(mode == Mode::mode_common)
        path = "F:/qt6project/qt6_TCSsystem/contiguousFile/game/scoreCommon.txt";
    else
        path = "F:/qt6project/qt6_TCSsystem/contiguousFile/game/scoreHard.txt";
     QFile file(path);
     //打开文件
     if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
         qDebug()<<"文件打开失败"<<endl;
         return;
     }
     //一行一行读，内容不为空，则加入排名中。最多10个名次。
     for(int i=0;i<10;i++){
         QString linestr = (QString)file.readLine();
         linestr.remove("\n");
         if(linestr.length()==0)
             break;
         QString str="第"+QString::number(i+1)+"名:"+linestr+"分";
         QListWidgetItem* item = new QListWidgetItem;
         item->setText(str);
         item->setFont(font);
         item->setSizeHint(size);
         if(mode == Mode::mode_common)
            ui->listWidget->addItem(item);
         else
            ui->listWidget_2->addItem(item);
     }
}

void ranklist::on_pushButton_clicked()
{
    this->close();
}
