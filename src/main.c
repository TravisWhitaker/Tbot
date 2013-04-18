#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int addrstatus;
struct addrinfo hints;
struct addrinfo *addrInfo;
int tube;
int bindStatus;
socklen_t addr_size;

char port[5] = "6667\0";

int main(int argc, char *argv[])
{
	switch(argc)
	{
		case 2:
			break;
		default:
			printf("Not necessarily beautiful, but mutated...\n");
			return 0;
			break;
	}
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if((addrstatus = getaddrinfo(NULL,"9999",&hints,&addrInfo)) != 0)
	{
		printf("Couldn't run getaddrinfo ya bimbooo...\n");
		return 0;
	}

	tube = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
	if(tube == -1)
	{
		printf("Failed to build tube.\n");
		return 0;
	}

	bindStatus = bind(tube, addrInfo->ai_addr, addrInfo->ai_addrlen);
	if(bindStatus == -1)
	{
		printf("Failed to bind tube.\n");
		return 0;
	}
	return 0;
}
