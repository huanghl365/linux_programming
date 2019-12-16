#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
int main(void)
{
	int fd ;
	int nread;
	char blk[1024];
	fd = open("/home/mxc/stat.c", O_RDWR);
	if (fd == -1)
		printf("open error\n");
	//SEEK_SET 表示文件头(第一个字符)，0 表示相对文件头偏移0个字节 ，正数往右偏移
	lseek(fd, 1, SEEK_SET);
	
	//SEEK_END 表示文件尾(EOF), 0 表示相对文件尾偏移0个字节 ，负数往左偏移
	//lseek(fd, -1, SEEK_END);
	
	//文件指针指向哪里，就从哪里开始读取数据，不会去掉指针指向的那个字节
	nread = read(fd, blk, sizeof(blk)-1);
	printf("nread = %d\n", nread);
	printf("%s", blk);
	printf("\n");
	exit(0)	;
}
