#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>

static const int MAX_TICKETS = 10;

/**
 * This initializes all the values in an integer array
 * to be zero.
 * @param db       The array to be modified
 * @param dbLength The length of the array to be modified
 * 
 *   USAGE:
 *   int someArray[10];
 *   initDatabase(someArray, 10);
 *   
 */
void initDatabase(int *db, int dbLength);

/**
 * This will print an error onto the screen and then exit()
 * the program.
 * @param msg    A c-style string containing a message
 *
 *  USAGE:
 *  error("ERROR something went wrong");
 *  
 */
void error(char *msg);

/**
 * This will generate a random 5 digit integer.
 * @param  modifier   This will modify the srand within the function to promote
 *                    true randomness 
 * @return Always returns a random 5 digit integer
 *
 *  USAGE:
 *  int result = generateTicket(14);
 *  printf("%d", result); --> "12345" to console
 *  
 */
int generateTicket();

/**
 * This will generate a ticket, place it into an integer array, and then
 * return the 5 digit value of said ticket.
 * If the integer array is already full (i.e. there are no 0s), then the return
 * value of this function will be -1.
 * @param  db       The array to be modified
 * @param  dbLength The length of the array to be modified
 * @param  errorMsg A c-style string containing an error message
 * @return          If error, return -1. Otherwise return the 5 digit value of the ticket
 *
 *  USAGE:
 *  int someArray[4] = {13564, 25548, 78896, 0};
 *  int result = buyTicket(someArray, 4, "This is the error message");
 *  printf("%d", result); --> i.e. "12345"
 *                        --> someArray[] == {13564, 25548, 78896, 12345};
 *  result = buyTicket(someArray, 4, "This is the error message");
 *  printf(%d, result);
 *  
 *    CONSOLE OUTPUT:
 *      12345
 *      This is the error message: Limit = 4
 *      -1    
 *    
 */
int buyTicket(int *db, int dbLength, char *errorMsg);

int main(int argc, char const *argv[])
{
  int i;
  int n;
  int sockfd;
  int newsockfd;
  int numClients;
  socklen_t serverLength;
  socklen_t clientLength;
  struct sockaddr_un clientAddress;
  struct sockaddr_un serverAddress;

  char buffer[1025];

  int ticketDatabase[MAX_TICKETS];
  initDatabase(ticketDatabase, MAX_TICKETS);

  
  for (i = 0; i < MAX_TICKETS+2; i++)
  {
    n = buyTicket(ticketDatabase, MAX_TICKETS, "ERROR");  
    printf("The return value is %d\n", n);
  }

  printf("Enter the # of clients allowed: ");
  scanf("%d", &numClients);

  sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

  if (sockfd < 0)
  {
    error("ERROR on creating socket");
  }

  memset((void *) &serverAddress, '\0', sizeof(serverAddress));
  // get the address from the first argument of the command line
  serverAddress.sun_family = AF_UNIX;
  strcpy(serverAddress.sun_path, argv[1]);
  printf("The server is running on address: %s\n", serverAddress.sun_path);

  serverLength = strlen(serverAddress.sun_path) + sizeof(serverAddress.sun_family);

  if (bind(sockfd, (struct sockaddr *) &serverAddress, serverLength) < 0)
  {
    error("ERROR on binding socket");
  }

  printf("Socket created!!!\n Waiting for a client!\n");

  while(numClients > 0)
  {
    listen(sockfd, 5);
    clientLength = sizeof(clientAddress);
    newsockfd = accept(sockfd, (struct sockaddr *) &clientAddress, &clientLength);
    if (newsockfd < 0)
    {
      error("ERROR on accepting");
    }

    n = read(newsockfd, buffer, sizeof(buffer));

    if (n < 0)
    {
      error("ERROR on reading socket");
    }

    if (strcmp(buffer, "buy"))
    {
      printf("The client is buying a ticket...\n");
      n = buyTicket(ticketDatabase, MAX_TICKETS, "There already too many tickets");
      if (n < 0)
      {
        // handle the error
      }

    }

    // place logic here
  }

  // ticketDatabase[0] = generateTicket();
  // printf("%d\n", ticketDatabase[0]);
  
  return 0;
}

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

  srand((unsigned) time(&t) + modifer + pos1);
  int pos2 = rand() % 10 * 1000;
  // if (pos2 < 1) pos2 = 1000;
  srand((unsigned) time(&t) + modifer - pos2);
  int pos3 = rand() % 10 * 100;
  // if (pos3 < 1) pos3 = 100;
  srand((unsigned) time(&t) - modifer);
  int pos4 = rand() % 10 * 10;
  // if (pos4 < 1) pos4 = 10;
  srand((unsigned) time(&t));
  int pos5 = rand() % 10 * 1;
  // if (pos5 < 1) pos5 = 1;

  // always returns a 5 digit number
  return pos1 + pos2 + pos3 + pos4 + pos5;
}

int buyTicket(int *db, int dbLength, char *errorMsg)
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

// int returnTicket(int *db, int dbLength, char *errorMsg)
// {
//   int i;
//   for (i=0; i < dbLength)
// }