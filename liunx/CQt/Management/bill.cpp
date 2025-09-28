#include "bill.h"
#include "ui_bill.h"
#include <QDesktopWidget>
Bill::Bill(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Bill)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setFixedSize(1000,700);//背景图尺寸
    this->setWindowIcon(QIcon(":/image/44.png"));//APP图标

   //窗口居中显示
   QDesktopWidget* desktop = QApplication::desktop();
   move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

   //设置为只读模式
   ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
   ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应

    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/31.jpg")));//背景图
    setPalette(pal);
    db = QSqlDatabase::addDatabase("QMYSQL"); //添加数据库
//    db.setHostName("192.168.79.129");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("12345678");
    db.setPort(3306);
    db.setDatabaseName("Order");

    if(!db.open())
    {
       QMessageBox::warning(this,"提示","无法连接数据库");
    }
}

Bill::~Bill()
{
    delete ui;
}

void Bill::on_pushButton_clicked()//关闭
{
    close();
}

void Bill::on_pushButton_2_clicked()//查询
{
    QString Num = ui->lineEdit->text();  //获得交易号

    QSqlQuery query(db);
      //打印账单信息
     bool ret = query.exec("select *from 账单详情");//数据库执行语句
      if(!ret)
      {
          QSqlError error = query.lastError();
          QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
          QMessageBox::warning(this,"提示",str);
      }

      //tablewifget操作
      QStringList headtext;
      headtext<<"交易号"<<"桌号"<<"序号"<<"菜名"<<"价格"<<"数量"<<"备注"<<"收银人";

      ui->tableWidget->setColumnCount(headtext.count());//列表设置为和headtext相等
      ui->tableWidget->setHorizontalHeaderLabels(headtext);//插入表头

      ui->tableWidget->setRowCount(0);
      while (query.next())   //读取下一行信息
      {
          QString num = query.value("交易号").toString();
          if(num == Num)
          {
              int rowcount = ui->tableWidget->rowCount();
              ui->tableWidget->insertRow(rowcount);
              QTableWidgetItem *column = new QTableWidgetItem(query.value("交易号").toString());
              QTableWidgetItem *column1 = new QTableWidgetItem(query.value("桌号").toString());
              QTableWidgetItem *column2 = new QTableWidgetItem(query.value("序号").toString());
              QTableWidgetItem *column3 = new QTableWidgetItem(query.value("菜名").toString());
              QTableWidgetItem *column4 = new QTableWidgetItem(query.value("价格").toString());
              QTableWidgetItem *column5 = new QTableWidgetItem(query.value("数量").toString());
              QTableWidgetItem *column6 = new QTableWidgetItem(query.value("备注").toString());
              QTableWidgetItem *column7 = new QTableWidgetItem(query.value("收银人").toString());

              ui->tableWidget->setItem(rowcount,0,column);
              ui->tableWidget->setItem(rowcount,1,column1);
              ui->tableWidget->setItem(rowcount,2,column2);
              ui->tableWidget->setItem(rowcount,3,column3);
              ui->tableWidget->setItem(rowcount,4,column4);
              ui->tableWidget->setItem(rowcount,5,column5);
              ui->tableWidget->setItem(rowcount,6,column6);
              ui->tableWidget->setItem(rowcount,7,column7);
          }

      }
}
