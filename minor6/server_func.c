#include "server.h"

void initDatabase(int *db, int dbLength)
{
  int i = 0;
  for (i = 0; i < dbLength; i++)
  {
    db[i] = 0;
  }
}

void error(char *msg)
{
  perror(msg);
  exit(1);
}

int generateTicket(int modifer)
{
  time_t t;
  srand((unsigned) time(&t));

  int pos1 = rand() % 10 * 10000;
  if (pos1 < 1) pos1 = 10000;

  // srand((unsigned) time(&t) + modifer + pos1);
  int pos2 = rand() % 10 * 1000;
  // if (pos2 < 1) pos2 = 1000;
  srand((unsigned) time(&t) + modifer - pos2);
  int pos3 = rand() % 10 * 100;
  // if (pos3 < 1) pos3 = 100;
  srand((unsigned) time(&t));
  int pos4 = rand() % 10 * 10;
  // if (pos4 < 1) pos4 = 10;
  // srand((unsigned) time(&t));
  int pos5 = rand() % 10 * 1;
  // if (pos5 < 1) pos5 = 1;

  // always returns a 5 digit number
  return pos1 + pos2 + pos3 + pos4 + pos5;
}

int pushIntoDatabase(int *db, int dbLength, char *errorMsg)
{
  int i;
  for (i = 0; i < dbLength; i++)  
  {
    if (db[i] < 1)
    {
      db[i] = generateTicket(i);
      return db[i];
    }
  }
  printf("%s: Limit = %d\n", errorMsg, dbLength);
  return -1;
}

int sendMessage(char *msg, int socket)
{
  return write(socket, msg, strlen(msg));
}

// int returnTicket(int *db, int dbLength, char *errorMsg)
// {
//   int i;
//   for (i=0; i < dbLength)
// }

