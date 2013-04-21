#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//Construct nick msg string:
char* nickConstruct(char *nick)
{
	char *output = malloc(sizeof(char)*1025);
	strncpy(output,"NICK ",5);
	int i;
	for(i=0;i<1025;i++)
	{
		*(output+i+5) = *(nick+i);
		if(*(output+i+5) == '\0')
		{
			break;
		}
	}
	*(output+i+5) = '\n';
	return output;
}

//Construct user msg string:
char* userConstruct(char *user, char *realname)
{
	char *output = malloc(sizeof(char)*1025);
	strncpy(output,"USER ",5);
	int i;
	for(i=0;i<1025;i++)
	{
		*(output+i+5) = *(user+i);
		if(*(output+i+5) == '\0')
		{
			break;
		}
	}
	strncpy(output+i+5," 0 * :",6);
	int j = i+11;
	for(i=0;i<1025;i++)
	{
		*(output+j) = *(realname+i);
		if(*(output+j) == '\0')
		{
			break;
		}
		j++;
	}
	*(output+j) = '\n';
	return output;
}
