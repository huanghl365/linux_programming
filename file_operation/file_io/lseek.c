#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
函数原型：off_t lseek(int fd, off_t offset, int whence);
描述：以whence基准，将文件位置偏移offset（指针偏移量）个字节的位置
参数：
fd：文件描述符
offset：偏移量，正数表示正向偏移，负数表示负向偏移
whence：设定从文件的哪里开始偏移,可能取值为：SEEK_CUR、SEEK_END、SEEK_SET等
SEEK_SET： 文件开头
SEEK_CUR： 当前位置
SEEK_END： 文件结尾
返回值：
返回结果偏移位置，以从文件开头开始的字节数为单位。 发生错误时，将返回值（off_t）-1，并设置errno

*/
/*
程序功能描述：测试leek的使用
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


	/*定位读测试*/
	/*
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

	/*定位写测试*/
	/*
	测试结果：
	如果不加O_APPEND, 则默认从头开始写，并且可以定位到任意位置写入数据
	如果是加了O_APPEND，则默认从尾部开始写，并且只能从尾部续写，无法通过lseek进行定位
	*/
	ret = lseek(fd, 0, SEEK_SET);
	printf("the offset is %d\n", ret);

	write(fd, write_buf, strlen(write_buf));

	close(fd);
	return 0;
}


