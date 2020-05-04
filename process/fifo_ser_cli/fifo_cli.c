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


//同时运行多个客户端，shell命令为：for i in 1 2 3 4 5;do  ./fifo_cli.out & done
int main()
{
	int ser_fifo, cli_fifo;
	struct cli_msg climsg;
	char cli_fifo_name[50] = "\0";
	char res_buffer[BUF_SIZE] = "\0";
	int i, ret;
	//写方式打开服务端管道
	ser_fifo = open(SERVER_FIFO, O_WRONLY);
	if (-1 == ser_fifo)
	{
		perror("open");
		exit(1);
	}
	
	
	//发送消息之前先打开客户端接收管道，避免服务端打不开
	climsg.cli_pid = getpid();
	sprintf(cli_fifo_name, CLIENT_FIFO, climsg.cli_pid);
	ret = mkfifo(cli_fifo_name, 0777);
	if (0 != ret)
	{
		perror("mkfifo");
		exit(1);
	}
		
	for (i=0; i<2; i++)
	{

		//向服务管道发送消息
		sprintf(climsg.cli_msg_buffer, "hello,I am process %d", climsg.cli_pid);
		write(ser_fifo, &climsg, sizeof(climsg));
		cli_fifo = open(cli_fifo_name, O_RDONLY);
		if (-1 == ser_fifo)
		{
			perror("open");
			exit(1);
		}
		else
		{
			//从接收管道接收消息
			memset(res_buffer, 0, sizeof(res_buffer));
			ret = read(cli_fifo, res_buffer, sizeof(res_buffer));
			if (ret >= 0)
			{
				printf("process %d receive: %s\n", getpid(), res_buffer);
				close(cli_fifo);
			}
			else
				break;
		}
		
	}
	//发送接收完毕，关闭并删除客户端管道文件
	close(cli_fifo);
	unlink(cli_fifo_name);
	return 0;
	
}
