#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

/*
程序功能描述：一个简单的TCP通信流程，包含了对退出信号的处理，并且分别使用四种IO接口进行数据的发送和接收处理
四种IO接口分别为：
read和write
recv和send
readv和writev
recvmsg和sendmsg
*/

#define PORT 8888						/*侦听端口地址*/
#define BACKLOG 2						/*侦听队列长度*/

void sig_proccess(int signo);
void sig_pipe(int signo);
void process_conn_server(int s);

int main(int argc, char *argv[])
{
	int ss,sc;		/*ss为服务器的socket描述符，sc为客户端的socket描述符*/
	struct sockaddr_in server_addr;	/*服务器地址结构*/
	struct sockaddr_in client_addr;	/*客户端地址结构*/
	int err;							/*返回值*/
	pid_t pid;							/*分叉的进行ID*/

	signal(SIGINT, sig_proccess);				/*挂接SIGINT信号，处理函数为		  sig_process()*/
	signal(SIGPIPE, sig_pipe);					/*挂接SIGPIPE信号，处理函数为sig_pipe()*/

	/*建立一个流式套接字*/
	ss = socket(AF_INET, SOCK_STREAM, 0);
	if(ss < 0){							/*出错*/
		printf("socket error\n");
		return -1;	
	}
	
	/*设置服务器地址*/
	bzero(&server_addr, sizeof(server_addr));			/*清零*/
	server_addr.sin_family = AF_INET;					/*协议族*/
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	/*本地地址*/
	server_addr.sin_port = htons(PORT);				/*服务器端口*/
	
	/*绑定地址结构到套接字描述符*/
	err = bind(ss, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err < 0){/*出错*/
		printf("bind error\n");
		return -1;	
	}
	
	/*设置侦听*/
	err = listen(ss, BACKLOG);
	if(err < 0){										/*出错*/
		printf("listen error\n");
		return -1;	
	}
	
		/*主循环过程*/
	for(;;)	{
		socklen_t addrlen = sizeof(struct sockaddr);
		
		sc = accept(ss, (struct sockaddr*)&client_addr, &addrlen); 
		/*接收客户端连接*/
		if(sc < 0){							/*出错*/
			continue;						/*结束本次循环*/
		}	
		
		/*建立一个新的进程处理到来的连接*/
		pid = fork();						/*分叉进程*/
		if( pid == 0 ){						/*子进程中*/
			close(ss);						/*在子进程中关闭服务器的侦听*/
			process_conn_server(sc);		/*处理连接*/
		}else{
			close(sc);						/*在父进程中关闭客户端的连接*/
		}
	}
}



