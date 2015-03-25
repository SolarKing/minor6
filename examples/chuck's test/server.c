/** NOTES
 * Creating a socket
 *   int socket(int domain, int type, int protocol)
 *     domains -> AF_INET or AF_UNIX
 *     socket types -> Streams - TCP/IP
 *                  -> Datagrams - UDP/IP
 *     socket address
 *       Struct sockadd_un
 *       {
 *         sa_family_t sun_family; // AF_UNIX
 *         char sun_path[]; // pathname
 *       }
 *     naming a socket
 *       int bind(int socket, const struct sockaddr *address, size_t address_len);
 * Sockets Interface
 *   Creating a socket queue
 *     int listen(int socket, int backlog);
 *   Accepting Connections
 *     int accept(int socket, struct sockaddr *address, size_t *address_len);
 *   Requesting Connections
 *     int connect(int socket, struct sockaddr *address, size_t *address_len);
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

void error(char *msg)
{
  perror(msg);
  exit(1);
}


int main(int argc, char const *argv[])
{

  if (argc < 2)
  {
    error("ERROR on command line argument...");
  }

  int sockfd;
  int newsockfd;
  int servlen;
  int n;
  int numClients;

  socklen_t clilen;

  struct sockaddr_un cli_addr, server_addr;

  char buffer[80];

  printf("Enter the # of clients allowed: ");
  scanf("%d", &numClients);

  sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

  if (sockfd < 0)
  {
    error("ERROR on creating socket...");
  }

  memset((void *) &server_addr, '\0', sizeof(server_addr));

  strcpy(server_addr.sun_path, argv[1]);

  servlen = strlen(server_addr.sun_path) + sizeof(server_addr.sun_family);

  if (bind(sockfd, (struct sockaddr *) &server_addr, servlen) < 0)
  {
    error("ERROR on binding socket...");
  }

  printf("Socket created!!!\nWaiting for a client!\n");

  while(numClients > 0)
  {
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
    {
      error("ERROR on accepting...");
    }
    n = read(newsockfd, buffer, 80);
    // if (n < 0)
    // {
    //   error("ERROR on reading socket...");
    // }
    
    printf("A connection has been established with %d\n", newsockfd);

    write(1, buffer, n);

    write(newsockfd, "I got you message!\n", 20);

    close(newsockfd);

    numClients--;
  }
  close(sockfd);

  return 0;
}