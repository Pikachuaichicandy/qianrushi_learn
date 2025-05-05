#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QPainter>
#include <QList>
#include <QRectF>
#include <QFont>
#include <QPushButton>
#include <QKeyEvent>
#include <QTime>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 创建并设置按钮
    QPushButton *btn = new QPushButton("重新开始", this);
    btn->raise();
    btn->resize(100, 40);
    btn->move(this->width() - btn->width() - 20, 20);
    btn->setStyleSheet("background-color: red;");
    btn->show();
    connect(btn, &QPushButton::clicked, this, &MainWindow::restartGame);

    // 初始化定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::timeout);

    // 初始化蛇
    QRectF initialRect(300, 300, nodeWidth, nodeHeight);
    snake.append(initialRect);
    addTop();
    addTop();

    // 初始化奖品
    addNewReward();

    // 初始化双击加速
    lastKeyPressTime = QTime::currentTime();
    moveSpeed = 100;  // 初始速度
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // 双击加速检测
    QTime currentTime = QTime::currentTime();
    int keyInterval = lastKeyPressTime.msecsTo(currentTime);
    lastKeyPressTime = currentTime;

    if (keyInterval < 300) {
        moveSpeed = 50;  // 加速
        timer->start(moveSpeed);
    }

    // 处理按键事件
    switch (event->key()) {
    case Qt::Key_Up:
        if (moveFlag != DOWN) moveFlag = UP;
        break;
    case Qt::Key_Down:
        if (moveFlag != UP) moveFlag = DOWN;
        break;
    case Qt::Key_Right:
        if (moveFlag != LEFT) moveFlag = RIGHT;
        break;
    case Qt::Key_Left:
        if (moveFlag != RIGHT) moveFlag = LEFT;
        break;
    case Qt::Key_Space:
        if (!gameStar) {
            gameStar = true;
            timer->start(moveSpeed);  // 启动定时器
        } else {
            gameStar = false;
            timer->stop();  // 停止定时器
        }
        break;
    default:
        break;
    }
}

void MainWindow::timeout()
{
    int count = 1;
    // 判断蛇是否吃到奖品
    if (snake[0].intersects(rewardNode)) {
        count++;
        addNewReward();  // 重新生成奖品
    }

    // 根据蛇的移动方向更新蛇身
    while (count--) {
        switch (moveFlag) {
        case UP: addTop(); break;
        case DOWN: addDown(); break;
        case LEFT: addLeft(); break;
        case RIGHT: addRight(); break;
        default: break;
        }
    }

    // 移动后删除蛇尾
    deleteLast();
    update();  // 更新界面
}

void MainWindow::addTop()
{
    QPointF leftTop = snake[0].topLeft();
    QPointF rightBotom = snake[0].topRight();

    if (snake[0].y() - nodeHeight < 0) {
        leftTop.setY(this->height() - nodeHeight);
        rightBotom.setY(this->height());
    } else {
        leftTop.setY(snake[0].y() - nodeHeight);
        rightBotom.setY(leftTop.y() + nodeHeight);
    }

    snake.insert(0, QRectF(leftTop, rightBotom));
}

void MainWindow::addDown()
{
    QPointF leftTop = snake[0].bottomLeft();
    QPointF rightBotom = snake[0].bottomRight();

    if (snake[0].y() + nodeHeight * 2 > this->height()) {
        leftTop.setY(0);
        rightBotom.setY(nodeHeight);
    } else {
        rightBotom.setY(snake[0].bottomRight().y() + nodeHeight);
    }

    snake.insert(0, QRectF(leftTop, rightBotom));
}

void MainWindow::addLeft()
{
    QPointF leftTop = snake[0].topLeft();
    QPointF rightBotom = leftTop + QPointF(nodeWidth, nodeHeight);

    if (snake[0].x() - nodeWidth < 0) {
        leftTop.setX(this->width() - nodeWidth);
    } else {
        leftTop.setX(snake[0].x() - nodeWidth);
    }

    snake.insert(0, QRectF(leftTop, rightBotom));
}

void MainWindow::addRight()
{
    QPointF leftTop = snake[0].topRight();
    QPointF rightBotom = leftTop + QPointF(nodeWidth, nodeHeight);

    if (snake[0].x() + nodeWidth * 2 > this->width()) {
        leftTop.setX(0);
    }

    snake.insert(0, QRectF(leftTop, rightBotom));
}

void MainWindow::deleteLast()
{
    snake.removeLast();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    QBrush brush;

    // 绘制背景图片
    QPixmap pix("F:/32keil_chenxundaimakaifa/liunx/CQt/IMG/pikachu.jpg");
    painter.drawPixmap(0, 0, 800, 600, pix);

    // 绘制蛇身
    pen.setColor(Qt::black);
    brush.setColor(Qt::darkMagenta);
    brush.setStyle(Qt::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    for (const auto& rect : snake) {
        painter.drawRect(rect);
    }

    // 绘制奖品
    pen.setColor(Qt::red);
    brush.setColor(Qt::red);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(rewardNode);

    // 检查游戏是否结束
    if (checkContact()) {
        QFont font("宋体", 30, QFont::ExtraLight);
        painter.setFont(font);
        painter.drawText((this->width() - 300) / 2, (this->height() - 30 / 2), "GAME OVER!");
        timer->stop();
    }

    // 绘制分数
    painter.setPen(Qt::black);
    QFont font("Arial", 20);
    painter.setFont(font);
    painter.drawText(10, 30, QString("Score: %1").arg(score));
}

void MainWindow::updateScoreDisplay()
{
    score = snake.length() * 10 + gameTime.elapsed() / 1000;
}

bool MainWindow::checkContact()
{
    for (int i = 0; i < snake.length(); i++) {
        for (int j = i + 1; j < snake.length(); j++) {
            if (snake[i] == snake[j]) {
                return true;
            }
        }
    }
    return false;
}

void MainWindow::addNewReward()
{
    rewardNode = QRectF(qrand() % (this->width() / 20) * 20,
                        qrand() % (this->height() / 20) * 20,
                        nodeWidth,
                        nodeHeight);
}

void MainWindow::restartGame()
{
    // 重置蛇的状态
    snake.clear();
    QRectF initialRect(300, 300, nodeWidth, nodeHeight);
    snake.append(initialRect);
    addTop();
    addTop();

    // 重置分数
    score = 0;
    updateScoreDisplay();

    // 重置定时器
    gameStar = true;
    timer->start(moveSpeed);

    // 重置游戏时间
    gameTime.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
