#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    char buf[256];
    struct tm *tmpstr;
    time_t thetime;
    time(&thetime);
    tmpstr = localtime(&thetime);
    strftime(buf, 256, "%Y-%m-%d %A %H:%M:%S", tmpstr);
    printf("%s\n", buf);

}
