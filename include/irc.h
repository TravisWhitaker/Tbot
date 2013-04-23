#ifndef IRC
#define IRC

void initIRC(int tube);
int join(int tube, char *channel);
void getLine(int tube, char *outputBuffer);
int pingpong(int tube, char *line);
int tryReconnect(int *tube, char *line, struct addrinfo *addrInfo, unsigned int delay);
int kickDetect(int tube, char *line, char *channel, int delay);

#endif
