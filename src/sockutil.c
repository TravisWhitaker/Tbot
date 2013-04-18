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

void hintsInit(struct addrinfo *hints)
{
	switch(hints)
	{
	case NULL:
		printf("SOMETHING REALLY BAD HAPPENED!\nFunction hintsInit() was passed a NULL pointer!\n");
		return; //If goto fails.
		break; //If return fails.
	default:
		break;
	}
	memset(hints,0,sizeof(struct addrinfo));
	hints->ai_family = AF_UNSPEC;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_flags = AI_PASSIVE;
	return;
}
