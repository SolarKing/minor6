/**
 * TASKS
 * 1) Send the "buy" command to the server
 * 2) Send the "cancel" command to the server
 * 3) Store the ticket from the server
 * 4) Recieve error handling from the server
 */



#include <stdio.h>


void error(char *msg)
{
  perror(msg);
  exit(1);
}


int main(int argc, char const *argv[])
{
  printf("Hello World\n");
  return 0;
}