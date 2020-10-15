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
程序功能说明：测试fopen使用不同方式打开对fseek使用的影响
*/
int main(void)
{
	FILE *fp = NULL;  
	char read_buffer[READ_SIZE] = "\0";
	char write_buffer[WRITE_SIZE] = "123456789";
	long int filesize = 0;
	fp = fopen("test.txt", "r+");    //更新方式打开，读写
	//fp = fopen("test.txt", "a+");  //更新方式打开，接续写
	if (NULL == fp)
	{
		perror("fopen");
		return -1;
	}
/*
测试：fopen分别使用r+和a+打开时，对fseek定位读的影响
测试结果：r+和a+的效果一样，默认都是从头开始读，并且可以定位到任意位置进行读取
*/

#if 1
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
#endif

	
	
/*
测试：fopen分别使用r+和a+打开时，对fseek定位写的影响
测试结果：
如果是r+, 则默认从头开始写，并且可以定位到任意位置写入数据
如果是a+，则默认从尾部开始写，并且只能从尾部续写，无法通过fseek进行定位
*/
	
#if 1	
	fseek(fp, 2L, SEEK_SET);
	fwrite(write_buffer, 1, strlen(write_buffer), fp);

	fseek(fp, -2L, SEEK_END);
	fwrite(write_buffer, 1, strlen(write_buffer), fp);
#endif
	
	fclose(fp);
	fp = NULL;
	return 0;
}

