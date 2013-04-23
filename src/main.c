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
#include <irc.h>
#include <constructor.h>

int addrstatus; //Store the return value of getaddrinfo().
struct addrinfo hints; //Struct to store the config to pass to getaddrinfo().
struct addrinfo *addrInfo; //Struct to store the output of getaddrinfo().
int *tube; //Store the socket descriptor.

char address[1024]; //Store the address the user enters as a command line argument.
char port[6]; //Store the port to use.
char IPstr[INET6_ADDRSTRLEN]; //Store the host's IP as a string.

int main(int argc, char *argv[])
{
	//Allocate some memory for later:
	char *line = malloc(sizeof(char)*1025);
	tube = malloc(sizeof(int));

	switch(argc) //Scold the user if arguments are malformed.
	{
	case 3:
		break;
	case 2:
		strncpy(port,"6667\0",6);
		break;
	case 1:
	case 0:
		printf("Too few arguments...\n");
		goto terminate;
		break;
	default:
		printf("Too many arguments...\n");
		goto terminate;
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
	switch(port[0]) //Do the same thing for 'port.'
	{
	case '6':
		break;
	default:
		memset(port,'\0',6);
		strncpy(port,argv[2],6);
		switch(port[6])
		{
		case '\0':
			break;
		default:
			port[6] = '\0';
		}
	}

	hintsInit(&hints); //Set socket options, see sockutils.c

	//Ask the OS for DNS resolve and port availability:
	if(getaddrinfo(address,port,&hints,&addrInfo) != 0)
	{
		printf("Couldn't resolve domain name.\n");
		goto terminate;
	}

	//Now we're going to tell the user about the DNS resolution:
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

	//Create the socket. We're going to assume that the first entry in the linked
	//list 'addrinfo' is going to work, socket() will probably fail otherwise:
	*tube = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
	switch(*tube)
	{
	case -1:
		printf("Couldn't initialize socket.\n");
		goto terminate;
		break;
	default:
		break;
	}

	if(connect(*tube, addrInfo->ai_addr, addrInfo->ai_addrlen) == -1)
	{
		printf("Couldn't connect to host.\n");
		goto terminate;
	}

	//From here on out, we just pass the socket discriptor around:
	initIRC(*tube);

	if(join(*tube,NULL) == 0)
	{
		printf("Failed to join channel.\n");
		goto terminate;
	}

	char sup[] = "PRIVMSG #notsummer :'Sup bitches.\n";
	char imback[] = "PRIVMSG #notsummer :I'm back, bitches!\n";
	int counter = 0;
	while(1)
	{
		if(counter >= 60000)
		{
			send(*tube,sup,msgLen(sup),0);
			counter = 0;
		}
		usleep(1000);
		counter++;
		getLine(*tube,line);
		if(pingpong(*tube,line) == 1)
		{
			continue;
		}
		if(tryReconnect(tube,line,addrInfo,10) == 1)
		{
			initIRC(*tube);
			join(*tube,NULL);
			continue;
		}
//		if(kickDetect(*tube,line,NULL,10) == 1)
//		{
//			send(*tube,imback,msgLen(imback),0);
//			continue;
//		}
	}

	goto terminate;

	//Goto here to terminate. This way we control how memory is freed.
	terminate:
		freeaddrinfo(addrInfo);
		free(line);
		close(*tube);
		free(tube);
		printf("Socket closed.\n");
		return 0;
}
