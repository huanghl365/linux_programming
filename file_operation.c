#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void open_test(void);
void lseek_test(void);
void perror_test(void);
void write_test(void);

void unlink_test(void);
void stat_fstat_lstat_test(void);
void chmod_chown_test(void);

void fopen_test(void);
void fdopen_test(void);
void fseek_test(void);
void fgets_gets_test(void);
void feof_ferror_test(void);
void scanf_test(void);
void getline_test(void);

int main()
{
	fopen_test();
	return 0;
}

/*
void snprintf_test(void)
{
	char dststr[10] = "\0";
	int 
	snprintf(dststr, 10, "123456789");
	
}*/

/*
FILE * fdopen(int fildes,const char * mode);
描述：fdopen取一个现存的文件描述符，并使一个标准的I / O流与该描述符相结合。此函数常用于由创建管道和网络通信通道函数获得的描述符。因为这些特殊类型的文件不能用标准I/O fopen函数打开，首先必须先调用设备专用函数以获得一个文件描述符，然后用fdopen使一个标准I/O流与该描述符相结合。
对于fdopen，type参数的意义则稍有区别。因为该描述符已被打开，所以 fdopen 当mode时为"w" 并不截断该文件。(例如，若该描述符原来是由 open函数打开的，该文件那时已经存在，则其O_TRUNC标志将决定是否截断该文件）。另外，fdopen 不能用于创建该文件（因为如若一个描述符引用一个文件，则该文件一定已经存在）。
参数：
fildes：已经打开的文件描述符
mode：文件打开方式，与fopen一样
返回值：
转换成功时返回指向该流的文件指针。失败则返回NULL，并把错误代码存在errno中。
*/
#define MAX_BUF_SIZE 256
void fdopen_test(void)
{
	int fd = -1;
	FILE *streamptr = NULL;
	char streambuf[MAX_BUF_SIZE] = "\0";
	char *searchptr = NULL;
	fd = open("./file_operation.c", O_RDONLY);
	if (-1 == fd)
	{
		perror("open");
		return ;
	}
	else
	{
		streamptr = fdopen(fd, "r");  //将文件描述符以文件流的只读方式打开
		if (NULL == streamptr)
		{
			perror("fdopen");
			streamptr = NULL;
			return ;
		}
		else
		{
			while (NULL != fgets(streambuf, MAX_BUF_SIZE, streamptr)) //循环读取文件的每行
			{
				if (NULL !=  (searchptr = strstr(streambuf, "printf")) ) //判断每行中是否有printf
				{
					printf("%s", searchptr);
					memset(streambuf, 0, sizeof(streambuf));
				}
			}
			close(fd);
			fclose(streamptr);
		}
		return ;
	}
}

/*
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
描述：getline用于读取一行字符直到换行符,包括换行符。它会生成一个包含一串从输入流读入的字符的字符串，直到以下情况发生会导致生成的此字符串结束。1）到文件结束，2）遇到函数的定界符，3）输入达到最大限度。

参数：
lineptr：指向存放该行字符的指针，如果是NULL，则由系统帮助malloc，请在使用完成后free释放。
n：如果是由系统malloc的指针，请填0。
stream：文件描述符
返回值：返回读取的字节数，失败返回-1。
*/
void getline_test(void)
{
	FILE *streamptr = fopen("./dir_operation.c", "r");
	char *lineptr = NULL;
	size_t len = 0;
	if (streamptr == NULL)
	{
		perror("fopen:");
	}
	else
	{
		perror(NULL);
		//传参时第一个参数只能传指针的取地址，不能传递指向指针的指针
		while (getline(&lineptr, &len, streamptr) != -1)
		{
			fprintf(stdout, "%s", lineptr);
		}
			
	}
	free(lineptr);
	fclose(streamptr);
	
}

void chmod_chown_test(void)
{
	struct stat statbuf;
	uid_t uid;
	gid_t gid;
	
	//只有是文件的属主，或者具有超级用户权限才能够改变文件的权限
	chmod("/home/mxc/testfile", S_IRUSR | S_IWUSR | S_IXUSR);
	chmod("/home/mxc/testfile", S_IRGRP | S_IWGRP | S_IXGRP);
	
	stat("/home/mxc/testfile", &statbuf);
	
	if (S_ISREG(statbuf.st_mode))   	//S_ISREG 用来判断是否为普通文件的宏定义
	{
		printf("regular file\n");
		if (S_IRUSR & statbuf.st_mode)  //S_IRUSR 属主具有读权限的掩码
			printf("own have read\n");
		if (S_IWUSR & statbuf.st_mode) 
			printf("own have write\n");
		if (S_IXUSR & statbuf.st_mode) 
			printf("own have execute\n");
		
		if (S_IRGRP & statbuf.st_mode)  //S_IRUSR 属主具有读权限的掩码
			printf("grp have read\n");
		if (S_IWGRP & statbuf.st_mode) 
			printf("grp have write\n");
		if (S_IXGRP & statbuf.st_mode) 
			printf("grp have execute\n");
	}
	printf("\n-----\n");
	
	uid = getuid();	//获取当前进程执行用户的用户ID
	gid = getgid(); //获取当前进程执行用户的用户组ID
	
	//如果要修改文件的属主，那么进程需要有超级用户的权限
	//1000 是 mxc的用户ID和组ID
	chown("/home/mxc/testfile", 1000, 1000); 
}

void scanf_test(void)
{
	char ch = '\0';
	char ch1 = '\0';
	char ch2 = '\0';
	int num = 9678;
	double num2 = 12.48568;
	printf("%010d\n",num); //字段限定长度为10，小于10以0填充，如果长度大于10，不截断
	printf("%010.4lf\n", num2); //字段限定长度为10（包括.），小数限定为4位（超过4位四舍五入），如果长度小于10以0填充，如果长度大于10，不截断
	
	//scanf("%c,%d,%lf", &ch, &num, &num2);  //空格作为分隔符，输入时要添加空格作为分隔符
	//scanf("%c %d %lf", &ch, &num, &num2);  //,作为分隔符，输入时要添加,作为分隔符
	scanf("helloworld:%c %d %lf", &ch, &num, &num2);   //转换空字符中如果加入了字符串，那么输入的时候也要输入字符串进行匹配
	printf("%c\n",ch);
	printf("%d\n",num);
	printf("%lf\n",num2);
	
	
	scanf("%c %c %c", &ch, &ch1, &ch2);  //第一个字符可以是空格，第二个开始不能是空格，因为空格被作为分隔符
	//scanf("%c%c%c", &ch, &ch1, &ch2); 	   //每个字符都能是空格
	//scanf函数并不够灵活并且有漏铜，可以使用fread和fgets，读取输入后对字符串进行分解
	printf("ch=%c\n",ch);
	printf("ch1=%c\n",ch1);
	printf("ch2=%c\n",ch2);
}

#define READ_SIZE 1024
void feof_ferror_test(void)
{
	FILE *fp1;  //指针前面一定加上*
	char ch,ch2;
	char read_buffer[READ_SIZE] = "\0";
	int ret =0 ;
	fp1 = fopen("/home/mxc/testfile", "r+");  //更新方式打开，读写


	while(NULL != fgets(read_buffer, READ_SIZE, fp1))  
	{
		printf("%s", read_buffer);
		memset(read_buffer, 0, READ_SIZE);
	} 
	
	//判断文件是否到达文件尾，如果到达文件尾，那么feof返回值为非零，否则返回值为0
	if (ret = feof(fp1) != 0)
	{
		printf("ret = %d the file is at end\n", ret);
	}
	
	//判断文件操作是否产生错误，有错误返回非零，否则返回0
	if (ferror(fp1) != 0)
	{
		printf("the file operation have error\n");
	}
	//clearerr(fp1); //清除文件指针指向的文件流的文件尾标识和错误标识
	
	if (feof(fp1) != 0)
	{
		printf("the file is at end\n");
	}
	fclose(fp1);
}

void fgets_gets_test(void)
{
	FILE *fp1;  //指针前面一定加上*
	char ch,ch2;
	char read_buffer[READ_SIZE] = "\0";
	fp1 = fopen("/home/mxc/testfile", "r+");  //更新方式打开，读写
	
	//fgets遇到三种情况读取结束：读取到n-1个字符；读取到换行符（保留换行符）；读到文件尾。fgets会给读取的字符串添加'\0'。
	//fgets返回一个指向读取字符串的指针;当读取到文件尾EOF时，返回空指针；出错时，返回空指针并设置errno
	fseek(fp1, 0, SEEK_SET);  
	while(NULL != fgets(read_buffer, READ_SIZE, fp1))  
	{
		printf("%s", read_buffer);
		memset(read_buffer, 0, READ_SIZE);
	}
	
	//gets读取到换行符停止读取（丢弃换行符）。gets会给读取的字符串添加'\0'。
	//gets返回一个指向读取字符串的指针;读取出错返回空指针
	//gets对读取字符串长度没有限制，因此最好使用fget替代
	memset(read_buffer, 0, READ_SIZE);
	/*if (NULL != gets(read_buffer))
		printf("%s", read_buffer);*/
	
	if (NULL != fgets(read_buffer, READ_SIZE, stdin))
		printf("%s", read_buffer);
	
	fclose(fp1);
}


#define READ_SIZE  1024
#define WRITE_SIZE  1024
void fseek_test(void)
{
	FILE *fp1;  //指针前面一定加上*
	char read_buffer[READ_SIZE] = "\0";
	char write_buffer[WRITE_SIZE] = "FUCK YOUR MOTHER";
	long int filesize = 0;
	//如果是r+,则读写都可以通过fseek定位到任意位置
	//如果是a+,则读可以通过fseek定位到任意位置，但是写只能接续写，无法通过fseek进行定位
	//fp1 = fopen("/home/mxc/testfile2", "r+");  //更新方式打开，读写
	fp1 = fopen("/home/mxc/testfile2", "a+");  //更新方式打开，接续写
	
	fseek(fp1, 10L, SEEK_SET);
	
	fread(read_buffer, 1, READ_SIZE, fp1);
	printf("%s\n", read_buffer);
	
	fseek(fp1, 10L, SEEK_SET);
	fwrite(write_buffer, 1, strlen(write_buffer), fp1);
	
	fseek(fp1, 0L, SEEK_END);
	filesize = ftell(fp1);		//配合fseek用来计算文件的大小
	printf("filesize = %ld\n", filesize);
	
	fclose(fp1);
}

#define READ_NUM  1024
void fopen_test(void)
{
	FILE *fp1, *fp2;  //指针前面一定加上*
	char read_buffer[READ_NUM] = "\0";
	int read_cnt , write_cnt;
	fp1 = fopen("./time_operation.c", "a+");  //更新方式打开，接续写
	//fp1 = fopen("./time_operation.c", "w+");  //更新方式打开，长度截短为0
	fp2 = fopen("./time_operation.c", "r");   //读方式打开
	
	
	if (fp1 != NULL && fp2 != NULL)
	{
		
		read_cnt = fread(read_buffer, 1, READ_NUM, fp2);
		printf("readcnt = %d\n",read_cnt);
		write_cnt = fwrite(read_buffer, 1, strlen(read_buffer), fp1);
		fflush(fp1);  //将文件流中未写出的数据立即写出
		printf("write_cnt = %d\n",write_cnt);
	}
	else
	{
		printf("fopen file failed\n");
	}
	fclose(fp1);//关闭文件，隐含一次fflush
	fclose(fp2);
}

void write_test(void)
{
	int nread = 0;
	int nwrite = 0;
	char readbuf[128];
	//0 表示标准输入的文件描述符   1表示标准输出的文件描述符
	//stdin 表示标准输入的文件指针	stdout表示标准输出的文件指针
	nread = read(0, readbuf, sizeof(readbuf));
	if (nread != -1 )
	{
	nwrite = write(1, readbuf, nread);
	}
}

void perror_test(void)
{
	int  out;
	char *pstr = NULL;
	out = open("/home/mxc/file.out", O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if (out < 0)
	{
		//根据error打印错误信息，可以自己加上提示字符串
		perror("open");
		
		//根据error返回错误信息
		pstr = strerror(errno);
		if (pstr != NULL)
		{
			printf("%s\n",pstr);
		}
	}	
	close(out);
}

void unlink_test(void)
{
	//使用open创建一个文件
	open("/home/mxc/testfile", O_CREAT|O_TRUNC|O_SYNC, S_IRUSR | S_IWUSR | S_IXUSR);
	
	//创建硬链接，成功返回0，失败返回-1
	link("/home/mxc/testfile","/home/mxc/hardlink");
	link("/home/mxc/testfile","/home/mxc/hardlink2");
	
	//创建一个软链接
	symlink("/home/mxc/testfile","/home/mxc/softlink");
	
	//删除文件
	unlink("/home/mxc/hardlink");
}

void stat_fstat_lstat_test(void)
{
	int fd = open("/home/mxc/file", O_RDWR);
	struct stat statbuf;
	stat("/home/mxc/file", &statbuf);
	if (S_ISREG(statbuf.st_mode))   //S_ISREG 用来判断是否为普通文件的宏定义
	{
		printf("regular file\n");
		if (S_IRUSR & statbuf.st_mode)  //S_IRUSR 属主具有读权限的掩码
			printf("have read\n");
		if (S_IWUSR & statbuf.st_mode) 
			printf("have write\n");
		if (S_IXUSR & statbuf.st_mode) 
			printf("have execute\n");
	}
	printf("\n-----\n");
	memset(&statbuf, 0, sizeof(statbuf));
	fstat(fd, &statbuf); //判断的对象是文件描述符而不是文件
	if (S_ISREG(statbuf.st_mode))   //S_ISREG 用来判断是否为普通文件的宏定义
	{
		printf("regular file\n");
		if (S_IRUSR & statbuf.st_mode)  //S_IRUSR 属主具有读权限的掩码
			printf("have read\n");
		if (S_IWUSR & statbuf.st_mode) 
			printf("have write\n");
		if (S_IXUSR & statbuf.st_mode) 
			printf("have execute\n");
	}
	
	printf("\n-----\n");
	memset(&statbuf, 0, sizeof(statbuf));
	lstat("/home/mxc/winshare", &statbuf); //当对象是软连接时，判断的对象是软连接本身而不是软连接指向的对象
	if (S_ISLNK(statbuf.st_mode))   //S_ISLNK 用来判断是否为软连接的宏定义
	{
		printf("link file\n");
		if (S_IRUSR & statbuf.st_mode)  //S_IRUSR 属主具有读权限的掩码
			printf("have read\n");
		if (S_IWUSR & statbuf.st_mode) 
			printf("have write\n");
		if (S_IXUSR & statbuf.st_mode) 
			printf("have execute\n");
	}
	
	close(fd);
}

void lseek_test(void)
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
}

void open_test(void)
{
	unsigned char ch[1024] = {'\0'};
	int in, out;
	int nread = 0;
	in = open("/home/mxc/testfile", O_RDONLY);
	//O_CREAT 创建文件，文件存在则不创建，O_TRUNC清空文件
	//S_IRUSR  S_IWUSR 设置创建文件的权限，与linux系统文件的umask 反值（反值不包括执行权限） 取与
	//out = open("/home/mxc/file.out", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	
	//O_APPEND 续写文件
	//out = open("/home/mxc/file.out", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
	
	//O_CREAT 创建文件， O_EXCL如果文件存在则报错
	out = open("/home/mxc/file.out", O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if (out < 0)
	{
		perror("open");
	}
	//read属于系统调用，每次调用都会有用户到内核空间的切换开销，因此加大每次读取字节，减小切换次数减小开销。
	while ((nread = read(in, ch, 1024)) > 0 )
	{
		
		write(out, ch, nread);
	}

	close(in);
	close(out);
}
