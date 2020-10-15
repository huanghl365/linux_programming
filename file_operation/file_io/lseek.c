#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
程序功能描述：测试open使用不同方式打开对lseek使用的影响
*/
int main(void)
{
	int fd ;
	int nread,ret;
	char blk[512] = "\0";
	char write_buf[512] = "gggggg";
	int i=0;
	
	//使用echo "string" > file时，不会写入‘\0’,而是写入‘\n’
	
	//fd = open("test.txt", O_RDWR | O_APPEND);
	fd = open("test.txt", O_RDWR);
	if (fd == -1)
		printf("open error\n");


/*
测试：open分别是否添加O_APPEND，对lseek定位读的影响
测试结果：加不加O_APPEND效果一样，默认都是从头开始读，并且可以定位到任意位置进行读取
*/

#if 0
	nread = read(fd, blk, sizeof(blk)-1);
	printf("nread = %d read the string: %s\n", nread, blk);



	ret = lseek(fd, 1, SEEK_SET);
	printf("the offset is %d\n", ret);
	//文件指针指向哪里，就从哪里开始读取数据，不会去掉指针指向的那个字节
	memset(blk, 0, sizeof(blk));
	nread = read(fd, blk, sizeof(blk)-1);
	printf("nread = %d read the string: %s\n", nread, blk);



	ret = lseek(fd, -1, SEEK_END);
	printf("the offset is %d\n", ret);
	memset(blk, 0, sizeof(blk));
	nread = read(fd, blk, sizeof(blk)-1);
	printf("nread = %d read the string: %s\n", nread, blk);



	//使用lseek测试文件的大小
	ret = lseek(fd, 0, SEEK_END);
	printf("the size of file is %d\n", ret);
#endif 


/*
测试：open分别是否添加O_APPEND，对lseek定位写的影响
测试结果：
如果不加O_APPEND, 则默认从头开始写，并且可以定位到任意位置写入数据
如果是加了O_APPEND，则默认从尾部开始写，并且只能从尾部续写，无法通过lseek进行定位
*/

#if 1
	ret = lseek(fd, 0, SEEK_SET);
	printf("the offset is %d\n", ret);

	write(fd, write_buf, strlen(write_buf));
#endif
	close(fd);
	return 0;
}


