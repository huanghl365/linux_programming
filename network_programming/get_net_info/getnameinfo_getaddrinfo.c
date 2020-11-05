/* hostname_to_ip hostname */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ip_to_hostname(const char* ip)
{
    int ret = 0;

    if(!ip)
    {
        printf("invalid params\n");
        return -1;
    }

    struct addrinfo hints;
    struct addrinfo *res, *res_p;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME | AI_NUMERICHOST;
    hints.ai_protocol = 0;

    ret = getaddrinfo(ip, NULL, &hints, &res);
    if(ret != 0)
	{
        printf("getaddrinfo: %s\n", gai_strerror(ret));
        return -1;
    }

    for(res_p = res; res_p != NULL; res_p = res_p->ai_next)
    {
        char host[1024] = {0};
        ret = getnameinfo(res_p->ai_addr, res_p->ai_addrlen, host, sizeof(host), NULL, 0, NI_NAMEREQD);
        if(ret != 0)
        {
            printf("getnameinfo: %s\n", gai_strerror(ret));
        }
        else
        {
            printf("hostname: %s\n", host);
        }
    }

    freeaddrinfo(res);
    return ret;
}

int hostname_to_ip(const char* hostname)
{
    int ret = 0;
    if(!hostname)
    {
        printf("invalid params\n");
        return -1;
    }

    struct addrinfo hints;
    struct addrinfo *res, *res_p;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;
    hints.ai_protocol = 0;

    ret = getaddrinfo(hostname, NULL, &hints, &res);
    if(ret != 0)
    {
        printf("getaddrinfo: %s\n", gai_strerror(ret));
        return -1;
    }

    for(res_p = res; res_p != NULL; res_p = res_p->ai_next)
    {
        char host[1024] = {0};
        ret = getnameinfo(res_p->ai_addr, res_p->ai_addrlen, host, sizeof(host), NULL, 0, NI_NUMERICHOST);
        if(ret != 0)
        {
            printf("getnameinfo: %s\n", gai_strerror(ret));
        }
        else
        {
            printf("ip: %s\n", host);
        }
    }

    freeaddrinfo(res);
    return ret;
}

int main()
{
	hostname_to_ip("gateway.mj.sctel.com.cn");
	
	return 0;
}

