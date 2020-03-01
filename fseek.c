#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define READ_SIZE  1024
#define WRITE_SIZE  1024
/*
函数原型：int fseek(FILE *stream, long offset, int whence);
描述：以whence基准，文件流指针偏移offset（指针偏移量）个字节的位置
参数：
stream：文件流指针
offset：偏移量，正数表示正向偏移，负数表示负向偏移
whence：设定从文件的哪里开始偏移,可能取值为：SEEK_CUR、SEEK_END、SEEK_SET
SEEK_SET： 文件开头
SEEK_CUR： 当前位置
SEEK_END： 文件结尾
返回值：
成功，返回0，失败返回非0值，并设置error的值
*/

/*
程序功能说明：测试fseek的使用
*/
int main(void)
{
	FILE *fp = NULL;  //指针前面一定加上*
	char read_buffer[READ_SIZE] = "\0";
	char write_buffer[WRITE_SIZE] = "123456789";
	long int filesize = 0;
	fp = fopen("test.txt", "r+");    //更新方式打开，读写
	//fp = fopen("test.txt", "a+");  	//更新方式打开，接续写
	if (NULL == fp)
	{
		perror("fopen");
		return -1;
	}

	/*定位读测试*/
	/*测试结果：r+和a+的效果一样，默认都是从头开始读，并且可以定位到任意位置进行读取*/
	fread(read_buffer, 1, READ_SIZE-1, fp);
	printf("read the string:%s\n", read_buffer);

	fseek(fp, 2L,  SEEK_SET);
	memset(read_buffer, 0, sizeof(read_buffer));
	fread(read_buffer, 1, READ_SIZE-1, fp);
	printf("read the string:%s\n", read_buffer);
	
	fseek(fp, -2L, SEEK_END);
	memset(read_buffer, 0, sizeof(read_buffer));
	fread(read_buffer, 1, READ_SIZE-1, fp);
	printf("read the string:%s\n", read_buffer);

	fseek(fp, 0L, SEEK_END);
	memset(read_buffer, 0, sizeof(read_buffer));
	fread(read_buffer, 1, READ_SIZE-1, fp);
	printf("read the string:%s\n", read_buffer);
	
	/*定位写测试*/
	/*
	测试结果：
	如果是r+, 则默认从头开始写，并且可以定位到任意位置写入数据
	如果是a+，则默认从尾部开始写，并且只能从尾部续写，无法通过fseek进行定位
	*/
	
	fseek(fp, 2L, SEEK_SET);
	fwrite(write_buffer, 1, strlen(write_buffer), fp);

	fseek(fp, -2L, SEEK_END);
	fwrite(write_buffer, 1, strlen(write_buffer), fp);
	
	
	fclose(fp);
	fp = NULL;
	return 0;
}

