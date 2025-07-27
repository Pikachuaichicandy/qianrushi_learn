#ifndef __MAKE_NET_H__
#define __MAKE_NET_H__
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <sys/errno.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
// 3. 宏定义
#define SERV_PORT   5001
#define SERV_IP_ADDR     "192.168.1.100"
#define BACKLOG         5   
#define QUIT_STR    "quit"
#endif

