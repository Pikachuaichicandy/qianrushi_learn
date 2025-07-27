#include"functions.h"
#include"store.h"
#include"config.h"

int recvClient(int fd)
{
    int head;
    int n=recv(fd,&head,sizeof(int),0);
    if(n==-1 || n==0)
        return n;
    //转化成主机字节
    head=ntohl(head);
    printf("fd=%d , 包头为：%d\n",fd,head);
    switch (head)
    {
    case 1:
    {
        cout<<"fd= "<<fd<<" ，方向改变"<<endl;
        int num=recvDir(fd);
        if(num==-1 || num==0)
            return num;
    }
    break;
    case 3:
        {
            int num=recvInt(fd);
            if(num==-1 || num==0)
                return num;
        }
        break;
    case 5:
        {
            int num=recvData(fd);
            if(num==-1 || num==0)
                return num;
        }
        break;
    case 7:
    {
        int num=recvDeath(fd);
        if(num==-1 || num==0)
            return num;
    }
    break;
    }
    return 1;
}

int recvInt(int fd)
{
    int roomIp;
    int n=recv(fd,&roomIp,sizeof(int),0);
    if(n==-1 || n==0)
        return n;
    //转化成主机字节
    roomIp=ntohl(roomIp);
    cout<<"房间号: "<<roomIp<<endl;
    //保存处理
    m_roomIp[roomIp].push_back(fd);
    if(m_roomIp[roomIp].size()==room_num)
    {
        m_isStartGame[roomIp]=room_num;
        //cout<<"转发处理"<<endl;
        for(int i=0;i<room_num;i++)
        {
            int h=4;
            //转化成主机字节
            h=htonl(h);
            send(m_roomIp[roomIp][i],&h,sizeof(int),0);
        }
    }
    
    return 1;
}

int recvData(int fd)
{
    int roomIp;
    int n=recv(fd,&roomIp,sizeof(int),0);
    if(n==-1 || n==0)
        return n;
    //转化成主机字节
    roomIp=ntohl(roomIp);

    //接收用户名
    int name_len;
    recv(fd,(char*)&name_len,sizeof(int),0);
    int name_len_ntohl=ntohl(name_len);

    char* name=new char[name_len_ntohl];
    recv(fd,name,name_len_ntohl,0);

    //接收最后两个蛇头，蛇身皮肤
    int snakeHead,snakeBody;
    recv(fd,(char*)&snakeHead,sizeof(int),0);
    recv(fd,(char*)&snakeBody,sizeof(int),0);

    //直接根据房间号进行重组发送，这时候要加坐标
    cout<<"namesize: "<<name_len_ntohl<<"  ,name: "<<name<<endl;

    //包头（5）加  这个玩家的位置（就是对应的vector中的位置） +两个x，y坐标  +  资料大小 + 用户名大小 + 用户名
    int int_bytes=sizeof(int);
    char* data=new char[7*int_bytes+name_len_ntohl];
    int head1=htonl(5);
    memcpy(data,&head1,sizeof(int));
    int position;
    for(int i=0;i<m_roomIp[roomIp].size();i++)
    {
        if(fd==m_roomIp[roomIp][i])
        {
            position=i;
            break;
        }
    }
    position=htonl(position);
    memcpy(data+int_bytes,&position,sizeof(int));
    point p=getPoint();
    int x=htonl(p.x),y=htonl(p.y);
    memcpy(data+2*int_bytes,&x,sizeof(int));
    memcpy(data+3*int_bytes,&y,sizeof(int));
    memcpy(data+4*int_bytes,&snakeHead,sizeof(int));
    memcpy(data+5*int_bytes,&snakeBody,sizeof(int));
    memcpy(data+6*int_bytes,&name_len,sizeof(int));
    memcpy(data+7*int_bytes,name,name_len_ntohl);
    

    //转发给其他客户端
    for(int i=0;i<m_roomIp[roomIp].size();i++)
    {
        if(fd!=m_roomIp[roomIp][i])
        {
            send(m_roomIp[roomIp][i],data,7*int_bytes+name_len_ntohl,0);
        }
    }

    //包头（6）+ 自己对应的位置 + 蛇头x，y坐标 + 食物x，y坐标;
    char* selfData=new char[6*int_bytes];
    head1=htonl(6);
    point foodPoint=getFoodPoint();
    int foodx=htonl(foodPoint.x),foody=htonl(foodPoint.y);
    memcpy(selfData,&head1,sizeof(int));
    memcpy(selfData+int_bytes,&position,sizeof(int));
    memcpy(selfData+2*int_bytes,&x,sizeof(int));
    memcpy(selfData+3*int_bytes,&y,sizeof(int));
    memcpy(selfData+4*int_bytes,&foodx,sizeof(int));
    memcpy(selfData+5*int_bytes,&foody,sizeof(int));
    send(fd,selfData,6*int_bytes,0);

    //删除内存
    delete[] name;
    delete[] data;
    delete[] selfData;

    return 1;
}

int recvDir(int fd)
{
    //接收房间号
    int roomIp,posi,dir,h=htonl(1);
    int n=recv(fd,&roomIp,sizeof(int),0);
    if(n==-1 || n==0)
        return n;
    //转化成主机字节
    roomIp=ntohl(roomIp);

    //接收位置
    recv(fd,&posi,sizeof(int),0);

    //接收方向
    recv(fd,&dir,sizeof(int),0);

    //封装数据
    int bytes_int=sizeof(int);
    char* data=new char[3*bytes_int];
    memcpy(data,&h,bytes_int);
    memcpy(data+bytes_int,&posi,bytes_int);
    memcpy(data+2*bytes_int,&dir,bytes_int);

    //为了同步所以自己的也要网络转发
    for(int i=0;i<m_roomIp[roomIp].size();i++)
    {
        send(m_roomIp[roomIp][i],data,3*bytes_int,0);
    }
    delete[] data;

    return 1;
}

int recvDeath(int fd)
{
    //接收房间号
    int roomIp,posi,h=htonl(7);
    int n=recv(fd,&roomIp,sizeof(int),0);
    if(n==-1 || n==0)
        return n;
    //转化成主机字节
    roomIp=ntohl(roomIp);
    recv(fd,&posi,sizeof(int),0);

    //封装数据
    int bytes_int =sizeof(int);
    char* data=new char[2*bytes_int];
    memcpy(data,&h,bytes_int);
    memcpy(data+bytes_int,&posi,bytes_int);

    //循环发送
    for(int i=0;i<m_roomIp[roomIp].size();i++)
    {
        if(fd!=m_roomIp[roomIp][i])
        {
            send(m_roomIp[roomIp][i],data,2*bytes_int,0);
        }
    }

    m_isStartGame[roomIp]--;
    if(m_isStartGame[roomIp]<=0)      //游戏里的人都死亡的时候
    {
        int headOver=htonl(8);
        for(int i=0;i<m_roomIp[roomIp].size();i++)
        {
            send(m_roomIp[roomIp][i],&headOver,bytes_int,0);
        }
        //清除房间内存
        m_roomIp.erase(roomIp);
        m_isStartGame.erase(roomIp);
    }

    delete[] data;

    return 1;
}

point getPoint()
{
    static int y=0;
    y++;
    return point(20,y);
}

point getFoodPoint()
{
    return point(15,15);
}

void sendFoodPoint()
{
    while (true)
    {
        sleep(5);    //休眠3秒
        if(m_isStartGame.size()==0)
        {
            cout<<"没有玩家，不需要返回食物"<<endl;
            continue;
        }
        cout<<"返回了食物坐标"<<endl;
        int bytes_int=sizeof(int);
        char* data=new char[3*bytes_int];
        for(map<int,int>::iterator it=m_isStartGame.begin();it!=m_isStartGame.end();it++)
        {
            point p=getFoodPoint();
            int h=htonl(9),x=htonl(p.x),y=htonl(p.y);
            memcpy(data,&h,bytes_int);
            memcpy(data+bytes_int,&x,bytes_int);
            memcpy(data+2*bytes_int,&y,bytes_int);
            if(m_isStartGame[it->first]>0)   //没有结束游戏
            {
                for(int i=0;i<m_roomIp[it->first].size();i++)   //这里应该考虑玩家直接退出的不用发送
                {
                    send(m_roomIp[it->first][i],data,3*bytes_int,0);
                }
            }
        }
        delete[] data;
    }
    return;
}