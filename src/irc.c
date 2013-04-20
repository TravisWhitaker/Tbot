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

char nick[11] = "NICK tbot\n";
char user[21] = "USER tbot 0 * :tbot\n";

void initIRC(int tube)
{
	char *buffer = malloc(sizeof(char));
	*buffer = '\0';
	do
	{
		recv(tube,buffer,1,0);
	} while(*buffer != 'F');
	buffer = realloc(buffer,1024);
	recv(tube,buffer,1024,0);
	send(tube,nick,10,0);
	send(tube,user,20,0);
	buffer = realloc(buffer,1000000);
	memset(buffer,'\0',1000000);
	usleep(500000);
	recv(tube,buffer,1000000,0);
	free(buffer);
	return;
}

//Grab exactly one \n terminated line of output from the server
//that is safe for printing:
void getLine(int tube, char *outputBuffer)
{
	char *buffer = malloc(sizeof(char)*1025);
	memset(buffer,'\0',1025);
	memset(outputBuffer,'\0',1025);
	for(int i=0;i<1024;i++)
	{
		if(recv(tube,buffer+i,1,MSG_DONTWAIT) == -1 || *(buffer+i) == '\n')
		{
			strncpy(outputBuffer,buffer,1025);
			free(buffer);
			return;
		}
	}
	return;
}

//Check to see if the line is a ping. If it is, ping back and return 1,
//else, return 0:
int pingpong(int tube,char *line)
{
	switch(*(line))
	{
	case 'P':
		switch(*(line+1))
		{
		case 'I':
			switch(*(line+2))
			{
			case 'N':
				switch(*(line+3))
				{
				case 'G':
					break; //u mad?
				default:
					return 0;
				}
				break;
			default:
				return 0;
			}
			break;
		default:
			return 0;
		}
		break;
	default:
		return 0;
	}
	char *buffer = malloc(sizeof(char)*1024);
	memset(buffer,'\0',1024);
	strncpy(buffer,"PONG :",6);
	int i;
	for(i=6;i<1024;i++)
	{
		*(buffer+i) = *(line+i);
		if(*(buffer+i) == '\n')
		{
			break;
		}
	}
	send(tube,buffer,i+1,0);
	free(buffer);
	return 1;
}
