#ifndef IRC
#define IRC

void initIRC(int tube);
void getLine(int tube, char *outputBuffer);
int pingpong(int tube, char *line);

#endif
