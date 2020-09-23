#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

/*
程序功能描述：测试mmap系统调用，打开一个文件建立内存映射，以结构体数组的方式进行读
写操作（比read write直接操作文件方便得多），并将修改的结果保存到文件中
*/

extern int errno;
#define STR_LEN 30
#define RECORD_NUM 100
typedef struct record
{
	int num;
	char str[STR_LEN];
}RECORD;

int main(void)
{
	
	FILE *fp = NULL;
	int i = 0;
	int fd = -1;
	int nread = 0;
	RECORD record_tmp = {0, "\0"};
	RECORD *pmmap = NULL;
	//memset(record_tmp, 0, sizeof(record_tmp));
	
	fp = fopen("/home/mxc/testfile.dat", "w+");
	if (fp != NULL)
	{
		for (i=0; i<RECORD_NUM; i++)
		{
			record_tmp.num = i;
			sprintf(record_tmp.str, "THIS IS RECORD STRING %d\n", i);
			fwrite(&record_tmp, sizeof(record_tmp), 1, fp);
		}
	}
	else
	{
		perror("open 1");
	}
	fclose(fp);
	
	fp = fopen("/home/mxc/testfile.dat", "r+");
	
	if (fp != NULL)
	{
		fseek(fp, 42 * sizeof(record_tmp), SEEK_SET);
		fread(&record_tmp, sizeof(record_tmp), 1, fp);
		printf("record_tmp.num = %d\n", record_tmp.num);
		printf("record_tmp.str = %s\n", record_tmp.str);
		//修改记录项
		record_tmp.num = 42 + 10;
		sprintf(record_tmp.str, "THIS IS RECORD STRING %d\n", 42 + 10);
		fseek(fp, 42 * sizeof(record_tmp), SEEK_SET);
		fwrite(&record_tmp, sizeof(record_tmp), 1, fp);
		
		fseek(fp, 42 * sizeof(record_tmp), SEEK_SET);
		fread(&record_tmp, sizeof(record_tmp), 1, fp);
		printf("record_tmp.num = %d\n", record_tmp.num);
		printf("record_tmp.str = %s\n", record_tmp.str);
		
	}
	else
	{
		perror("open 2");
	}
	fclose(fp);
	
	fd = open("/home/mxc/testfile.dat", O_RDWR);
	if (-1 == fd)
	{
		perror("open");
	}
	
	//使用mmap建立一段共享内存，这里通过文件描述符建立对打开的文件到内存的映射
	//pmmap = (RECORD *)mmap(0, RECORD_NUM * sizeof(RECORD), PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	pmmap = (RECORD *)mmap(0, RECORD_NUM * sizeof(RECORD), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (pmmap != MAP_FAILED) //MAP_FAILED : (void *) -1
	{
		pmmap[42].num = 100 + 42;
		sprintf(pmmap[42].str, "THIS IS RECORD STRING %d\n", 100 + 42);
		printf("pmmap[%d].num = %d\n", 42, pmmap[42].num);
		printf("pmmap[%d].str = %s\n", 42, pmmap[42].str);
	}
	else
	{
		perror("mmap");
	}
	
	//把内存段的某个部分或者整段修改写回到被映射的文件
	msync((void*)pmmap, RECORD_NUM * sizeof(RECORD), MS_ASYNC);
	
	lseek(fd, 42 * sizeof(record_tmp), SEEK_SET);
	nread = read(fd, &record_tmp, sizeof(RECORD));
	if (nread != 0)
	{
		printf("record_tmp.num = %d\n", record_tmp.num);
		printf("record_tmp.str = %s\n", record_tmp.str);
	}
	else
	{
		perror("read");
	}
	//释放内存映射
	munmap((void*)pmmap, RECORD_NUM * sizeof(RECORD));
	close(fd);
	
	
	return 0;
}
