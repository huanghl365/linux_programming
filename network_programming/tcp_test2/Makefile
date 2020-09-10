all:client server					#all规则，它依赖于client和server规则

client:process.o client.o	#client规则，生成客户端可执行程序
	gcc -o client process.o client.o
server:process.o server.o	#server规则，生成服务器端可执行程序
	gcc -o server process.o server.o	
process.o:						#process.o规则，生成process.o
	gcc -c process.c -o process.o
clean:								#清理规则，删除client、server和中间文件
	rm -f client server *.o
