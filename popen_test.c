#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{

    FILE *read_fp = NULL;
    char read_buf[32] = {0};
    char *strptr = NULL;
    char readcnt = 0;
    read_fp = popen("ls ./", "r");
    if (read_fp != NULL)
    {
	while(1)
	{
	   //strptr = fgets(read_buf, 32, read_fp);
	    memset(read_buf, 0 ,sizeof(read_buf));
	    readcnt = fread(read_buf, sizeof(char), 32-1, read_fp);
	    //if (strptr != NULL)
	    if (readcnt > 0)
	    {
		//printf("get the str: %s\n", strptr);
		printf("get the str: %s\n", read_buf);
	    }
	    else
	    {
		perror("");
		break;
	    }
	}
	pclose(read_fp);
    }
    return 0;
}
