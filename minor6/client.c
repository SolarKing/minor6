#include "client.h"

int main(int argc, char const *argv[])
{
  
  
  int j;
  int k = 0;
  int parseCondition;

  int n;
  int sockfd;

  int ticket = -1;

  socklen_t serverLength;

  struct sockaddr_un serverAddress;
  
  char buffer[BUFFER_SIZE];

  if (argc < 2)
  {
    fprintf(stderr, "Usage %s path\n", argv[0]);
    exit(0);
  }

  int isConnected = 1;
  while(isConnected == 1)
  {

    memset((void *) &serverAddress, '\0', sizeof(serverAddress));
    serverAddress.sun_family = AF_UNIX;
    strcpy(serverAddress.sun_path, argv[1]);
    serverLength = strlen(serverAddress.sun_path) + sizeof(serverAddress.sun_family);

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sockfd < 0) { error("ERROR on opening socket"); }


    if (connect(sockfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
    {
      error("ERROR on connecting");
    } 

    // printf("Please enter a command: ");
    // memset((void *) buffer, '\0', 256);
    // scanf("%s", buffer);

    // printf("debug: wrote into the buffer: \"%s\"\n", buffer);

    // start conditionals
    
    memset((void *) buffer, '\0', 256);
    switch (k)
    {
      case 1: srand(time(NULL) + k); break;
      case 2: srand(time(NULL) - k); break;
      case 3: srand(time(NULL) + k); break;
      case 4: srand(time(NULL) - k); break;
      case 5: srand(time(NULL) + k); break;
      case 6: srand(time(NULL) - k); break;
      default: srand(time(NULL)); break;
    }
    if ((rand()%10) < 8) { sprintf(buffer, "%s", "buy"); }
    else { sprintf(buffer, "%s", "cancel"); }

    if (k == 7) { sprintf(buffer, "%s", "end"); }
    k++;

    if (strcmp(buffer, "buy") == 0)
    {
      printf("\nBuying a ticket...\n");
      n = write(sockfd, "buy", 3);
      memset((void *) buffer, '\0', 256);

      // read for error
      n = read(sockfd, buffer, sizeof(buffer));
      // printf("debug: buffer = %s\n", buffer);

      if (strncmp(buffer, "error", 5) == 0)
      {
        j = 0;
        parseCondition = -1;
        while(parseCondition < 0)
        {
          if (buffer[j+5] == '\0') { parseCondition = 1; }
          buffer[j] = buffer[j+5];
          j++;
        }
        printf("(Server) Error: %s\n", buffer);
      }
      else if (strncmp(buffer, "noerr", 5) == 0)
      {
        j = 0;
        parseCondition = -1;
        while(parseCondition < 0)
        {
          if (buffer[j+5] == '\0') { parseCondition = 1; }
          buffer[j] = buffer[j+5];
          j++;
        }
        ticket = atoi(buffer);
        printf("(Server) You bought ticket #%d\n", ticket);
        // printf("debug: converted ticket string %s to integer %d\n", buffer, ticket);
      }
    }
    else if (strcmp(buffer, "cancel") == 0)
    {
      
      // send "cancel" to server
      memset((void *) buffer, '\0', 256);
      sprintf(buffer, "%s%d", "cancel", ticket);
      n = write(sockfd, buffer, sizeof(buffer));
      memset((void *) buffer, '\0', 256);

      // read error statement from server
      n = read(sockfd, buffer, sizeof(buffer));
      // printf("debug: buffer = %s\n", buffer);

      // handle "error" or "noerr"
      if (strncmp(buffer, "error", 5) == 0)
      {
        j = 0;
        parseCondition = -1;
        while(parseCondition < 0)
        {
          if (buffer[j+5] == '\0') { parseCondition = 1; }
          buffer[j] = buffer[j+5];
          j++;
        }
        printf("\nReturning ticket...\n");
        printf("(Server) %s\n", buffer); 
      }
      else if (strncmp(buffer, "noerr", 5) == 0)
      {
        j = 0;
        parseCondition = -1;
        while(parseCondition < 0)
        {
          if (buffer[j+5] == '\0') { parseCondition = 1; }
          buffer[j] = buffer[j+5];
          j++; 
        }
        printf("\nReturning ticket #%d...\n", ticket);
        printf("(Server) %s\n", buffer);
        ticket = -1;
      }
    }
    else if (strcmp(buffer, "end") == 0)
    {
      n = write(sockfd, "end", 3);
      memset((void *) buffer, '\0', 256);
      // printf("debug: 2 closing the socket...\n");
      close(sockfd);
      printf("Ending connection.\n\n");
      return 0;
    }
    else
    {
      n = write(sockfd, "end", 3);
      memset((void *) buffer, '\0', 256);
      close(sockfd);
      error("Forcing close connection");
    } 
    // printf("debug: 1 closing the socket...\n");
    close(sockfd);
  }

  return 0;
}

