#include <sys/types.h>
#include <sys/socket.h>						/*包含socket()/bind()*/
#include <netinet/in.h>						/*包含struct sockaddr_in*/
#include <string.h>							/*包含memset()*/
#include <stdio.h>
#include <arpa/inet.h>


/*
程序功能描述：一个简单的udp回显程序，使用recvmsg和sendmsg进行数据的接收和发送
*/

#define PORT_SERV 8888/*服务器端口*/
#define BUFF_LEN 256									/*缓冲区大小*/
void static udpserv_echo(int s, struct sockaddr*client)
{
	int n;												/*接收数据长度*/
	char buff[BUFF_LEN];								/*接收发送缓冲区															*/
	socklen_t len;											/*地址长度*/
	struct iovec v;
	struct msghdr msg;									/*消息结构*/
	char ip[16];
	int port;
	struct sockaddr_in *client_addr;
	while(1)											/*循环等待*/
	{
		msg.msg_name = client;									/*保存客户端地址*/
		msg.msg_namelen = sizeof(struct sockaddr_in);			/*地址长度*/
		msg.msg_control = NULL;					/*没有控制域*/
		msg.msg_controllen = 0;					/*控制域长度为0*/
		msg.msg_iov = &v;						/*挂接向量指针*/
		msg.msg_iovlen = 1;						/*msg_iov元素数量*/
		msg.msg_flags = 0;						/*无特殊操作*/

		v.iov_base = buff;
		v.iov_len = BUFF_LEN;
		n = recvmsg(s, &msg, 0);

#if 1	//打印客户端地址
	memset(ip, 0, sizeof(ip));
	client_addr = (struct sockaddr_in*)client;
	inet_ntop(AF_INET,(void *)&(client_addr->sin_addr), ip, sizeof(ip));
	port = ntohs(client_addr->sin_port);
	printf("get message from %s[%d]\n", ip, port);

#endif

		v.iov_base = buff;
		v.iov_len = n;

		sendmsg(s, &msg, 0);/*将接收到的n个字节发送回客户端*/
	}	
}

int main(int argc, char*argv[])
{
	int s;									/*套接字文件描述符*/
	struct sockaddr_in addr_serv,addr_clie;		/*地址结构*/
	
	s = socket(AF_INET, SOCK_DGRAM, 0);			/*建立数据报套接字*/
	
	memset(&addr_serv, 0, sizeof(addr_serv));		/*清空地址结构*/
	addr_serv.sin_family = AF_INET;				/*地址类型为AF_INET*/
	addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);	/*任意本地地址*/
	addr_serv.sin_port = htons(PORT_SERV);			/*服务器端口*/
	
	bind(s, (struct sockaddr*)&addr_serv, sizeof(addr_serv));
													/*绑定地址*/
	udpserv_echo(s, (struct sockaddr*)&addr_clie);	/*回显处理程序*/
	
	return 0;	
}
