#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

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
	int fd;
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
	
	//使用mmap建立一段共享内存，这里通过文件描述符建立对打开的文件到内存的映射
	//参数1：:0表示内存地址随机分配
	//参数2：共享内存的大小
	//参数3：表示内存段可读可写
	//参数4：MAP_SHARED表示把对共享内存的修改保存到磁盘文件
	//MAP_PRIVATE表示对内存段的修改只在本进程修改，修改不会保存到磁盘文件或者打开的文件中
	//参数5：文件描述符
	//参数:6：0表示经共享内存访问的文件中数据的起始偏移值为0
	//pmmap = (RECORD *)mmap(0, RECORD_NUM * sizeof(RECORD), PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	pmmap = (RECORD *)mmap(0, RECORD_NUM * sizeof(RECORD), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (pmmap != NULL)
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
	
	//把内存段的某个部分或者整段修改写回到被映射的文件中
	//好像不用这个调用也没什么影响，修改后就会自动写到打开的文件中
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