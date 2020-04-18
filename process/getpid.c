#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t PID = -1;
	pid_t PPID = -1;
	uid_t UID = -1;
	uid_t EUID = -1;
	PID = getpid(); //获取当前进程的PID，返回类型是pid_t
	PPID = getppid();//获取当前进程的PPID，返回类型是pid_t
	printf("PID = %d\n", PID);
	printf("PPID = %d\n", PPID);
	
	
	UID = getuid();
	EUID = geteuid();
	printf("UID = %d\n", UID);	//获取当前进程的用户ID
	printf("EUID = %d\n", EUID);
	
	return 0;
}

