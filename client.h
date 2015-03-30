#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#ifndef CLIENT_H       /* Include guard */
#define CLIENT_H    

static const int BUFFER_SIZE = 256;

void error(char * msg);
int sendMessage(char *msg, int socket);
void initDatabase(int *db, int dbLength);

#endif
