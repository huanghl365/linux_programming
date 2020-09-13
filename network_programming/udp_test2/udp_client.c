#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>						/*包含socket()/bind()*/
#include <netinet/in.h>						/*包含struct sockaddr_in*/
#include <string.h>							/*包含memset()*/
#define PORT_SERV 8888						/*服务器端口*/
#define BUFF_LEN 256							/*缓冲区大小*/
static void udpclie_echo(int s, struct sockaddr*to)
{
	char buff[BUFF_LEN] = "UDP TEST WHAT ARE YOU DO";			/*发送给服务器的测试数据05	*/
	struct msghdr msg;						/*消息结构*/
	
	struct iovec v;

	msg.msg_name = to;												/*指定服务器地址*/
	msg.msg_namelen = sizeof(struct sockaddr_in);					/*服务器地址长度*/
	msg.msg_control = NULL;					/*没有控制域*/
	msg.msg_controllen = 0;					/*控制域长度为0*/
	msg.msg_iov = &v;						/*挂接向量指针*/
	msg.msg_iovlen = 1;						/*msg_iov元素数量*/
	msg.msg_flags = 0;						/*无特殊操作*/

	v.iov_base = buff;
	v.iov_len = BUFF_LEN;
	sendmsg(s, &msg, 0);						/*发送给服务器*/

	recvmsg(s, &msg, 0);	/*从服务器接收数据*/
												
	printf("recved:%s\n",buff);					/*打印数据*/
}

int main(int argc, char*argv[])
{
	int s;											/*套接字文件描述符*/
	struct sockaddr_in addr_serv;					/*地址结构*/
	
	s = socket(AF_INET, SOCK_DGRAM, 0);			/*建立数据报套接字*/
	
	memset(&addr_serv, 0, sizeof(addr_serv));		/*清空地址结构*/
	addr_serv.sin_family = AF_INET;				/*地址类型为AF_INET*/
	addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);	/*任意本地地址*/
	addr_serv.sin_port = htons(PORT_SERV);			/*服务器端口*/
	
	udpclie_echo(s, (struct sockaddr*)&addr_serv);	/*客户端回显程序*/
	
	close(s);
	return 0;	
}
