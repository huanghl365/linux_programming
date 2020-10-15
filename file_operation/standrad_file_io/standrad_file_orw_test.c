#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
程序功能说明：标准文件IO，读取一个文件，写入另一个文件
*/

#define READ_NUM  1024

int main(void)
{
	FILE *fp1, *fp2;  //指针前面一定加上*
	int read_ret,write_ret;
	char read_buffer[READ_NUM] = "\0";
	int read_cnt , write_cnt;
	fp1 = fopen("test1.txt", "a+");   			 //更新方式打开，接续写
	//fp1 = fopen("test1.txt", "w+");   		 //更新方式，清空文件
	fp2 = fopen("test2.txt", "r");   			 //读方式打开, 文件必须存在

	if (fp1 == NULL || fp2 == NULL)
	{
		perror("fopen");
		fp1 = NULL;
		fp2 = NULL;
		return -1;
	}

	while(1)
	{
		read_ret = fread(read_buffer, 1, READ_NUM, fp2); 
		printf("read_ret = %d\n", read_ret);
		
		if (read_ret == 0) //返回0，可能达到文件尾或者发生错误
		{
			if (ferror(fp1) != 0) //判断是不是到达文件尾
			{
				perror("fread");
				return -1;
			}				
			break;
			
		}
			
		write_ret = fwrite(read_buffer, 1, read_ret, fp1);
		if (write_ret != read_ret)
		{
			printf("fwrite failed\n");
			return -1;
		}
		
		fflush(fp1);   //将文件流中未写出的数据立即写出，不用加也可以
			
	}

	
	fclose(fp1);//关闭文件，隐含一次fflush
	fclose(fp2);
	fp1 = fp2 = NULL;
	return 0;
}



