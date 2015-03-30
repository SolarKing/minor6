#include "client.h"

int main(int argc, char const *argv[])
{
  
  
  int n;
  int port;
  int sockfd;

  int ticket = -1;

  struct sockaddr_in serverAddress;
  struct hostent *server;

  char buffer[BUFFER_SIZE];

  if (argc < 3)
  {
    fprintf(stderr, "Usage %s hostname port\n", argv[0]);
    exit(0);
  }

  port = atoi(argv[2]);

  int isConnected = 1;
  while(isConnected)
  {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) { error("ERROR on opening socket"); }

    server = gethostbyname(argv[1]);

    if (server == NULL)
    {
      fprintf(stderr, "ERROR no such host\n");
      exit(0);
    }

    memset((void *) &serverAddress, '\0', sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    memcpy((void *) server->h_addr, (void *) &serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
    {
      error("ERROR on connecting");
    } 

    printf("Please enter a command: ");
    memset((void *) buffer, '\0', 256);
    scanf("%s", buffer);

    printf("debug: wrote into the buffer: \"%s\"\n", buffer);

    // start conditionals

    if (strcmp(buffer, "buy") == 0)
    {
      n = write(sockfd, "buy", 3);
      memset((void *) buffer, '\0', 256);

      // read for error
      n = read(sockfd, buffer, sizeof(buffer));
      printf("debug: buffer = %s\n", buffer);

      sendMessage("waiting", sockfd); // send waiting

      if (strcmp(buffer, "error") == 0)
      {
        // read error message
        memset((void *) buffer, '\0', 256);
        n = read(sockfd, buffer, sizeof(buffer));
        printf("debug: buffer = %s\n", buffer);
        printf("(Server) Error: %s\n", buffer);
      }
      else if (strcmp(buffer, "no error") == 0)
      {
        memset((void *) buffer, '\0', 256);
        n = read(sockfd, buffer, sizeof(buffer));
        printf("debug: buffer = %s\n", buffer);
        printf("debug: recieved ticket %s from server\n", buffer);
        printf("debug: converting ticket string %s to integer...\n", buffer);
        ticket = atoi(buffer);
        printf("debug: converted ticket string %s to integer %d\n", buffer, ticket);
      }
    }
    else if (strcmp(buffer, "cancel") == 0)
    {
      // send "cancel" to server
      n = write(sockfd, "cancel", 6); // send command
      

      // this part blocks the io
      memset((void *) buffer, '\0', 256);
      n = read(sockfd, buffer, sizeof(buffer)); // expect "waiting"
      

      // send ticket number to server
      memset((void *) buffer, '\0', 256);
      sprintf(buffer, "%d", ticket);
      n = write(sockfd, buffer, sizeof(buffer));
      memset((void *) buffer, '\0', 256);

      // read error statement from server
      n = read(sockfd, buffer, sizeof(buffer));
      printf("debug: buffer = %s\n", buffer);
      // handle "error" or "no error"
      if (strcmp(buffer, "error") == 0)
      {
        memset((void *) buffer, '\0', 256);
        n = read(sockfd, buffer, sizeof(buffer));
        printf("debug: buffer = %s\n", buffer);
        printf("(Server) %s\n", buffer); 
      }
      else if (strcmp(buffer, "no error") == 0)
      {
        memset((void *) buffer, '\0', 256);
        n = read(sockfd, buffer, sizeof(buffer));
        printf("debug: buffer = %s\n", buffer);
        printf("(Server) %s\n", buffer);
        ticket = -1;
      }
    }
    else if (strcmp(buffer, "end") == 0)
    {
      n = write(sockfd, "end", 3);
      memset((void *) buffer, '\0', 256);
      printf("debug: closing the socket...\n");
      close(sockfd);
      return 0;
    }
    else
    {
      n = write(sockfd, "error", strlen("error"));
      memset((void *) buffer, '\0', 256);
    } 
    printf("debug: closing the socket...\n");
    close(sockfd);
  }


  return 0;
}

