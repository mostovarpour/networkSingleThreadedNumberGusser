// Daniel Durazo
// Matthew Ostovarpour

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_ADDR "127.0.0.1" // for client
#define PORT 23657              // port the server will listen on

#define FALSE 0
#define TRUE !FALSE

#define NUM_CONNECTIONS 1       // number of pending connections in the connection queue

/* Function prototypes */
void number_guesser(int);
