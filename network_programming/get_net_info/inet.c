#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

/*
程序功能描述：介绍IP地址转换函数inet_aton、inet_addr、inet_ntoa的使用
*/
int main(int argc, char *argv[])
{
	struct in_addr ip,local,network;
	char addr1[]="192.168.1.1";			/*a.b.c.d类型的网络地址字符串*/
	char addr2[]="255.255.255.255";		/*二进制值为全1的IP地址对应的字符串*/
	char *str=NULL,*str2=NULL;
	
	int err = 0;

	/*测试函数inet_aton，点分十进制字符串 -> 二进制地址（网络字节序）*/	
	err = inet_aton(addr1, &ip);
	if(err){
		printf("inet_aton:string %s value is:0x%x\n",addr1, ip.s_addr);
	}else{
		printf("inet_aton:string %s  error\n",addr1);
	}
	printf("----------------------\n");
	
	
	/*测试inet_addr，点分十进制字符串 -> 二进制地址（网络字节序）
	注意：inet_addr不能转换255.255.255.255
	*/	
	ip.s_addr = inet_addr(addr1);
	if(ip.s_addr != -1){
		printf("inet_addr:string %s value is:0x%x\n",addr1, ip.s_addr);

	}else{
		printf("inet_addr:string %s  error\n",addr1);
	};
	ip.s_addr = inet_addr(addr2);
	if(ip.s_addr != -1){
		printf("inet_addr:string %s value is:0x%x\n",addr2, ip.s_addr);	
	}else{
		printf("inet_addr:string %s  error\n",addr2);
	};
	printf("----------------------\n");


	/* 测试inet_ntoa，二进制地址（网络字节序）-> 点分十进制字符串
	*/	
	ip.s_addr = inet_addr(addr1); //先把点分十进制转为网络字节序
	if(err != -1){
		printf("inet_addr:string %s value is:0x%x\n",addr1, ip.s_addr);
	}else{
		printf("inet_addr:string %s  error\n",addr1);
	};
	str = inet_ntoa(ip);	
	printf("inet_ntoa:string %s ip:0x%x \n",str,ip.s_addr);

	ip.s_addr = 192<<24|168<<16|1<<8|1; //不做字节序转换
	str = inet_ntoa(ip);	
	printf("inet_ntoa:string %s ip:0x%x \n",str,ip.s_addr);
	printf("----------------------\n");



	/*测试inet_ntoa，先测试192.168.1.1，再测试255.255.255.255，证明函数的不可重入性
	 */
	ip.s_addr = 192<<24|168<<16|1<<8|1;
	str = inet_ntoa(ip);
	ip.s_addr = 255<<24|255<<16|255<<8|255;
	str2 = inet_ntoa(ip);	
	printf("inet_ntoa:ip:0x%x string1 %s,pre is:%s \n",ip.s_addr,str2, str);
	printf("inet_ntoa:ip:0x%x string1 %s\n",ip.s_addr,str);
	printf("----------------------\n");
	
	
	/* 测试函数inet_lnaof，获取二进制地址(网络字节序)的二进制主机号 
	   测试函数inet_netof，获取二进制地址(网络字节序)的二进制网络号
	*/	
	inet_aton(addr1, &ip);
	local.s_addr = inet_lnaof(ip);
	printf("inet_lnaof:	local:0x%x \n",local.s_addr);
	network.s_addr = inet_netof(ip);
	printf("inet_netof: network:0x%x \n",network.s_addr);	
	
	return 0;
}



