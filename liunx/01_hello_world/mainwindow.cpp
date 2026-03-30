#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // 设置窗口大小为LCD分辨率
    setFixedSize(1024, 600);
    
    // 创建测试按钮
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // 标题标签
    QLabel *titleLabel = new QLabel("LCD屏幕测试程序 - 分辨率: 1024x600", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333; margin: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // 测试区域
    QLabel *testArea = new QLabel(this);
    testArea->setFixedSize(800, 400);
    testArea->setStyleSheet("background-color: white; border: 2px solid #ccc;");
    testArea->setAlignment(Qt::AlignCenter);
    testArea->setText("点击下方按钮开始测试");
    testArea->setStyleSheet("font-size: 18px; color: #666; background-color: white; border: 2px solid #ccc;");
    mainLayout->addWidget(testArea, 0, Qt::AlignCenter);
    
    // 按钮布局
    QGridLayout *buttonLayout = new QGridLayout();
    
    // 颜色测试按钮
    QPushButton *redBtn = new QPushButton("红色测试", this);
    QPushButton *greenBtn = new QPushButton("绿色测试", this);
    QPushButton *blueBtn = new QPushButton("蓝色测试", this);
    QPushButton *whiteBtn = new QPushButton("白色测试", this);
    QPushButton *blackBtn = new QPushButton("黑色测试", this);
    
    // 其他测试按钮
    QPushButton *gradientBtn = new QPushButton("渐变测试", this);
    QPushButton *gridBtn = new QPushButton("网格测试", this);
    QPushButton *resetBtn = new QPushButton("重置测试", this);
    
    // 设置按钮样式
    QString buttonStyle = "QPushButton { font-size: 14px; padding: 8px; margin: 2px; } ";
    redBtn->setStyleSheet(buttonStyle + "QPushButton { background-color: #ff6b6b; color: white; }");
    greenBtn->setStyleSheet(buttonStyle + "QPushButton { background-color: #51cf66; color: white; }");
    blueBtn->setStyleSheet(buttonStyle + "QPushButton { background-color: #339af0; color: white; }");
    whiteBtn->setStyleSheet(buttonStyle + "QPushButton { background-color: white; color: black; border: 1px solid #ccc; }");
    blackBtn->setStyleSheet(buttonStyle + "QPushButton { background-color: black; color: white; }");
    gradientBtn->setStyleSheet(buttonStyle + "QPushButton { background-color: #ffd43b; color: black; }");
    gridBtn->setStyleSheet(buttonStyle + "QPushButton { background-color: #9775fa; color: white; }");
    resetBtn->setStyleSheet(buttonStyle + "QPushButton { background-color: #495057; color: white; }");
    
    // 第一行按钮
    buttonLayout->addWidget(redBtn, 0, 0);
    buttonLayout->addWidget(greenBtn, 0, 1);
    buttonLayout->addWidget(blueBtn, 0, 2);
    buttonLayout->addWidget(whiteBtn, 0, 3);
    
    // 第二行按钮
    buttonLayout->addWidget(blackBtn, 1, 0);
    buttonLayout->addWidget(gradientBtn, 1, 1);
    buttonLayout->addWidget(gridBtn, 1, 2);
    buttonLayout->addWidget(resetBtn, 1, 3);
    
    mainLayout->addLayout(buttonLayout);
    
    // 连接信号槽
    connect(redBtn, &QPushButton::clicked, this, &MainWindow::onTestRed);
    connect(greenBtn, &QPushButton::clicked, this, &MainWindow::onTestGreen);
    connect(blueBtn, &QPushButton::clicked, this, &MainWindow::onTestBlue);
    connect(whiteBtn, &QPushButton::clicked, this, &MainWindow::onTestWhite);
    connect(blackBtn, &QPushButton::clicked, this, &MainWindow::onTestBlack);
    connect(gradientBtn, &QPushButton::clicked, this, &MainWindow::onTestGradient);
    connect(gridBtn, &QPushButton::clicked, this, &MainWindow::onTestGrid);
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::onResetTest);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTestRed()
{
    QLabel *testArea = findChild<QLabel*>();
    if (testArea) {
        testArea->setStyleSheet("background-color: red; color: white; font-size: 20px; font-weight: bold; border: 2px solid #ccc;");
        testArea->setText("红色测试\n检查红色显示是否均匀\n无坏点、无偏色");
    }
}

void MainWindow::onTestGreen()
{
    QLabel *testArea = findChild<QLabel*>();
    if (testArea) {
        testArea->setStyleSheet("background-color: green; color: white; font-size: 20px; font-weight: bold; border: 2px solid #ccc;");
        testArea->setText("绿色测试\n检查绿色显示是否均匀\n无坏点、无偏色");
    }
}

void MainWindow::onTestBlue()
{
    QLabel *testArea = findChild<QLabel*>();
    if (testArea) {
        testArea->setStyleSheet("background-color: blue; color: white; font-size: 20px; font-weight: bold; border: 2px solid #ccc;");
        testArea->setText("蓝色测试\n检查蓝色显示是否均匀\n无坏点、无偏色");
    }
}

void MainWindow::onTestWhite()
{
    QLabel *testArea = findChild<QLabel*>();
    if (testArea) {
        testArea->setStyleSheet("background-color: white; color: black; font-size: 20px; font-weight: bold; border: 2px solid #ccc;");
        testArea->setText("白色测试\n检查白色显示是否均匀\n无亮点、亮度一致");
    }
}

void MainWindow::onTestBlack()
{
    QLabel *testArea = findChild<QLabel*>();
    if (testArea) {
        testArea->setStyleSheet("background-color: black; color: white; font-size: 20px; font-weight: bold; border: 2px solid #ccc;");
        testArea->setText("黑色测试\n检查黑色显示是否纯净\n无漏光、无亮点");
    }
}

void MainWindow::onTestGradient()
{
    QLabel *testArea = findChild<QLabel*>();
    if (testArea) {
        testArea->setStyleSheet(
            "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 red, stop:0.5 green, stop:1 blue); "
            "color: white; font-size: 18px; font-weight: bold; border: 2px solid #ccc;"
        );
        testArea->setText("渐变测试\n检查颜色过渡是否平滑\n无断层、无banding现象");
    }
}

void MainWindow::onTestGrid()
{
    QLabel *testArea = findChild<QLabel*>();
    if (testArea) {
        testArea->setStyleSheet("background-color: white; color: black; font-size: 16px; border: 2px solid #ccc;");
        testArea->setText("网格测试\n检查像素排列是否整齐\n无坏点、无亮点");
        
        // 创建网格效果
        QPixmap pixmap(800, 400);
        pixmap.fill(Qt::white);
        
        QPainter painter(&pixmap);
        painter.setPen(QPen(Qt::black, 1));
        
        // 绘制网格
        for (int x = 0; x <= 800; x += 20) {
            painter.drawLine(x, 0, x, 400);
        }
        for (int y = 0; y <= 400; y += 20) {
            painter.drawLine(0, y, 800, y);
        }
        
        testArea->setPixmap(pixmap);
    }
}

void MainWindow::onResetTest()
{
    QLabel *testArea = findChild<QLabel*>();
    if (testArea) {
        testArea->setStyleSheet("font-size: 18px; color: #666; background-color: white; border: 2px solid #ccc;");
        testArea->setText("点击下方按钮开始测试");
        testArea->clear();
    }
}