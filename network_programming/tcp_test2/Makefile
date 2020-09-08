#all:client server					#all规则，它依赖于client和server规则
#all:client_2 server_2					#all规则，它依赖于client和server规则
all:client_3 server_3					#all规则，它依赖于client和server规则

CC:=gcc
#CC:=arm-hisiv500-linux-gcc

client:process.o client.o               	#client规则，生成客户端可执行程序
	$(CC) -o client process.o client.o
server:process.o server.o 	                #server规则，生成服务器端可执行程序
	$(CC) -o server process.o server.o	
process.o:						#tcp_process.o规则，生成tcp_process.o
	$(CC) -c process.c -o process.o


client_2:process_2.o client.o               	#client规则，生成客户端可执行程序
	$(CC) -o client_2 process_2.o client.o
server_2:process_2.o server.o 	                #server规则，生成服务器端可执行程序
	$(CC) -o server_2 process_2.o server.o	
process_2.o:						#tcp_process.o规则，生成tcp_process.o
	$(CC) -c process_2.c -o process_2.o


client_3:process_3.o client.o               	#client规则，生成客户端可执行程序
	$(CC) -o client_3 process_3.o client.o
server_3:process_3.o server.o 	                #server规则，生成服务器端可执行程序
	$(CC) -o server_3 process_3.o server.o	
process_3.o:						#tcp_process.o规则，生成tcp_process.o
	$(CC) -c process_3.c -o process_3.o


clean:								#清理规则，删除client、server和中间文件
	rm -f client server  client_2 server_2 client_3 server_3 *.o
