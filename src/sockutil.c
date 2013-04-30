#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <sockutil.h>

//Load the hints struct:
void hintsInit(struct addrinfo *hints)
{
	memset(hints,0,sizeof(struct addrinfo));
	hints->ai_family = AF_UNSPEC;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_flags = AI_PASSIVE;
	return;
}

//Find the length of a pending IRC line for send():
int msgLen(char *msg)
{
	register unsigned int i;
	for(i=0;i<1024;i++)
	{
		if(*(msg+i) == '\n')
		{
			break;
		}
	}
	return i+1;
}

void printResolve(struct addrinfo *addrInfo, char *address, char *port)
{
	char IPstr[INET6_ADDRSTRLEN];
	void *IPppt;

	if(addrInfo->ai_family == AF_INET)
	{
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)addrInfo->ai_addr;
		IPppt = &(ipv4->sin_addr);
	}
	else
	{
		struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)addrInfo->ai_addr;
		IPppt = &(ipv6->sin6_addr);
	}
	inet_ntop(addrInfo->ai_family, IPppt, IPstr, sizeof(IPstr));
	printf("%s resolved to %s, connecting on port %s...\n",address,IPstr,port);
}
