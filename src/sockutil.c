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
