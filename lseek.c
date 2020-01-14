#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
程序功能描述：详细分析lseek对文件指针的操控
*/
int main(void)
{
	int fd ;
	int nread;
	char blk[1024] = "\0";
	int i=0;
	fd = open("test.txt", O_RDWR);
	if (fd == -1)
		printf("open error\n");
	//SEEK_SET 表示文件头(第一个字符)，0 表示相对文件头偏移0个字节 ，正数往右偏移
	nread = lseek(fd, 1, SEEK_SET);
	printf("the offset of point is %d\n", nread);
	
	//SEEK_END 表示文件尾(EOF), 0 表示相对文件尾偏移0个字节 ，负数往左偏移
	nread = lseek(fd, -1, SEEK_END);
	printf("the offset of point is %d\n", nread);


	//使用echo "string" > file时，不会写入‘\0’,而是写入‘\n’
	//使用lseek测试文件的大小
	nread = lseek(fd, 0, SEEK_END);
	printf("the size of file is %d\n", nread);


	//文件指针指向哪里，就从哪里开始读取数据，不会去掉指针指向的那个字节
	nread = read(fd, blk, sizeof(blk)-1);
	printf("nread = %d\n", nread);
	printf("%s", blk);

	close(fd);
	return 0;
}


