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


/*
void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
描述：mmap将一个文件或者其它对象映射进内存。文件被映射到多个页上，如果文件的大小不是所有页的大
小之和，最后一个页不被使用的空间将会清零。
参数：
addr：映射区的开始地址，设置为0时表示由系统决定映射区的起始地址。
length：映射区的长度，长度单位是 以字节为单位，不足一内存页按一内存页处理。
prot：期望的内存保护标志，不能与文件的打开模式冲突。是以下的某个值，可以通过or运算合理地组合在一起
PROT_EXEC  页内容可以被执行
PROT_READ  页内容可以被读取
PROT_WRITE 页可以被写入
PROT_NONE  页不可访问
flags：指定映射对象的类型，映射选项和映射页是否可以共享。它的值可以是一个或者多个标志的组合体，这里只列举两个常用的
MAP_SHARED  与其它所有映射这个对象的进程共享映射空间。对共享区的写入，相当于输出到文件。直到msync()或者munmap()被调用，文件实际上不会被更新。
MAP_PRIVATE 建立一个写入时拷贝的私有映射。内存区域的写入不会影响到原文件。这个标志和以上标志是互斥的，只能使用其中一个。
fd：有效的文件描述符。一般是由open()函数返回，其值也可以设置为-1，此时需要指定flags参数中的MAP_ANON,表明进行的是匿名映射。
off_toffset：被映射对象内容的起点。
返回值：
成功执行时，mmap()返回被映射区的指针，munmap()返回0。失败时，mmap()返回MAP_FAILED[其值为(void *)-1]，munmap返回-1。
*/

/*
程序功能描述：测试mmap系统调用，打开一个文件建立内存映射，以结构体数组的方式进行读写操作（比read write直接操作文件方便得多），并将修改的结果保存到文件中
*/
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