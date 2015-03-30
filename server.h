#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>

#ifndef SERVER_H   /* Include guard */
#define SERVER_H

static const int MAX_TICKETS = 10;
static const int YES = 1;
static const int NO = 0;

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
int pushIntoDatabase(int *db, int dbLength, char *errorMsg);

/**
 * This will send a c-style string to the client.
 * @param  msg C-style string
 * @return     Equivalent to the return value of write()
 */
int sendMessage(char *msg, int socket);


#endif
