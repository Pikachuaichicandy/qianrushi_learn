#include "mythread.h"

QTcpSocket *mythread::chef_socket=NULL;
QTcpSocket *mythread::cashier_socket=NULL;
QTcpSocket *mythread::order_socket[20] ={NULL};
mythread::mythread(QTcpSocket *socket)
{
    tcpsocket=socket;
    Init();
}

void mythread::run()
{
    connect(tcpsocket,&QTcpSocket::readyRead,  //read函数
             [=](){
         QByteArray array= tcpsocket->readAll();
         Flag(array); //客户端信息标志位判断
     });
}

void mythread::Flag(QByteArray array)//客户端信息标志位判断
{
    Foodvec.clear();
    Foodvec.resize(array.size()/sizeof(FoodInfo));
    memcpy(Foodvec.data(),array.data(),array.size());
    qDebug()<<Foodvec[0].food;
    if(strcmp(Foodvec[0].food,"menu") == 0)//顾客端请求菜单发送
    {
        order_menu();
    }
    else if(strcmp(Foodvec[0].food,"order") == 0)//顾客的订单信息
    {
        order_food();
    }
    else if(strcmp(Foodvec[0].food,"chef") == 0)//厨房请求订单信息
    {
        chef_socket = tcpsocket;
        chef_sent();
    }
    else if(strcmp(Foodvec[0].food,"chef_in") == 0)//厨房上菜
    {
        chef_in();
    }
    else if(strcmp(Foodvec[0].food,"cashier") == 0)//收银登录
    {
        cashier_socket = tcpsocket;
        cashier_in();
    }
    else if(strcmp(Foodvec[0].food,"cashier_query") == 0)//收银查询账单结果
    {
        cashier_query();
    }
    else if(strcmp(Foodvec[0].food,"cashier_refresh") == 0)//收银刷新餐桌状态
    {
        cashier_refresh();
    }
    else if(strcmp(Foodvec[0].food,"cashier_finish") == 0)//收银结算
    {
        cashier_finish();
    }

}

void mythread::cashier_in()//收银登录
{
    QSqlQuery query(db);

    QString name = Foodvec[1].food;//用户名
    QString pw = Foodvec[1].post;//密码

    QString temp=QString::fromUtf8("select 密码 from 用户 where 用户名 ='%1'").arg(name);
    query.exec(temp);
    query.next();
    if(query.value("密码").toString() ==  pw)
    {
        strcpy(Foodvec[0].food,"cashier_in");
        QByteArray a1;
        a1.resize(sizeof(FoodInfo)*Foodvec.size());
        memcpy(a1.data(),Foodvec.data(),sizeof(MenuInfo)*Foodvec.size());
        cashier_socket->write(a1);
    }
    else
    {
        strcpy(Foodvec[0].food,"cashier_no");
        QByteArray a1;
        a1.resize(sizeof(FoodInfo)*Foodvec.size());
        memcpy(a1.data(),Foodvec.data(),sizeof(MenuInfo)*Foodvec.size());
        cashier_socket->write(a1);
    }
}

void mythread::cashier_query()//收银查询账单结果
{
    QString temp = QString(Foodvec[0].table); //获取桌号
    int n = temp.toInt();
    Foodvec = foodvec[n];
    strcpy(Foodvec[0].food,"cashier_query");

    QByteArray a1;
    a1.resize(sizeof(FoodInfo)*Foodvec.size());
    memcpy(a1.data(),Foodvec.data(),sizeof(MenuInfo)*Foodvec.size()*2+Foodvec.size()*50);
    cashier_socket->write(a1);
}

void mythread::cashier_refresh()//收银刷新餐桌状态
{

    QSqlQuery query(db);
    //获得餐桌状态信息
    bool ret = query.exec("select *from 餐桌");//数据库执行语句
    if(!ret)
    {
       // QSqlError error = query.lastError();
       // QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
       // QMessageBox::warning(this,"提示",str);
    }

    while (query.next())   //读取下一行信息
    {
        QString str = query.value("桌号").toString();
        QString str1 = query.value("状态").toString();

        char table[5]; //桌号
        char food[100]; //餐桌状态

        QByteArray ba = str.toLocal8Bit();
        memcpy(table,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str1.toLocal8Bit();
        memcpy(food,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止

        strcpy(F_head.table, table);
        strcpy(F_head.food, food);
        Foodvec.push_back(F_head);//餐桌状态放入容器中
    }

    QByteArray a1;
    a1.resize(sizeof(FoodInfo)*Foodvec.size());
    memcpy(a1.data(),Foodvec.data(),sizeof(MenuInfo)*Foodvec.size()*2+Foodvec.size()*50);
    cashier_socket->write(a1);
}

void mythread::cashier_finish()//收银结算
{
    QString Table = QString(Foodvec[0].table);  //桌号
    int n = Table.toInt();
    QString Price = QString(Foodvec[0].price);  //消费额
    QString Name = QString(Foodvec[0].post);    //收银人
    QSqlQuery query(db);
    char str[100];

    Foodvec = foodvec[n];//获得顾客订单信息

    bool ret = query.exec("select *from 账单");//数据库执行语句
    if(!ret)
    {
//         QSqlError error = query.lastError();
//         QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
//         QMessageBox::warning(this,"提示",str);
    }

    int count = 100000;//交易号
    while (query.next())   //读取下一行信息
    {
        count = query.value("交易号").toInt();
    }
    if(count < 100001)
    {
        count = 100000;
    }
    count++;

    QString Num;
    Num=QString::number(count);

    //获取当前时间
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString Time =current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");

    char num[10]; //交易号
    char time[100]; //时间
    char table[5];//桌号
    char sprice[10];//消费额
    char name[100];//收银人
    QByteArray ba = Num.toLocal8Bit();
    memcpy(num,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
    ba = Time.toLocal8Bit();
    memcpy(time,ba.data(),ba.size()+1);  //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
    ba = Table.toLocal8Bit();
    memcpy(table,ba.data(),ba.size()+1); //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
    ba = Price.toLocal8Bit();
    memcpy(sprice,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
    ba = Name.toLocal8Bit();
    memcpy(name,ba.data(),ba.size()+1);  //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止

    sprintf(str,"insert into 账单 values('%s','%s','%s','%s','%s')",num,time,table,sprice,name);
    query.exec(str);

    for (int j=1;j<(Foodvec).size();j++)
    {

        char number[10];
        char food[100];
        char price[10];
        char quatity[10];
        char post[200];

        strcpy(number,Foodvec[j].number);
        strcpy(food,Foodvec[j].food);
        strcpy(price,Foodvec[j].price);
        strcpy(quatity,Foodvec[j].quatity);
        strcpy(post,Foodvec[j].post);
        sprintf(str,"insert into 账单详情 values('%s','%s','%s','%s','%s','%s','%s','%s')",num,table,number,food,price,quatity,post,name);
        query.exec(str);
    }


}

void mythread::order_menu()//向顾客端发送菜单
{
    QByteArray a1;
    a1.resize(sizeof(MenuInfo)*MenuVec.size());
    memcpy(a1.data(),MenuVec.data(),sizeof(MenuInfo)*MenuVec.size());
    tcpsocket->write(a1);
}

void mythread::chef_sent()//向厨房发送订单
{
    Foodvec.clear();//订单容器清空
    QSqlQuery query(db);
    //获得菜单信息
    bool ret = query.exec("select *from 厨房");//数据库执行语句
    if(!ret)
    {
       // QSqlError error = query.lastError();
       // QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
       // QMessageBox::warning(this,"提示",str);
    }

    while (query.next())   //读取下一行信息
    {
        QString str = query.value("桌号").toString();
        QString str1 = query.value("序号").toString();
        QString str2 = query.value("菜名").toString();
        QString str3 = query.value("价格").toString();
        QString str4 = query.value("数量").toString();
        QString str5 = query.value("备注").toString();

        char table[5]; //桌号
        char number[10];//序号
        char food[100]; //餐名
        char price[10];//价格
        char quatity[10];//数量
        char post[200];//备注
        QByteArray ba = str.toLocal8Bit();
        memcpy(table,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str1.toLocal8Bit();
        memcpy(number,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str2.toLocal8Bit();
        memcpy(food,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str3.toLocal8Bit();
        memcpy(price,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str4.toLocal8Bit();
        memcpy(quatity,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str5.toLocal8Bit();
        memcpy(post,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止

        strcpy(F_head.table, table);
        strcpy(F_head.number, number);
        strcpy(F_head.food, food);
        strcpy(F_head.price, price);
        strcpy(F_head.quatity, quatity);
        strcpy(F_head.post, post);
        Foodvec.push_back(F_head);//订单信息放入容器中
    }

    QByteArray a1;
    a1.resize(sizeof(FoodInfo)*Foodvec.size());
    memcpy(a1.data(),Foodvec.data(),(sizeof(MenuInfo)*Foodvec.size()*2+Foodvec.size()*50));
    chef_socket->write(a1);
}

void mythread::chef_in()//厨房上菜
{
    QSqlQuery query(db);
    char str[100];

    QString table= Foodvec[1].table;
    QString number = Foodvec[1].number;

    std::string s1 = table.toStdString();
    std::string s2 = number.toStdString();

    sprintf(str,"delete from 厨房 where 桌号 = '%s' and 序号 = '%s'",s1.c_str(),s2.c_str());
    query.exec(str);
    qDebug()<<str;

    //向顾客端发送上菜信息
    QString temp = QString(Foodvec[1].table);
    int n = temp.toInt();

    MenuVec.clear();
    strcpy(M_head.number,"\0");  //放入标志位
    strcpy(M_head.food,"chef_in"); //告诉顾客端此为上菜信息
    strcpy(M_head.price,"\0");
    strcpy(M_head.state,"\0");
    MenuVec.push_back(M_head);//菜单信息放入容器中首元素

    strcpy(M_head.number,Foodvec[1].number);
    strcpy(M_head.food,Foodvec[1].food);
    strcpy(M_head.price,Foodvec[1].price);
    strcpy(M_head.state,"\0");
    MenuVec.push_back(M_head);//所上菜品信息

    QByteArray a1;
    a1.resize(sizeof(MenuInfo)*MenuVec.size());
    memcpy(a1.data(),MenuVec.data(),sizeof(MenuInfo)*MenuVec.size());
    order_socket[n]->write(a1);
}

void mythread::order_food()//订单处理
{
    QString temp = QString(Foodvec[0].table);
    int n = temp.toInt();
    order_socket[n] = tcpsocket;
    foodvec[n] = Foodvec;
    QSqlQuery query(db);
    char str[100];

    //清除估顾客未做餐品
    QString Table= Foodvec[1].table;
    std::string s1 = Table.toStdString();

    sprintf(str,"delete from 厨房 where 桌号 = '%s' ",s1.c_str());
    query.exec(str);


    for (int j=1;j<(Foodvec).size();j++)
    {
        char table[5];
        char number[10];
        char food[100];
        char price[10];
        char quatity[10];
        char post[200];

        strcpy(table,Foodvec[j].table);
        strcpy(number,Foodvec[j].number);
        strcpy(food,Foodvec[j].food);
        strcpy(price,Foodvec[j].price);
        strcpy(quatity,Foodvec[j].quatity);
        strcpy(post,Foodvec[j].post);
        if(table != "0")
        {
            sprintf(str,"insert into 厨房 values('%s','%s','%s','%s','%s','%s')",table,number,food,price,quatity,post);
            query.exec(str);
        }
    }


}

void mythread::Init()//初始化
{
    //tcpsocket=socket;
    db = QSqlDatabase::addDatabase("QMYSQL"); //添加数据库
    db.setHostName("192.168.79.129");
    db.setUserName("root");
    db.setPassword("12345678");
    db.setPort(3306);
    db.setDatabaseName("Order");

    if(!db.open())
    {
       //QMessageBox::warning(this,"提示","无法连接数据库");
    }


    QSqlQuery query;
    //获得菜单信息
    bool ret = query.exec("select *from 菜单");//数据库执行语句
    if(!ret)
    {
       // QSqlError error = query.lastError();
       // QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
       // QMessageBox::warning(this,"提示",str);
    }

    strcpy(M_head.number,"\0");  //放入标志位
    strcpy(M_head.food,"menu");
    strcpy(M_head.price,"\0");
    strcpy(M_head.state,"\0");
    MenuVec.push_back(M_head);//菜单信息放入容器中首元素
   // qDebug()<< MenuVec[0].food;

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
//        QSqlError error = query.lastError();
//        QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
//       // QMessageBox::warning(this,"提示",str);
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
}