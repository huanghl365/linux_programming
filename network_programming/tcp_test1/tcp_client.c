#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 8888								/*侦听端口地址*/

typedef void (*sighandler_t)(int);
void sig_process_func(int sig);


int main(int argc, char *argv[])
{
	int s;										/*s为socket描述符*/
	struct sockaddr_in server_addr;			/*服务器地址结构*/

	
	sighandler_t ret = (sighandler_t)-2;

/*
SIGPIPE：向一个已经关闭的套接字发送数据时，可以得到一个SIGPIPE信号。
SIGPIPE信号会终止当前进程。
SIGINT：ctrl + c 或者kill -2 pid终止进程时，会收到给信号。
截取退出信号并进行处理，是程序稳定性的基本要求。
*/
#if 1
	signal(SIGPIPE, sig_process_func);      
	signal(SIGINT, sig_process_func);  
#endif
	
	s = socket(AF_INET, SOCK_STREAM, 0); 		/*建立一个流式套接字 */
	if(s < 0){									/*出错*/
		printf("socket error\n");
		return -1;
	}	
	
	/*设置服务器地址*/
	bzero(&server_addr, sizeof(server_addr));	/*清零*/
	server_addr.sin_family = AF_INET;					/*协议族*/
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	/*本地地址*/
	server_addr.sin_port = htons(PORT);				/*服务器端口*/
	
	/*将用户输入的字符串类型的IP地址转为整型*/
	//inet_pton(AF_INET, argv[1], &server_addr.sin_addr);	
	/*连接服务器*/
	connect(s, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
	process_conn_client(s);						/*客户端处理过程*/
	close(s);									/*关闭连接*/
	return 0;
}

