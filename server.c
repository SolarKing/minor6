#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const int MAX_TICKETS = 10;



int main(int argc, char const *argv[])
{
  int ticketDatabase[MAX_TICKETS];

  initDatabase(ticketDatabase, MAX_TICKETS);


  printf("Hello World\n");
  return 0;
}

void initDatabase(int *db[], int dbLength)
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