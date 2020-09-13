#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 8888								/*侦听端口地址*/
void sig_proccess(int signo);
void sig_pipe(int signo);
void process_conn_client(int s);

int main(int argc, char *argv[])
{
	int s;										/*s为socket描述符*/
	struct sockaddr_in server_addr;			/*服务器地址结构*/
	/*if(argc == 1){
		printf("PLS input server addr\n");
		return 0;	
	}*/

	
	signal(SIGINT, sig_proccess);				/*挂接SIGINT信号，处理函数为		  sig_process()*/
	signal(SIGPIPE, sig_pipe);					/*挂接SIGPIPE信号，处理函数为sig_pipe()*/

	
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

