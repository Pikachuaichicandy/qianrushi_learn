#include "net.h"
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
void cli_data_handle(void *arg);
void sig_child_handle(int sig)
{
    // 处理子进程结束信号，避免僵尸进程
    if (sig == SIGCHLD)
    {
        while (waitpid(-1, NULL, WNOHANG) > 0); // 回收所有已结束的子进程
    }
}
int main(void)
{
    int fd = -1;
    struct sockaddr_in sin ;
    signal(SIGCHLD, sig_child_handle); // 忽略子进程结束信号，防止僵尸进程
    
    /*
    创建socket fd*/
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(-1);
    }
    /*绑定*/
    /*2.1填充struct sockaddr_in结构体变量*/
    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERV_PORT);//将主机字节序转换为网络字节序
    
    
    /*优化让服务器可以绑定在任意的IP地址上*/


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
    /*2.2绑定*/
    if(bind(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("bind");
        exit(-1);
    } 
    /*3.监听*/
    /*调用listen()把主动套接字转换为被动套接字*/
    if(listen(fd, BACKLOG) < 0)
    {
        perror("listen");
        exit(-1);
    }
    printf("Server starting ....ok!\n");
    int newfd = -1;
    /*4.阻塞等待客户端连接请求*/
    #if 0
    int newfd = -1;
    newfd = accept(fd, NULL, NULL);
    if(newfd < 0)
    {
        perror("accept");
        exit(-1);
    }   
    /*4.接受连接*/
    #else
    // /*优化2：通过程序获取刚建立连接的socket的客户端的ip地址和端口号*/
    // struct sockaddr_in client_addr;
    // socklen_t addrlen = sizeof(client_addr);
    // if ((newfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen)) < 0)
    // {
    //     perror("accept");
    //     exit(-1);
    // }
    // // printf("client ip = %s,port = %d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));//inet_ntoa将网络字节序转换为点分十进制字符串
    //                                                                                                 //ntohs将网络字节序转换为主机字节序的端口号
    // char ipv4_addr[16];
    // if(inet_ntop(AF_INET, (void *)&client_addr.sin_addr, ipv4_addr, sizeof(ipv4_addr)) != 1)
    // {
    //     perror("inet_ntop");
    //     exit(-1);
    // }
    // printf("Clinet (%s:%d) is connected.\n", ipv4_addr, ntohs(client_addr.sin_port));


    /*优化3用多进程处理已经建立好连接的客户端数据*/

    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    while(1)
    {   
        pid_t pid = -1 ;
        int newfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
        if(newfd < 0)
        {
            perror("accept");
            break;
        }
        /*创建一个子进程用于处理已经建立连接的客户的交互数据*/
        if((pid_t pid = fork()) < 0)
        {
            perror("fork");
            break;
        }
        if(0 ==pid)
        {
            //子进程处理数据
            char ipv4_addr[16];
            if(inet_ntop(AF_INET, (void *)&client_addr.sin_addr, ipv4_addr, sizeof(ipv4_addr)) != 1)
            {
                perror("inet_ntop");
                exit(-1);
            }
            printf("Clinet (%s:%d) is connected.\n", ipv4_addr, ntohs(client_addr.sin_port));
            
            //和newfd读写
            int ret = -1;
            char buf[BUFSIZ];
            while(1){
                bzero(buf,BUFSIZ);
                do
                {
                     ret = read(newfd,buf,BUFSIZ-1);
                }while(ret<0 && EINTR ==errno);
                if(ret < 0)
                {
                    perror("read");
                    exit(-1);
                }
                if(ret == 0){
                    //对方已经关闭
                    break;
                }
                printf("receive data = %s\n",buf);
                if(strncasecmp(buf,QUIT_STR,strlen(QUIT_STR)) == 0){
                    //对方关闭连接
                    break;
                }

            }
            /*6.关闭连接*/
            close(newfd);
            exit(0); //子进程结束
            cli_data_handle(&newfd); // 调用处理函数
            return 0; // 确保子进程返回
        }
        else
        {
            //父进程继续等待下一个连接
            close(newfd); //父进程关闭newfd，避免资源泄露

        }

    }
    

    #endif
//     /*5.和newfd读写*/
//     int ret = -1;
//     char buf[BUFSIZ];
//     while(1){
//         bzero(buf,BUFSIZ);
//         do
//         {
//              ret = read(newfd,buf,BUFSIZ-1);
//         }while(ret<0 && EINTR ==errno);
//         if(ret < 0)
//         {
//             perror("read");
//             exit(-1);
//         }
//         if(ret == 0){
//             //对方已经关闭
//             break;
//         }
//         printf("receive data = %s\n",buf);
//         if(strncasecmp(buf,QUIT_STR,strlen(QUIT_STR)) == 0){
//             //对方关闭连接
//             break;
//         }

//     }
//     /*6.关闭连接*/
//     close(fd);
//     close(newfd);
// 	return 0;
// }


void cli_data_handle(void *arg)
{
    int newfd = *(int *)arg; // 获取传入的newfd
    printf("Client data handler started for fd: %d\n", newfd);
    
    // 和newfd读写
    int ret = -1;
    char buf[BUFSIZ];
    char resp_buf[BUFSIZ+10];
    while(1){
        bzero(buf, BUFSIZ);
        do
        {
            ret = read(newfd, buf, BUFSIZ - 1);
        } while (ret < 0 && EINTR == errno);
        
        if (ret < 0)
        {
            perror("read");
            exit(-1);
        }
        if (ret == 0)
        {
            // 对方已经关闭
            break;
        }
        printf("receive data = %s\n", buf);

        bzero(resp_buf, BUFSIZ+10);
        strcpy(resp_buf, SEVR_RESP_STR,strnlen(SEVR_RESP_STR));
        strcat(resp_buf, buf);
        do
        {
            ret = write(newfd,resp_buf,strnlen(resp_buf));
        } while (ret < 0 && EINTR == errno);
        if (ret < 0)
        {
            perror("write");
            exit(-1);
        }
        if (strncasecmp(buf, QUIT_STR, strlen(QUIT_STR)) == 0)
        {
            printf("Client (%d) requested to quit.\n",newfd);
            break;
        }
    }
    
    /*6.关闭连接*/
    close(newfd);
}
}

