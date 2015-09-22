#include "server.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX 1024

/*
 * main()
 */
int main(int argc, char** argv) {
    int server_socket;                 // descriptor of server socket
    struct sockaddr_in server_address; // for naming the server's listening socket
    int client_socket;

    // create unnamed network socket for server to listen on
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    
    // name the socket (making sure the correct network byte ordering is observed)
    server_address.sin_family      = AF_INET;           // accept IP addresses
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // accept clients on any interface
    server_address.sin_port        = htons(PORT);       // port to listen on
    
    // binding unnamed socket to a particular port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }
    
    // listen for client connections (pending connections get put into a queue)
    if (listen(server_socket, NUM_CONNECTIONS) == -1) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }
    
    // server loop
    while (TRUE) {
        
        // accept connection to client
        if ((client_socket = accept(server_socket, NULL, NULL)) == -1) {
            perror("Error accepting client");
        } else {
            printf("\nAccepted client\n");            
            handle_client(client_socket);
        }
    }
}


/*
 * handle client
 */
void handle_client(int client_socket) {
    //Our code for keeping track the user input and number guessing
    char input;
    int keep_going = TRUE;
    char response;
    int fence = MAX/2;
    int increment = MAX/2;
    int number = 0;
    
    //Prompting the user for input
    write(client_socket, "Think of a number between 1 and 1000.\n", sizeof(char)*39);

    while (keep_going) {
	write(client_socket, "Is your number greater than ", sizeof(char)*28);
	write(client_socket, &fence, sizeof(int));
	write(client_socket, "\n\n", sizeof(char)*4);
	
        // read char from client
        switch (read(client_socket, &input, sizeof(char))) {
            case 0:
                keep_going = FALSE;
                printf("\nEnd of stream, returning ...\n");
                break;
            case -1:
                perror("Error Reading from network!\n");
                keep_going = FALSE;
                break;
        }
        
        if (input == 'q') {
            write(client_socket, &input, sizeof(char));
            break;
        }

	if (input == 'y') {

	} else {

	}
        
        // send result back to client
        //write(client_socket, &input, sizeof(char));
    }
    
    // cleanup
    if (close(client_socket) == -1) {
        perror("Error closing socket");
        exit(EXIT_FAILURE); 
    } else {
        printf("\nClosed socket to client, exit");
    }
}
