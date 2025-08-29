#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>
#include <signal.h>
#include <time.h>


#define   N  32

#define  R  1   // user - register
#define  L  2   // user - login
#define  Q  3   // user - query
#define  H  4   // user - history

#define  DATABASE  "my.db"

// 定义通信双方的信息结构体
typedef struct {
	int type;
	char name[N];
	char data[256];
}MSG;



int do_client(int acceptfd, sqlite3 *db);
void do_register(int acceptfd, MSG *msg, sqlite3 *db);
int do_login(int acceptfd, MSG *msg, sqlite3 *db);
int do_query(int acceptfd, MSG *msg, sqlite3 *db);
int do_history(int acceptfd, MSG *msg, sqlite3 *db);
int history_callback(void* arg,int f_num,char** f_value,char** f_name);
int do_searchword(int acceptfd, MSG *msg, char word[]);
int get_date(char *date);

// ./server  192.168.3.196  10000
int main(int argc, const char *argv[])
{

	int sockfd;
	struct sockaddr_in  serveraddr;
	int n;
	MSG  msg;
	sqlite3 *db;
	int acceptfd;
	pid_t pid;

	if(argc != 3)
	{
		printf("Usage:%s serverip  port.\n", argv[0]);
		return -1;
	}

	//打开数据库
	if(sqlite3_open(DATABASE, &db) != SQLITE_OK)
	{
		printf("%s\n", sqlite3_errmsg(db));
		return -1;
	}
	else
	{
		printf("open DATABASE success.\n");
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM,0)) < 0)
	{
		perror("fail to socket.\n");
		return -1;
	}

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("fail to bind.\n");
		return -1;
	}

	// 将套接字设为监听模式
	if(listen(sockfd, 5) < 0)
	{
		printf("fail to listen.\n");
		return -1;
	}

	//处理僵尸进程
	// signal(SIGCHLD, SIG_IGN);



	//改为select
	// 将监听的fd的状态检测委托给内核检测
    int maxfd = sockfd;  // 让内核检测的文件描述符的范围是0~maxfd
    // 初始化检测的读集合
    fd_set rdset;
    fd_set rdtemp;
    // 清零
    FD_ZERO(&rdset);
    // 将监听的lfd设置到检测的读集合中
    FD_SET(sockfd, &rdset);
    // 通过select委托内核检测读集合中的文件描述符状态, 检测read缓冲区有没有数据
    // 如果有数据, select解除阻塞返回
    // 应该让内核持续检测





	// while(1)
	// {
	// 	if((acceptfd = accept(sockfd, NULL, NULL)) < 0)
	// 	{
	// 		perror("fail to accept");
	// 		return -1;
	// 	}

	// 	if((pid = fork()) < 0)
	// 	{
	// 		perror("fail to fork");
	// 		return -1;
	// 	}
	// 	else if(pid == 0)  // 儿子进程
	// 	{
	// 		//处理客户端具体的消息
	// 		close(sockfd);
	// 		do_client(acceptfd, db);

	// 	}
	// 	else  // 父亲进程,用来接受客户端的请求的
	// 	{
	// 		close(acceptfd);
	// 	}
	// }
	
	// return 0;

	 while(1)
    {
        // 默认阻塞
        // rdset 中是委托内核检测的所有的文件描述符
        rdtemp = rdset;
        int num = select(maxfd+1, &rdtemp, NULL, NULL, NULL);
        // rdset中的数据被内核改写了, 只保留了发生变化的文件描述的标志位上的1, 没变化的改为0
        // 只要rdset中的fd对应的标志位为1 -> 缓冲区有数据了
        // 判断
        // 有没有新连接
        if(FD_ISSET(sockfd, &rdtemp))
        {
            // 接受连接请求, 这个调用不阻塞
            struct sockaddr_in cliaddr;
            int cliLen = sizeof(cliaddr);
            int acceptfd = accept(sockfd, (struct sockaddr*)&cliaddr, &cliLen);

            // 得到了有效的文件描述符
            // 通信的文件描述符添加到读集合
            // 在下一轮select检测的时候, 就能得到缓冲区的状态
            FD_SET(acceptfd, &rdset);
            // 重置最大的文件描述符
            maxfd = acceptfd > maxfd ? acceptfd : maxfd;
        }

        // 没有新连接, 通信
        for(int i=0; i<maxfd+1; ++i)
        {
			// 判断从监听的文件描述符之后到maxfd这个范围内的文件描述符是否读缓冲区有数据
            if(i != sockfd && FD_ISSET(i, &rdtemp))
            {

				close(sockfd);
				do_client(i, db);
				FD_CLR(i, &rdset);
             	close(i);


            //     // 接收数据
            //     char buf[10] = {0};
            //     // 一次只能接收10个字节, 客户端一次发送100个字节
            //     // 一次是接收不完的, 文件描述符对应的读缓冲区中还有数据
            //     // 下一轮select检测的时候, 内核还会标记这个文件描述符缓冲区有数据 -> 再读一次
            //     // 	循环会一直持续, 知道缓冲区数据被读完位置
            //     int len = read(i, buf, sizeof(buf));
            //     if(len == 0)
            //     {
            //         printf("客户端关闭了连接...\n");
            //         // 将检测的文件描述符从读集合中删除
            //         FD_CLR(i, &rdset);
            //         close(i);
            //     }
            //     else if(len > 0)
            //     {
            //         // 收到了数据
            //         // 发送数据
            //         write(i, buf, strlen(buf)+1);
            //     }
            //     else
            //     {
            //         // 异常
            //         perror("read");
            //     }
				
            // }

		// 	else  // 父亲进程,用来接受客户端的请求的
		// 	{
		// 		close(acceptfd);
		// 	}
		// }
			}
        }

	}
	// 添加缺失的while循环闭合
	close(sockfd);
	close(acceptfd);
	sqlite3_close(db);
	return 0;

}
	



int do_client(int acceptfd, sqlite3 *db)
{
	MSG msg;
	while(recv(acceptfd, &msg, sizeof(msg), 0) > 0)
	{
	  printf("type:%d\n", msg.type);
	   switch(msg.type)
	   {
	  	 case R:
			 do_register(acceptfd, &msg, db);
			 break;
		 case L:
			 do_login(acceptfd, &msg, db);
			 break;
		 case Q:
			 do_query(acceptfd, &msg, db);
			 break;
		 case H:
			 do_history(acceptfd, &msg, db);
			 break;
		 default:
			 printf("Invalid data msg.\n");
	   }

	}

	printf("client exit.\n");
	close(acceptfd);
	exit(0);

	return 0;
}

void do_register(int acceptfd, MSG *msg, sqlite3 *db)
{
	char * errmsg;
	char sql[512];

	sprintf(sql, "insert into usr values('%s', '%s');", msg->name, msg->data);
	// snprintf(sql, sizeof(sql), "insert into usr values('%s', '%s');", msg->name, msg->data);
	printf("%s\n", sql);

	if(sqlite3_exec(db,sql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
		strcpy(msg->data, "usr name already exist.");
	}
	else
	{
		printf("client  register ok!\n");
		strcpy(msg->data, "OK!");
	}

	if(send(acceptfd, msg, sizeof(MSG), 0) < 0)
	{
		perror("fail to send");
		return ;
	}

	return ;
}

int do_login(int acceptfd, MSG *msg , sqlite3 *db)
{
	char sql[512] = {};
	char *errmsg;
	int nrow;
	int ncloumn;
	char **resultp;

	sprintf(sql, "select * from usr where name = '%s' and pass = '%s';", msg->name, msg->data);
	// snprintf(sql, sizeof(sql), "select * from usr where name = '%s' and pass = '%s';", msg->name, msg->data);
	printf("%s\n", sql);

	if(sqlite3_get_table(db, sql, &resultp, &nrow, &ncloumn, &errmsg)!= SQLITE_OK)
	{
		printf("%s\n", errmsg);
		printf("get_table fail!\n");
			return -1;
	}
	else
	{
		printf("get_table ok!\n");
	}

	// 查询成功，数据库中拥有此用户
	if(nrow == 1)
	{
		strcpy(msg->data, "OK");
		send(acceptfd, msg, sizeof(MSG), 0);
		return 1;
	}

	if(nrow == 0) // 密码或者用户名错误
	{
		strcpy(msg->data,"usr/passwd wrong.");
		send(acceptfd, msg, sizeof(MSG), 0);
	}

	return 0;
}

int do_searchword(int acceptfd, MSG *msg, char word[])
{
	FILE * fp;
	int len = 0;
	char temp[512] = {};
	int result;
	char *p;


	//打开文件，读取文件，进行比对
	
	if((fp = fopen("dict.txt", "r")) == NULL)
	{
		perror("fail to fopen.\n");
		strcpy(msg->data, "Failed to open dict.txt");
		send(acceptfd, msg, sizeof(MSG), 0);
		return -1;
	}

	//打印出，客户端要查询的单词
	len = strlen(word);
	printf("%s , len = %d\n", word, len);

	//读文件，来查询单词
	while(fgets(temp, 512, fp) != NULL)
	{

	//	printf("temp:%s\n", temp);

		// abandon  ab
		result = strncmp(temp,word,len);

		if(result < 0)
		{
			continue;
		}
		if(result > 0 || ((result == 0) && (temp[len]!=' ')))
		{
			break;
		}

		// 表示找到了，查询的单词
		p = temp + len; //  abandon   v.akdsf dafsjkj 
	//	printf("found word:%s\n", p);
		while(*p == ' ')
		{
			p++;
		}

		// 找到了注释，跳跃过所有的空格
		strcpy(msg->data, p);
		printf("found word:%s\n", msg->data);

		// 注释拷贝完毕之后，应该关闭文件
		fclose(fp);
		return 1;
	}

	fclose(fp);

	return 0;
}

int get_date(char *date)
{
	time_t t;
	struct tm *tp;

	time(&t);

	//进行时间格式转换
	tp = localtime(&t);

	sprintf(date, "%d-%d-%d %d:%d:%d", tp->tm_year + 1900, tp->tm_mon+1, tp->tm_mday, 
			tp->tm_hour, tp->tm_min , tp->tm_sec);
	printf("get date:%s\n", date);

	return 0;
}

int do_query(int acceptfd, MSG *msg , sqlite3 *db)
{
	char word[64];
	int found = 0;
	char date[128] = {};
	char sql[128] = {};
	char *errmsg;

	//拿出msg结构体中，要查询的单词
	strcpy(word, msg->data);

	found = do_searchword(acceptfd, msg, word);
	printf("查询一个单词完毕.\n");

	// 表示找到了单词，那么此时应该将 用户名，时间，单词，插入到历史记录表中去。
	if(found == 1)
	{
		// 需要获取系统时间
		get_date(date);

        sprintf(sql, "insert into record values('%s', '%s', '%s')", msg->name, date, word);

		if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
		{
			printf("%s\n", errmsg);
			return -1;
		}
		else
		{
			printf("Insert record done.\n");
		}

	}
	else  //表示没有找到
	{
		strcpy(msg->data, "Not found!");
	}

	// 将查询的结果，发送给客户端
	send(acceptfd, msg, sizeof(MSG), 0);

	return 0;
}

// 得到查询结果，并且需要将历史记录发送给客户端
int history_callback(void* arg,int f_num,char** f_value,char** f_name)
{
	// record  , name  , date  , word 
	int acceptfd;
	MSG msg;

	acceptfd = *((int *)arg);

	sprintf(msg.data, "%s , %s", f_value[1], f_value[2]);

	send(acceptfd, &msg, sizeof(MSG), 0);

	return 0;
}


int do_history(int acceptfd, MSG *msg, sqlite3 *db)
{
	char sql[128] = {};
	char *errmsg;

	sprintf(sql, "select * from record where name = '%s'", msg->name);

	//查询数据库
	if(sqlite3_exec(db, sql, history_callback,(void *)&acceptfd, &errmsg)!= SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}
	else
	{
		printf("Query record done.\n");
	}

	// 所有的记录查询发送完毕之后，给客户端发出一个结束信息
	msg->data[0] = '\0';

	send(acceptfd, msg, sizeof(MSG), 0);

	return 0;
}