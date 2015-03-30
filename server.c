#include "server.h"

int main(int argc, char const *argv[])
{
  int i;
  int ticket;
  int foundTicket;

  int returnStatus; // return status
  int sockfd;
  int newsockfd;

  int numClients;
  char buffer[1025];

  socklen_t clientLength;
  socklen_t serverLength;

  struct sockaddr_un clientAddress;
  struct sockaddr_un serverAddress;

  int ticketDatabase[MAX_TICKETS];
  initDatabase(ticketDatabase, MAX_TICKETS);

  if (argc < 2) { error("ERROR no port provided"); }

  // for (i = 0; i < MAX_TICKETS+2; i++)
  // {
  //   n = buyTicket(ticketDatabase, MAX_TICKETS, "ERROR");  
  //   printf("The return value is %d\n", n);
  // }

  printf("Enter the # of clients allowed: ");
  scanf("%d", &numClients);

  sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

  if (sockfd < 0) { error("ERROR on creating socket"); }

  memset((void *) &serverAddress, '\0', sizeof(serverAddress));

  // get the address from the first argument of the command line
  serverAddress.sun_family = AF_UNIX;
  strcpy(serverAddress.sun_path, argv[1]);
  serverLength = strlen(serverAddress.sun_path) + sizeof(serverAddress.sun_family);

  if (bind(sockfd, (struct sockaddr *) &serverAddress, serverLength) < 0)
  {
    error("ERROR on binding socket");
  }

  printf("Socket created!!!\n");

  while(numClients > 0)
  {
    printf("\nWaiting for a client...\n");
    listen(sockfd, 1);

    clientLength = sizeof(clientAddress);
    newsockfd = accept(sockfd, (struct sockaddr *) &clientAddress, &clientLength);

    if (newsockfd < 0) { error("ERROR on accepting"); }

    memset((void *) buffer, '\0', 1025);
    returnStatus = read(newsockfd, buffer, sizeof(buffer)); // receive command
    printf("debug: buffer = %s\n", buffer);


    if (returnStatus < 0) { error("ERROR on reading socket"); }

    printf("The buffer reads: \"%s\"\n", buffer);

    // command handling starts here!!!

    if (strcmp(buffer, "buy") == 0)
    {
      printf("The client is buying a ticket...\n");
      returnStatus = pushIntoDatabase(ticketDatabase, MAX_TICKETS, "Tickets sold out!");
      printf("debug: about to send ticket %d\n", returnStatus);
      if (returnStatus < 0)
      {
        sendMessage("error", newsockfd);
        memset((void *) buffer, '\0', 1025);
        read(sockfd, buffer, sizeof(buffer)); // expect "waiting"
        sendMessage("Tickets sold out! Sorry!", newsockfd);
      }
      else
      {
        sendMessage("no error", newsockfd);

        memset((void *) buffer, '\0', 1025);
        read(sockfd, buffer, sizeof(buffer)); // expect "waiting"

        memset((void *) buffer, '\0', 1025);
        sprintf(buffer, "%d", returnStatus);
        printf("debug: sending ticket %s\n", buffer);
        write(newsockfd, buffer, sizeof(buffer));
        memset((void *) buffer, '\0', 1025);
        printf("Ticket sold!\n");
      }
    }

    if (strcmp(buffer, "cancel") == 0)
    {
      sendMessage("waiting", newsockfd); // send "waiting" to client
      printf("The client is returning a ticket...\n");
      memset((void *) buffer, '\0', 1025);
      read(newsockfd, buffer, sizeof(buffer)); // recieve ticket from client
      printf("debug: buffer = %s\n", buffer);
      ticket = atoi(buffer);
      foundTicket = 0;
      for (i = 0; i < MAX_TICKETS; i++)
      {
        if (ticketDatabase[i] == ticket)
        {
          ticketDatabase[i] = 0;
          foundTicket = 1;
          i = MAX_TICKETS;
        }
      }
      if (foundTicket == 0)
      {
        sendMessage("error", newsockfd);
        memset((void *) buffer, '\0', 1025);
        read(sockfd, buffer, sizeof(buffer)); // expect "waiting"
        memset((void *) buffer, '\0', 1025);
        sendMessage("Could not refund ticket.", newsockfd);
      }
      else
      {
        sendMessage("no error", newsockfd);
        memset((void *) buffer, '\0', 1025);
        read(sockfd, buffer, sizeof(buffer)); // expect "waiting"
        sendMessage("Ticket returned and refunded!", newsockfd);
        printf("Ticket refunded!\n");
      }
    }

    if (strcmp(buffer, "end") == 0)
    {
     printf("Client is disconnecting...\n"); 
     numClients--;
    }

    close(newsockfd);
  }

  printf("Closing server...\n");
  close(sockfd);
  printf("Server closed.\n");

  return 0;
}
