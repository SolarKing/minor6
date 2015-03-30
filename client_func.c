#include "client.h"

void error(char * msg)
{
  perror(msg);
  exit(1);
}

int sendMessage(char *msg, int socket)
{
  return write(socket, msg, sizeof(msg));
}

// int returnTicket(int *db, int dbLength, char *errorMsg)
// {
//   int i;
//   for (i=0; i < dbLength)
// }


void initDatabase(int *db, int dbLength)
{
  int i = 0;
  for (i = 0; i < dbLength; i++)
  {
    db[i] = 0;
  }
}