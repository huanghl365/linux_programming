#include <stdio.h>
//段错误例子,库函数使用出错
#if 1
char *string;
int main()
{
	printf("enter string:");
	gets(string);
	printf("the string is %s\n", string);
	return 0;
}
#endif 

//段错误例子,库函数使用出错
#if 0
int main()
{
   int code = 0;
   scanf("%d",code);
   printf("%d\n",code);
   return 0;
}
#endif 

//简单调试程序
#if 0
main(int argc,char *argv[]){
    int i,j;
    j=0;
    for(i=0;i<10;i++){
        j+=5;
        printf("now a=%d\n", j);
    }
}

#endif

	

//段错误例子
#if 0
char *pstr = "hello";
int main()
{
	pstr[1]	 = 'T';
	return 0;
}
#endif 





