#ifndef SOCKUTIL
#define SOCKUTIL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void hintsInit(struct addrinfo *hints);
int msgLen(char *msg);

#endif
