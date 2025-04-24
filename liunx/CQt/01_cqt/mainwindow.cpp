#include "mainwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QPushButton>
#include <QStatusBar>
#include <QLabel>
#include <QDockWidget>
#include <QTextEdit>
mainwindow::mainwindow(QWidget *parent) : QMainWindow(parent)
{
    //重置窗口大小
    resize(600,400);
    //菜单栏创建
    QMenuBar *Bar = menuBar();
    //将菜单栏放入窗口中
    setMenuBar(Bar);
    //创建菜单
//    Bar->addMenu("文件");
    QMenu *fileMenu = Bar->addMenu("工具");
    QMenu *editMenu = Bar->addMenu("插件");
    //创建菜单项
    fileMenu->addAction("草图");
    //添加分隔线
    fileMenu->addSeparator();
    fileMenu->addAction("拉伸");


    //工具栏可以有多个
    QToolBar * toolBar = new QToolBar(this);
//    addToolBar(toolBar);
    addToolBar(Qt::LeftToolBarArea, toolBar);
    //后期设置 只允许 左右停靠
    toolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);
    //设置浮动
    toolBar->setMovable(false);
    //设置移动（总开关）
    toolBar->setMovable(false);
    //在工具栏中可以设置内容
    toolBar->addAction("开始");
    //添加分隔线
    toolBar->addSeparator();
    toolBar->addAction("钣金");
    //工具栏中添加控件
    QPushButton *btn = new QPushButton("a",this);
    toolBar->addWidget(btn);


    //状态栏最多一个
    QStatusBar *stBar = statusBar();
    //设置到窗口中
    //放标签控件
    QLabel *lable = new QLabel("提示信息",this);
    stBar->addWidget(lable);

    //铆接部件（浮动窗口）
    QDockWidget *dockWidget = new QDockWidget("浮动",this);
    addDockWidget(Qt::BottomDockWidgetArea,dockWidget);


    //设置中心部件只能有一个
    QTextEdit *edit = new QTextEdit(this);
    setCentralWidget(edit);


}
mainwindow::~mainwindow()
{

}
