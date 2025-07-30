#include "net.h"
int main(void)
{
    /*1. 创建socketfd*/
    int fd = -1;
    struct sockaddr_in sin;
    fd = socket(AF_INET,SOCK_DGRAM,0);
    if(fd < 0)
    {
        perror("socket");
        return -1;
    }

    /*2. 允许本地地址快速使用*/
    int b_reuse = 1;
    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&b_reuse,sizeof(b_reuse));

    /*2.1 填充sockaddr_in结构体*/
    bzero(&sin,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERV_PORT); //网络字节序的端口号

    /*2.2 绑定到任意IP地址*/
#if 1
    // sin.sin_addr.s_addr = inet_addr(SERV_IP_ADDR);
    sin.sin_addr.s_addr = htonl(INADDR_ANY); // 绑定到任意IP地址，htonl将主机字节序转换为网络字节序
#else
    if(inet_pton(AF_INET,SERV_IP_ADDR,(void*)&sin.sin_addr.s_addr) !=1)
    {
        perror("inet_pton");
        exit(-1);
    }
#endif

    /*2.3 绑定地址信息*/
    if(bind(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("bind");
        exit(-1);
    } 

    char buf [BUFSIZ];
    struct sockaddr_in cin;
    socklen_t addrlen = sizeof(cin);
    printf("UDP Server is running\n");
    while(1)
    {
        bzero(buf,BUFSIZ);
       if(recvfrom(fd,buf,BUFSIZ-1,0,(struct sockaddr *)&cin,&addrlen) < 0)
       {
           perror("recvfrom");
           continue;
       }
       char ipv4_addr[16];
       if(inet_ntop(AF_INET, (void *)&cin.sin_addr.s_addr, ipv4_addr, sizeof(ipv4_addr)) == NULL)
       {
           perror("inet_ntop");
           exit(-1);
       }
       printf("receive from(%s;%d),data:%s\n",ipv4_addr,ntohs(cin.sin_port),buf);
        if(!strncasecmp(buf,QUIT_STR,strlen(QUIT_STR)))
    {
        printf("client(%s:%d) is exiting\n",ipv4_addr,ntohs(cin.sin_port));
        break;
    }
    }
    close(fd);
    return 0;
}

   