#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
using namespace std;

struct point
{
    int x,y;
    point(int a,int b){x=a,y=b;}
};
//typedef struct point point;

//接收客户端消息并分类
int recvClient(int fd);
//接收int类型数据
int recvInt(int fd);
//接收包头5的资料信息
int recvData(int fd);
//接收方向信息并转发
int recvDir(int fd);
//接收玩家死亡信息并转发
int recvDeath(int fd);



//返回x,y坐标
point getPoint();
//食物坐标目前固定
point getFoodPoint();
