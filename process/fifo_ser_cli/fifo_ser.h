#ifndef _FIFO_SER_H_
#define _FIFO_SER_H_
//协议结构体
typedef struct cli_msg
{
	int cli_pid; //保存客户端pid
	char cli_msg_buffer[50];
} cli_msg;

#define SERVER_FIFO	"server_fifo"
#define CLIENT_FIFO "client_%d_fifo"
#define BUF_SIZE PIPE_BUF

#endif 

