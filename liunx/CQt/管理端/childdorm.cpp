#include "childdorm.h"
#include "ui_childdorm.h"

childdorm::childdorm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::childdorm)
{
    ui->setupUi(this);
    this->setFixedSize(1500,800);//背景图尺寸
    // this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setWindowTitle(QStringLiteral("管理员界面"));
   this->setWindowIcon(QIcon(":/image/44.png"));//APP图标

    //窗口居住显示
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    QPalette pal = this->palette();
    //pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/27.jpg")));//背景图
    setPalette(pal);

    //设置为只读模式
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_4->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应
    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应
    ui->tableWidget_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应


    db = QSqlDatabase::addDatabase("QMYSQL"); //添加数据库
    db.setHostName("192.168.79.129");
    db.setUserName("root");
    db.setPassword("12345678");
    db.setPort(3306);
    db.setDatabaseName("Order");

    if(!db.open())
    {
       QMessageBox::warning(this,"提示","无法连接数据库");
    }


    Init();//页面初始化
}

childdorm::~childdorm()
{
    delete ui;
}

void childdorm::getname(QString name)
{
    ui->lineEdit->setText(name);
}

void childdorm::on_pushButton_clicked()//重新登录
{
    this->hide();
    Widget *child = new Widget();
    child->show();
}


void childdorm::on_pushButton_2_clicked()//退出
{
  exit(-1);
}


void childdorm::on_pushButton_3_clicked()//添加用户
{

    add_users *child = new add_users();
    child->show();
}

void childdorm::on_pushButton_5_clicked()//修改密码
{
    change_pw *child = new change_pw();
    child->show();
}

void childdorm::on_pushButton_6_clicked()//修改权限
{
    change_per *child = new change_per();
    child->show();
}


void childdorm::on_pushButton_8_clicked()//添加菜品
{
    Add_food *child = new Add_food();
    child->show();
}


void childdorm::on_pushButton_10_clicked()//修改菜品价格
{
    change_price *child = new change_price;
    child->show();
}

void childdorm::on_pushButton_11_clicked()//修改菜品状态
{
    change_status *child = new change_status;
    child->show();
}

void childdorm::on_pushButton_13_clicked()//添加饮品
{
    Add_drinks *child = new Add_drinks;
    child->show();
}

void childdorm::on_pushButton_15_clicked()//修改饮品价格
{
    Modify_drinks *child = new Modify_drinks;
    child->show();
}

void childdorm::on_pushButton_16_clicked()//修改饮品状态
{
    Modify_stutus *child = new Modify_stutus;
    child->show();
}

void childdorm::on_pushButton_18_clicked()//账单详情
{
    Bill *child = new Bill;
    child->show();
}

void childdorm::Init()  //页面初始化
{
    QSqlQuery query;
    //获得菜单信息
    bool ret = query.exec("select *from 菜单");//数据库执行语句
    if(!ret)
    {
        QSqlError error = query.lastError();
        QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
        QMessageBox::warning(this,"提示",str);
    }


    while (query.next())   //读取下一行信息
    {

        QString str = query.value("序号").toString();
        QString str1 = query.value("菜名").toString();
        QString str2 = query.value("价格").toString();
        QString str3 = query.value("状态").toString();

        char Number[10];
        char food[100];
        char Price[10];
        char State[20];
        QByteArray ba = str.toLocal8Bit();
        memcpy(Number,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str1.toLocal8Bit();
        memcpy(food,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str2.toLocal8Bit();
        memcpy(Price,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str3.toLocal8Bit();
        memcpy(State,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        strcpy(M_head.number,Number);
        strcpy(M_head.food, food);
        strcpy(M_head.price ,Price);
        strcpy(M_head.state , State);
        MenuVec.push_back(M_head);//菜单信息放入容器中
    }

    ret = query.exec("select *from 饮品");//数据库执行语句
    if(!ret)
    {
        QSqlError error = query.lastError();
        QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
        QMessageBox::warning(this,"提示",str);
    }


    while (query.next())   //读取下一行信息
    {

        QString str = query.value("序号").toString();
        QString str1 = query.value("饮品名").toString();
        QString str2 = query.value("价格").toString();
        QString str3 = query.value("状态").toString();

        char Number[10];
        char food[100];
        char Price[10];
        char State[20];
        QByteArray ba = str.toLocal8Bit();
        memcpy(Number,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str1.toLocal8Bit();
        memcpy(food,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str2.toLocal8Bit();
        memcpy(Price,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str3.toLocal8Bit();
        memcpy(State,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        strcpy(M_head.number,Number);
        strcpy(M_head.food, food);
        strcpy(M_head.price ,Price);
        strcpy(M_head.state , State);
        MenuVec.push_back(M_head);//菜单信息放入容器中
    }

    Menu_frint();//打印菜单
    User_frint();//打印用户
    Bill_frint();//打印账单
}

void childdorm::Menu_frint() //打印菜单
{
    QStringList headtext;
    headtext<<"序号"<<"餐名"<<"价格"<<"状态";
    ui->tableWidget_3->setColumnCount(headtext.count());//列表设置为和headtext相等
    ui->tableWidget_3->setHorizontalHeaderLabels(headtext);//插入表头
    ui->tableWidget_3->setRowCount(0);

    QStringList headtext1;
    headtext1<<"序号"<<"饮品名"<<"价格"<<"状态";
    ui->tableWidget_4->setColumnCount(headtext1.count());//列表设置为和headtext相等
    ui->tableWidget_4->setHorizontalHeaderLabels(headtext1);//插入表头
    ui->tableWidget_4->setRowCount(0);

    for (int j=0;j< MenuVec.size();j++)
    {
        QString temp = QString(MenuVec[j].number);
        int number = temp.toInt();
        if( number < 20000)
        {
            int rowcount = ui->tableWidget_3->rowCount();
            ui->tableWidget_3->insertRow(rowcount);
            QTableWidgetItem *column = new QTableWidgetItem(MenuVec[j].number);
            QTableWidgetItem *column1 = new QTableWidgetItem(MenuVec[j].food);
            QTableWidgetItem *column2 = new QTableWidgetItem(MenuVec[j].price);
            QTableWidgetItem *column3 = new QTableWidgetItem(MenuVec[j].state);

            ui->tableWidget_3->setItem(rowcount,0,column);
            ui->tableWidget_3->setItem(rowcount,1,column1);
            ui->tableWidget_3->setItem(rowcount,2,column2);
            ui->tableWidget_3->setItem(rowcount,3,column3);

        }
        else
        {
            int rowcount = ui->tableWidget_4->rowCount();
            ui->tableWidget_4->insertRow(rowcount);
            QTableWidgetItem *column = new QTableWidgetItem(MenuVec[j].number);
            QTableWidgetItem *column1 = new QTableWidgetItem(MenuVec[j].food);
            QTableWidgetItem *column2 = new QTableWidgetItem(MenuVec[j].price);
            QTableWidgetItem *column3 = new QTableWidgetItem(MenuVec[j].state);

            ui->tableWidget_4->setItem(rowcount,0,column);
            ui->tableWidget_4->setItem(rowcount,1,column1);
            ui->tableWidget_4->setItem(rowcount,2,column2);
            ui->tableWidget_4->setItem(rowcount,3,column3);
        }

     }
}

void childdorm::User_frint() //打印用户
{
    QSqlQuery query(db);
      //打印菜用户信息
     bool ret = query.exec("select *from 用户");//数据库执行语句
      if(!ret)
      {
          QSqlError error = query.lastError();
          QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
          QMessageBox::warning(this,"提示",str);
      }

      //tablewifget操作
      QStringList headtext;
      headtext<<"用户名"<<"密码"<<"权限";

      ui->tableWidget->setColumnCount(headtext.count());//列表设置为和headtext相等
      ui->tableWidget->setHorizontalHeaderLabels(headtext);//插入表头

      ui->tableWidget->setRowCount(0);
      while (query.next())   //读取下一行信息
      {
          int rowcount = ui->tableWidget->rowCount();

          ui->tableWidget->insertRow(rowcount);


          QTableWidgetItem *column = new QTableWidgetItem(query.value("用户名").toString());
          QTableWidgetItem *column1 = new QTableWidgetItem(query.value("密码").toString());
          QTableWidgetItem *column2 = new QTableWidgetItem(query.value("权限").toString());

          ui->tableWidget->setItem(rowcount,0,column);
          ui->tableWidget->setItem(rowcount,1,column1);
          ui->tableWidget->setItem(rowcount,2,column2);

      }
}

void childdorm::Bill_frint() //打印账单
{
    QSqlQuery query(db);
      //打印账单信息
     bool ret = query.exec("select *from 账单");//数据库执行语句
      if(!ret)
      {
          QSqlError error = query.lastError();
          QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
          QMessageBox::warning(this,"提示",str);
      }

      //tablewifget操作
      QStringList headtext;
      headtext<<"交易号"<<"时间"<<"桌号"<<"消费额"<<"收银人";

      ui->tableWidget_2->setColumnCount(headtext.count());//列表设置为和headtext相等
      ui->tableWidget_2->setHorizontalHeaderLabels(headtext);//插入表头

      ui->tableWidget_2->setRowCount(0);
      while (query.next())   //读取下一行信息
      {
          int rowcount = ui->tableWidget_2->rowCount();

          ui->tableWidget_2->insertRow(rowcount);

          QTableWidgetItem *column = new QTableWidgetItem(query.value("交易号").toString());
          QTableWidgetItem *column1 = new QTableWidgetItem(query.value("时间").toString());
          QTableWidgetItem *column2 = new QTableWidgetItem(query.value("桌号").toString());
          QTableWidgetItem *column3 = new QTableWidgetItem(query.value("消费额").toString());
          QTableWidgetItem *column4 = new QTableWidgetItem(query.value("收银人").toString());

          ui->tableWidget_2->setItem(rowcount,0,column);
          ui->tableWidget_2->setItem(rowcount,1,column1);
          ui->tableWidget_2->setItem(rowcount,2,column2);
          ui->tableWidget_2->setItem(rowcount,3,column3);
          ui->tableWidget_2->setItem(rowcount,4,column4);
      }
}

void childdorm::on_pushButton_7_clicked()//刷新用户
{
    User_frint();
}

void childdorm::on_pushButton_19_clicked()//刷新账单
{
   Bill_frint();
}

void childdorm::on_pushButton_17_clicked()//刷新饮品
{
    Menu_frint();
}

void childdorm::on_pushButton_12_clicked()//刷新菜单
{
     Menu_frint();
}

void childdorm::on_pushButton_4_clicked()//删除用户
{
     QSqlQuery query(db);
     char str[100];
     //char str1[10];
     int currow = ui->tableWidget->currentRow();//当前行
     QString name= ui->tableWidget->item(currow,0)->text();

     std::string s1 = name.toStdString();

     if(currow == -1)
     {
         QMessageBox::warning(this,"提示","操作有误!");
     }
     else
     {
         sprintf(str,"delete from 用户 where 用户名 = '%s'",s1.c_str());

         query.exec(str);

        QString temp=QString::fromUtf8("select 密码 from 用户 where 用户名 ='%1'").arg(name);
         query.exec(temp);
          query.next();
          if(query.value("密码").toString() != "\0")
          {
              QMessageBox::warning(this,"提示","删除失败!");
          }
          else
          {
              ui->tableWidget->removeRow(currow);//删除当前行
              QMessageBox::warning(this,"提示","删除成功!");

          }

     }
}

void childdorm::on_pushButton_9_clicked() //删除菜
{
    QSqlQuery query(db);
    char str[100];
    int currow = ui->tableWidget_3->currentRow();//当前行
    QString number= ui->tableWidget_3->item(currow,0)->text();

    std::string s1 = number.toStdString();

    if(currow == -1)
    {
        QMessageBox::warning(this,"提示","操作有误!");
    }
    else
    {
        sprintf(str,"delete from 菜单 where 序号 = '%s'",s1.c_str());

        query.exec(str);

         QString temp=QString::fromUtf8("select 菜名 from 菜单 where 序号 ='%1'").arg(number);
         query.exec(temp);
         query.next();
         if(query.value("菜名").toString() != "\0")
         {
             QMessageBox::warning(this,"提示","删除失败!");
         }
         else
         {
             ui->tableWidget_3->removeRow(currow);//删除当前行
             QMessageBox::warning(this,"提示","删除成功!");

         }

    }
}

void childdorm::on_pushButton_14_clicked()//删除饮品
{
    QSqlQuery query(db);
    char str[100];
    int currow = ui->tableWidget_4->currentRow();//当前行
    QString number= ui->tableWidget_4->item(currow,0)->text();

    std::string s1 = number.toStdString();

    if(currow == -1)
    {
        QMessageBox::warning(this,"提示","操作有误!");
    }
    else
    {
        sprintf(str,"delete from 饮品 where 序号 = '%s'",s1.c_str());

        query.exec(str);

         QString temp=QString::fromUtf8("select 饮品名 from 菜单 where 序号 ='%1'").arg(number);
         query.exec(temp);
         query.next();
         if(query.value("饮品名").toString() != "\0")
         {
             QMessageBox::warning(this,"提示","删除失败!");
         }
         else
         {
             ui->tableWidget_4->removeRow(currow);//删除当前行
             QMessageBox::warning(this,"提示","删除成功!");

         }

    }
}
