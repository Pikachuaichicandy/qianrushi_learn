#include "net.h"
#include <arpa/inet.h> // 确保包含必要的头文件
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

void usage(char *name)
{
    printf("\n%s serv_ip serv_port",name);
    printf("\n\t serv_ip:服务器IP地址");
    printf("\n\t serv_port:服务器端口号\n\n");
}

/*client serv_ip serv_port*/
int main(int argc, char **argv)
{
    int port = -1;
    int fd = -1;
    struct sockaddr_in sin; // 声明 struct sockaddr_in 类型的变量 sin

    if(argc !=3)
    {
        usage(argv[0]);
        exit(-1);
    }

    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0)
    {
        perror("socket");
        exit(-1);
    }

    port = atoi(argv[2]);
    if(port < 5000 || port > 65535)
    {
        usage(argv[0]);
        exit(-1);
    }
    
    /*2.连接服务器*/
    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);//将主机字节序转换为网络字节序*/
#if 0
    sin.sin_addr.s_addr = inet_addr(SERV_IP_ADDR);
#else
    if(inet_pton(AF_INET,argv[1],(void*)&sin.sin_addr.s_addr) !=1)
    {
        perror("inet_pton");
        exit(-1);
    }
#endif
    if(connect(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("connect");
        exit(-1);
    } 
    /*接受连接*/

    /*和服务器读写*/
    int ret = -1;
    char buf[BUFSIZ];
    while(1){
        bzero(buf,BUFSIZ);
        if(fgets(buf,BUFSIZ-1,stdin) == NULL)
        {
            perror("fgets");
            continue;
        }
        do
        {
            ret = write(fd,buf,strlen(buf));
        }while(ret<0 && EINTR ==errno);
        if(ret < 0)
        {
            perror("write");
            exit(-1);
        }
        if (strncmp(buf,"quit",4) == 0)
        {
            printf("Client is exiting\n");
            break;
        }   
        printf("write to server:%s\n",buf);
    }
    /*读写服务器*/
    /*关闭套接字*/
    return 0;
}