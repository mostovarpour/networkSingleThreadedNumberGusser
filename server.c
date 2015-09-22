#include "server.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX 1024

void guessing_numbers();

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
        guessing_numbers();
    }
}


/*
 * handle client
 */
void handle_client(int client_socket) {
    char input;
    int keep_going = TRUE;
    
    while (keep_going) {
        // read char from client
        switch (read(client_socket, &input, sizeof(char))) {
            case 0:
                keep_going = FALSE;
                printf("\nEnd of stream, returning ...\n");
                break;
            case -1:
                perror("Error reading from network!\n");
                keep_going = FALSE;
                break;
        }
        printf("%c", input);
        
        if (input == 'q') {
            write(client_socket, &input, sizeof(char));
            break;
        }
        
        // send result back to client
        write(client_socket, &input, sizeof(char));
    }
    
    // cleanup
    if (close(client_socket) == -1) {
        perror("Error closing socket");
        exit(EXIT_FAILURE); 
    } else {
        printf("\nClosed socket to client, exit");
    }
}

void guessing_numbers()
{
    char response;
    int fence = MAX/2;
    int increment = MAX/2;
    int number = 0;

    while (increment)
    {
        printf("Is your team number greater than %d", fence);
        response = getchar(); getchar();
        printf("response: %c\n", response);

        if (response == 'y')
        {
            number += increment;
            increment /= 2;
            fence += increment;
            printf("Add increment %d\n", increment);
        }
        else
        {
            increment /= 2;
            fence -= increment;
            printf("Subtract increment: %d\n", increment);
        }
    }

    number++;
    printf("Your number: %i\n", number);
}
