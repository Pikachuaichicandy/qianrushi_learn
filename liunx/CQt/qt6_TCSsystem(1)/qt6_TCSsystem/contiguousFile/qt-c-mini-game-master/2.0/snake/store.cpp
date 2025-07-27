#include "store.h"
#include "ui_store.h"
#include"config.h"
#include<QSettings>
#include<QList>
#include<QDebug>

Store::Store(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Store)
{
    ui->setupUi(this);
    m_button=new QPushButton(ui->widget);
    m_button->setGeometry(this->width()/2-50,this->height()-100,100,40);
    m_button->setText("更改设置");
    connect(m_button,&QPushButton::clicked,this,&Store::changeSkin);

    //读取settings数据
    QSettings settings("D:/Qtpro/cmake/snake/config/skin.ini", QSettings::IniFormat);
    m_head=settings.value("head").toInt();
    m_body=settings.value("body").toInt();
    m_bground=settings.value("bground").toInt();
    headCount=settings.value("headCount").toInt();
    bodyCount=settings.value("bodyCount").toInt();
    bgroundCount=settings.value("bgroundCount").toInt();

    //读取名字
    QList<QStringList> stringLists;
    QStringList list =settings.value("headName").toStringList();
    stringLists.append(list);
    list =settings.value("bodyName").toStringList();
    stringLists.append(list);
    list =settings.value("bgroundName").toStringList();
    stringLists.append(list);

    init(stringLists);
}

Store::~Store()
{
    qDebug()<<"store类析构";
    delete ui;
}

void Store::init(QList<QStringList>& stringLists)
{
    int boxWidth=150,boxHeight=45;
    int x=0;

    setFixedSize(boxWidth*(columnNum+1),boxHeight*10);

    for(int i=0;i<columnNum;i++)
    {
        m_box[i]=new QComboBox(ui->widget);
        m_box[i]->setIconSize(QSize(40,40));
        m_box[i]->setGeometry(x,0,boxWidth-20,boxHeight);
        x+=boxWidth;
    }
    for(int i=0;i<headCount;i++)
        m_box[0]->addItem(QIcon(QString(":/images/snake/head/head%1/headLeft.png").arg(i)),stringLists[0][i]);
    for(int i=0;i<bodyCount;i++)
        m_box[1]->addItem(QIcon(QString(":/images/snake/body/body%1.png").arg(i)),stringLists[1][i]);
    for(int i=0;i<bgroundCount;i++)
    {
        if(i==0)
            m_box[2]->addItem(stringLists[2][i]);
        else
            m_box[2]->addItem(QIcon(QString(":/images/background/background%1.jpg").arg(i)),stringLists[2][i]);
    }

    //设置当前索引显示的值
    m_box[0]->setCurrentIndex(m_head);
    m_box[1]->setCurrentIndex(m_body);
    m_box[2]->setCurrentIndex(m_bground);
}

void Store::saveSettings()
{
    QSettings settings("D:/Qtpro/cmake/snake/config/skin.ini", QSettings::IniFormat);
    // 设置值
    settings.setValue("head",m_head);
    settings.setValue("body",m_body);
    settings.setValue("bground",m_bground);
//    QStringList stringList;
//    stringList << "基础蛇头" << "九龙蛇头" << "鱼蛇头";
//    settings.setValue("headName",stringList);
//    stringList.clear();
//    stringList << "基础蛇身" << "雷电蛇身";
//    settings.setValue("bodyName",stringList);
//    stringList.clear();
//    stringList << "无背景皮肤"<<"紫色背景";
//    settings.setValue("bgroundName",stringList);
    // 保存设置
    settings.sync(); // 强制保存
    emit updatePaint();
}

void Store::changeSkin()
{
    if(m_box[0]->currentIndex()!=m_head || m_box[1]->currentIndex()!=m_body || m_box[2]->currentIndex()!=m_bground)
    {
        m_head=m_box[0]->currentIndex();
        m_body=m_box[1]->currentIndex();
        m_bground=m_box[2]->currentIndex();
        saveSettings();
    }
    //qDebug()<<m_head<<" "<<m_body<<" "<<m_bground;
    this->hide();
}
