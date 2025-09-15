#include "cd_work.h"
#include "ui_cd_work.h"

cd_work::cd_work(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cd_work)
{
    ui->setupUi(this);
}

cd_work::~cd_work()
{
    delete ui;
}


void cd_work::searchword(MSGInfo *msg)
{
    MSGInfo data;  // 直接创建对象
    // 获取 UI 中输入的word
    QString msg = ui->msg->toPlainText();
    data.msg = msg;




    if (m_tcp->state() == QTcpSocket::ConnectedState) {
        m_tcp->write(msg.toUtf8());
        ui->record->append("client say: " + msg);
    }
    else
    {
        ui->record->append("连接已断开，无法发送消息！");
    }



//    QFile file("F:/32keil_chenxundaimakaifa/liunx/CQt/QtCidian_client/dict.txt");
//        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//            qDebug() << "Failed to open dict.txt";
//            strcpy(msg->data, "Failed to open dict.txt");
//            send(acceptfd, msg, sizeof(MSG), 0);
//            return;
//        }

//        QString word = msg->data; // 用户查询的单词
//        int len = word.length();

//        QTextStream in(&file);
//        QString line;
//        while (!in.atEnd()) {
//            line = in.readLine(); // 逐行读取文件内容

//            // 比较查询单词与当前行的开头部分
//            if (line.startsWith(word)) {
//                // 找到了，提取出单词的定义
//                QString definition = line.mid(word.length()).trimmed();

//                // 将查询结果返回
//                strcpy(msg->data, definition.toUtf8().data());
//                qDebug() << "Found word:" << msg->data;

//                file.close();
//                send(acceptfd, msg, sizeof(MSG), 0);
//                return;
//            }
//        }

//        // 如果未找到单词
//        strcpy(msg->data, "Word not found");
//        send(acceptfd, msg, sizeof(MSG), 0);
//        file.close();
}



void cd_work::on_exit_clicked()
{
//    connect(ui->exit, &QPushButton::clicked, this, [=]() {
//        this->close();  // 关闭当前窗口
//        mainWindow->show();  // 假设 mainWindow 是已经实例化的主窗口对象
//    });

    this->close();
}

void cd_work::on_query_word_clicked()
{

}

