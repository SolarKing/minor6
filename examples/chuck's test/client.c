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
  int sockfd;
  socklen_t servlen;
  int n;

  struct sockaddr_un serv_addr;

  char buffer[82];

  memset((void *) &serv_addr, '\0', sizeof(serv_addr));

  serv_addr.sun_family = AF_UNIX;

  strcpy(serv_addr.sun_path, argv[1]);

  servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

  sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

  if (sockfd < 0)
  {
    error("ERROR on creating socket...");
  }

  if (connect(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
  {
    error("ERROR on connecting...");
  }

  printf("Please enter your message: ");

  memset(buffer, '\0', 82);

  fgets(buffer, 80, stdin);

  n = write(sockfd, buffer, strlen(buffer));

  if (n < 0)
  {
    error("ERROR on writing to socket...");
  }

  memset(buffer, '\0', 82);

  n = read(sockfd, buffer, 80);

  if (n < 0)
  {
    error("ERROR on reading from socket...");
  }

  printf("The return message was\n");

  write(1, buffer, n);

  close(sockfd);
  
  return 0;
}