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

int addrstatus; //Store the return value of getaddrinfo().
struct addrinfo hints; //Struct to store the config to pass to getaddrinfo().
struct addrinfo *addrInfo; //Struct to store the output of getaddrinfo().
int tube; //Store the socket descriptor.
int bindStatus; //Store the return value of bind().

char address[1000];
char port[5] = "6667\0";

int main(int argc, char *argv[])
{
	switch(argc)
	{
	case 2:
		break;
	case 1:
	case 0:
		printf("Too few arguments...\n");
		return 0;
		break;
	default:
		printf("Too many arguments...\n");
		return 0;
		break;
	}

	hintsInit(&hints);

	if((addrstatus = getaddrinfo(NULL,"9999",&hints,&addrInfo)) != 0)
	{
		printf("She's out of sync...\n");
		return 0;
	}

	tube = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
	if(tube == -1)
	{
		printf("The Internet is not a truck, but rather a series of tubes...\n");
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
