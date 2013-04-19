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
	usleep(10000);
	recv(tube,buffer,1000000,0);
	printf("!!! SERVER !!!\n%s\n!!! !SERVER! !!!\n",buffer);
	free(buffer);
	return;
}
