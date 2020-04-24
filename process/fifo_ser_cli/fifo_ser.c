#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "fifo_ser.h"

/*
程序功能描述：编写一个基于FIFO的服务端/客户端程序，多个客户端按照协议格式通过一个命名管道向服务端发送数据，
服务端处理之后通过另一个命名管道发送给客户端。(管道是单向的，所以创建两个)
*/

int main()
{
	int ser_fifo, cli_fifo;
	int ret;
	int i;
	int read_cnt =0 ;
	struct cli_msg climsg;
	char cli_fifo_name[50] = "\0";
	char *pchartmp = NULL;

	//创建管道
	ret = mkfifo(SERVER_FIFO, 0777);
	if (0 != ret)
	{
		perror("mkfifo");
		exit(1);
	}
	

	//读方式打开服务端管道
	ser_fifo = open(SERVER_FIFO, O_RDONLY);
	if (-1 == ser_fifo)
	{
		perror("open");
		exit(1);
	}

	//sleep(10);
	
	//读取客户端消息
	do	
	{
		ret = read(ser_fifo, &climsg, sizeof(cli_msg));
		if (ret > 0)
		{
			//打开客户端管道
			//printf("cli_pid = %d\n", climsg.cli_pid);
			sprintf(cli_fifo_name, CLIENT_FIFO, climsg.cli_pid);
			//printf("cli_fifo_name = %s\n",cli_fifo_name);
			cli_fifo = open(cli_fifo_name, O_WRONLY);
			if (-1 == ser_fifo)
			{
				perror("open");
				exit(1);
			}
			else
			{
				
				//处理及发送消息
				pchartmp = climsg.cli_msg_buffer;
				printf("cli_msg_buffer = %s\n", pchartmp);
				
				 while (*pchartmp) 
				 {
                	*pchartmp = toupper(*pchartmp);
                	pchartmp++;
           		 }
				
				 //printf("after proc, cli_msg_buffer = %s\n", climsg.cli_msg_buffer);
				
				 write(cli_fifo, climsg.cli_msg_buffer, strlen(climsg.cli_msg_buffer));
				 close(cli_fifo);
			
			}
						
		}
	}while(1);

	close(ser_fifo);
	unlink(SERVER_FIFO);
	return 0;
}

