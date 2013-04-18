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

char address[1024]; //Store the address the user enters as a command line argument.
char port[5] = "6667\0"; //Hard code the IRCd port for now...

int main(int argc, char *argv[])
{
	switch(argc) //Scold the user if there isn't exactly one argument.
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

	memset(address,'\0',1024); //Make sure this memory is clear.
	strncpy(address,argv[1],1024); //Copy the first 1024 bytes of input.
	switch(address[1023]) //Make sure that the string is null-terminated.
	{
	case '\0':
		break;
	default:
		address[1023] = '\0';
		break;
	}

	hintsInit(&hints); //Set socket options, see sockutils.c

	//Ask the OS for DNS resolve and port availability:
	if(getaddrinfo(address,port,&hints,&addrInfo) != 0)
	{
		printf("Couldn't resolve domain name.\n");
		return 0;
	}

	//Create the socket. We're going to assume that the first entry in the linked
	//list 'addrinfo' is going to work, socket() will probably fail otherwise:
	tube = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
	switch(tube)
	{
	case -1:
		printf("Couldn't initialize socket.\n");
		return 0;
		break;
	default:
		break;
	}

	bindStatus = bind(tube, addrInfo->ai_addr, addrInfo->ai_addrlen);
	if(bindStatus == -1)
	{
		printf("Failed to bind tube.\n");
		return 0;
	}
	return 0;
}
