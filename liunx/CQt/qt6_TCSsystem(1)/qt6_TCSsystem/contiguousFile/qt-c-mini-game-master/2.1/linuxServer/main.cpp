#include<iostream>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<unistd.h>
#include"config.h"
#include"functions.h"
using namespace std;



int main()
{
    int server_fd=socket(AF_INET,SOCK_STREAM,0);
    if(server_fd<0)
    {
        perror("服务器创建失败");
        return 1;
    }

    //绑定ip和端口
    struct sockaddr_in server_addr={0};
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(m_port);
    if(bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1)
    {
        perror("bind绑定失败");
        close(server_fd);
        return 1;
    }

    //监听
    if(listen(server_fd,max_num)<0)
    {
        perror("listen监听失败");
        close(server_fd);
        return 1;
    }
    printf("服务器已经启动\n");

    //创建epoll实例
    int epoll_fd=epoll_create1(0);
    if(epoll_fd<0)
    {
        perror("epoll创建失败");
        close(server_fd);
        return 1;
    }

    //将监听的server_fd加入epoll
    struct epoll_event ev,events[max_num+1];
    ev.data.fd=server_fd;
    ev.events=EPOLLIN;
    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,server_fd,&ev)==-1)
    {
        perror("epoll服务器加入失败");
        close(epoll_fd);
        close(server_fd);
        return 1;
    }

    //创建食物线程
    thread foodThread(sendFoodPoint);

    while (true)
    {
        int event_num=epoll_wait(epoll_fd,events,max_num+1,0);
        if(event_num<0)
        {
            perror("epoll获取数量失败");
            close(epoll_fd);
            close(server_fd);
            return 1;
        }

        for(int i=0;i<event_num;i++)
        {
            int fd=events[i].data.fd;
            if(fd==server_fd)
            {
                struct sockaddr_in client_addr={0};
                socklen_t addr_len=sizeof(client_addr);
                int client_fd=accept(fd,(struct sockaddr*)&client_addr,&addr_len);
                if(client_fd<0)
                {
                    perror("accept 错误：");
                    continue;
                }

                //客户端加入到epoll实例中
                ev.data.fd=client_fd;
                ev.events=EPOLLIN;
                if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,client_fd,&ev)==-1)
                {
                    perror("epoll客户端加入失败");
                    break;
                }

                //显示连接的用户的ip
                printf("fd= %d , ip= %s , 已经连接\n",client_fd,inet_ntoa(client_addr.sin_addr));
            }
            else    //客户端事件
            {
                int n=recvClient(fd);
                if(n==-1 || n==0)
                {
                    perror("客户端出错");
                    if(epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,nullptr)==-1)
                    {
                        perror("客户端删除失败");
                        continue;
                    }
                    shutdown(fd,SHUT_RDWR);
                    close(fd);
                    continue;
                }
                
            }
        }
    }

    foodThread.join();
    close(epoll_fd);
    close(server_fd);

    return 0;
}