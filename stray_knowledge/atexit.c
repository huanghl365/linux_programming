#include <stdio.h>
#include <stdlib.h>
void fun1(void)
{
	printf("process over, call the fun1\n");
}
void fun2(void)
{
	printf("process over, call the fun2\n");
}

/*
return、exit、_exit的主要区别：
1、exit主要用来终止进程，正常终止一般使用exit(0)，异常终止使用exit(1),只要调用了exit，程序就会退出；
return只有在main函数中调用才能终止进程，函数调用时return表示调用堆栈的返回
2、_exit和exit功能基本一样，但是exit和return终止进程时能调用atexit注册的回调函数，_exit不会调用回调函数
*/

int main(void)
{
	//atexit注册进程终止处理函数,先注册的后执行,当
	//在main中执行return或者程序使用了exit调用时，会执行回调函数
	atexit(fun1);
	atexit(fun2);
	printf("hello world\n");
	return 0;
	//测试结果：先打印hello world再执行进程终止函数
}

