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
#include <constructor.h>
#include <irc.h>

char defaultNick[] = "tbot\0";
char realname[] = "The T Robot\0";

void initIRC(int tube)
{
	char *nick = malloc(sizeof(char)*15);
	char *num = malloc(sizeof(char)*6);
	char *buffer = malloc(sizeof(char));
	*buffer = '\0';
	do
	{
		recv(tube,buffer,1,0);
	} while(*buffer != 'F');
	buffer = realloc(buffer,1024);
	recv(tube,buffer,1024,0);
	strncpy(nick,defaultNick,5);
	for(int i=0;i<1024;i++)
	{
		memset(buffer,'\0',1024);
		switch(i)
		{
		case 0:
			break;
		default:
			memset(num,'\0',6);
			sprintf(num,"%d",i);
			strncpy(nick+4,num,6);
			break;
		}
		printf("Trying nick %s...\n",nick);
		send(tube,nickConstruct(nick),msgLen(nickConstruct(nick)),0);
		usleep(100000);
		if(recv(tube,buffer,1024,MSG_DONTWAIT) == -1)
		{
			printf("Registered successfully.\n");
			break;
		}
	}
	send(tube,userConstruct(nick,realname),msgLen(userConstruct(nick,realname)),0);
	buffer = realloc(buffer,1000000);
	memset(buffer,'\0',1000000);
	usleep(500000);
	recv(tube,buffer,1000000,0);
	free(buffer);
	free(nick);
	free(num);
	return;
}

//Join a channel and detect success or failure:
int join(int tube, char *channel)
{
	char tempjoin[] = "JOIN :#notsummer";
	char *nick = malloc(sizeof(char)*15);
	char *num = malloc(sizeof(char)*6);
	char *buffer = malloc(sizeof(char)*1025);
	strncpy(nick,defaultNick,5);
	for(int i=0;i<1024;i++)
	{
		memset(buffer,'\0',1024);
		switch(i)
		{
		case 0:
			break;
		default:
			memset(num,'\0',6);
			sprintf(num,"%d",i);
			strncpy(nick+4,num,6);
			break;
		}
		printf("Trying nick %s...\n",nick);
		send(tube,nickConstruct(nick),msgLen(nickConstruct(nick)),0);
		usleep(1000);
		send(tube,tempjoin,msgLen(tempjoin),0);
		memset(buffer,'\0',1025);
		usleep(1000000);
		getLine(tube,buffer);
		printf("Checking for channel join success...\n");
		printf("%s",buffer);
		for(int j=0;j<1024;j++)
		{
			if(*(buffer+j) == 'J')
			{
				if(*(buffer+j+1) == 'O' && *(buffer+j+2) == 'I' && *(buffer+j+3) == 'N' && *(buffer+j+4) == ' ' && *(buffer+j+5) == ':')
				{
					free(nick);
					free(num);
					free(buffer);
					return 1;
				}
			}
			if(*(buffer+j) == '\n')
			{
				break;
			}
		}
		printf("Trying next number...\n");
	}
	return 0;
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
					break;
				}
				break;
			default:
				return 0;
				break;
			}
			break;
		default:
			return 0;
			break;
		}
		break;
	default:
		return 0;
		break;
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

//Try to reconnect to the server upon disconnect, disregarding the reason,
//returns just like pingpong and can change the value of 'tube':
int tryReconnect(int *tube,char *line,struct addrinfo *addrInfo, unsigned int delay)
{
	switch(*(line))
	{
	case 'E':
		switch(*(line+1))
		{
		case 'R':
			switch(*(line+2))
			{
			case 'R':
				switch(*(line+3))
				{
				case 'O':
					switch(*(line+4))
					{
					case 'R':
						break; //u mad?
					default:
						return 0;
						break;
					}
					break;
				default:
					return 0;
					break;
				}
				break;
			default:
				return 0;
				break;
			}
			break;
		default:
			return 0;
			break;
		}
		break;
	default:
		return 0;
		break;
	}
	close(*tube);
	printf("Disconnected from the server, reconnecting in...\n");
	int i;
	for(i=delay;i>0;i--)
	{
		printf("%d...\n",i);
		usleep(1000000);
	}
	*tube = socket(addrInfo->ai_family,addrInfo->ai_socktype,addrInfo->ai_protocol);
	switch(*tube)
	{
	case -1:
		printf("Couldn't initialize socket.\n");
		usleep(1000000);
		return tryReconnect(tube,"ERROR",addrInfo,delay+5);
		break;
	default:
		break;
	}
	if(connect(*tube,addrInfo->ai_addr,addrInfo->ai_addrlen) == -1)
	{
		printf("Couldn't reconnect.\n");
		usleep(1000000);
		return tryReconnect(tube,"ERROR",addrInfo,delay+5);
	}
	else
	{
		printf("Success.\n");
		return 1;
	}
}

//Try to re-join the channel if a kick is detected:
int kickDetect(int tube, char *line, char *channel, int delay)
{
	char *nick = malloc(sizeof(char)*15);
	char *num = malloc(sizeof(char)*6);
	char *buffer = malloc(sizeof(char)*1025);
	char tempjoin[] = "JOIN :#notsummer";
	for(int i=0;i<1025;i++)
	{
		if(*(line+i) == 'K')
		{
			printf("Detected K\n");
			if(*(line+i+1) == 'I' && *(line+i+2) == 'C' && *(line+i+3) == 'K') //u mad?
			{
				printf("Detected ICK\n");
				break;
			}
		}
		if(*(line+i) == '\n' || i==1024)
		{
			free(nick);
			free(num);
			free(buffer);
			return 0;
		}
	}
	for(int i=0;i<1025;i++)
	{
		printf("Detected T\n");
		if(*(line+i) == 't')
		{
			if(*(line+i+1) == 'b' && *(line+i+2) == 'o' && *(line+i+3) == 't') //u mad?
			{
				printf("Detected bot\n");
				break;
			}
		}
		else if(*(line+i) == '\n')
		{
			free(nick);
			free(num);
			free(buffer);
			return 0;
		}
	}
	strncpy(nick,defaultNick,5);
	printf("Kicked from channel, re-joining in...\n");
	int i;
	for(i=delay;i>0;i--)
	{
		printf("%d...\n",i);
		usleep(1000000);
	}
	for(int i=0;i<1024;i++)
	{
		memset(buffer,'\0',1025);
		switch(i)
		{
		case 0:
			break;
		default:
			memset(num,'\0',6);
			sprintf(num,"%d",i);
			strncpy(nick+4,num,6);
			break;
		}
		printf("Trying nick %s...\n",nick);
		send(tube,nickConstruct(nick),msgLen(nickConstruct(nick)),0);
		usleep(1000);
		send(tube,tempjoin,msgLen(tempjoin),0);
		memset(buffer,'\0',1025);
		usleep(1000000);
		getLine(tube,buffer);
		for(int j=0;i<1025;i++)
		{
			if(*(buffer+j) == 'J')
			{
				if(*(buffer+j+1) == 'O' && *(buffer+j+2) == 'I' && *(buffer+j+3) == 'N' && *(buffer+j+4) == ' ' && *(buffer+j+5) == ':')
				{
					printf("Success.\n");
					free(nick);
					free(num);
					free(buffer);
					return 1;
				}
			}
			if(*(buffer+j) == '\n')
			{
				break;
			}
		}
	}
	printf("Failure.\n");
	free(nick);
	free(num);
	free(buffer);
	return kickDetect(tube,line,channel,delay+60);
}
