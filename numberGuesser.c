// Daniel Durazo
// Matthew Ostovarpour

#include "header.h"

#define MAX 1024 // Highest number to "guess". Must be a power of 2!

/*
 * This function was originally called: "handle_client" in the server code.
 * Now the server accepts a client, calls number_guesser, waits for new client.
 *
 * We use a buffer to read 100 bytes at a time. This allows us to handle the case
 * where the user enters an entire word rather than just the y/n/q character.
 *
 * It also "clears" out the input stream so that it is back to empty, meaning the
 * next user input is assured to be read (rather than the remaining bytes from a
 * previous input).
 *
 * Sending numbers back to the client also is handled through a buffer. As the
 * client is assumed to be telnet, data sent back to the client would be assumed
 * to be text. Sending an integer to the client would display incorrectly. To
 * solve this, we first copy the integer to a string using sprintf and the unsigned
 * integer format tag "%u". This allows changing of MAX and proper display of the
 * 1 to MAX for the user to choose.
 *
 * MAX needs to be a power of 2 in order to function correctly. If not a power of
 * 2, the integer division on odd numbers results in the fence/increment to become
 * incorrect...
 */
void number_guesser(int client_socket) {

    // Buffer to store client's input: attempt to read entire input line
    // in case user enters "yes" "no" or "quit" instead of y/n/q
    char input[100];

    // Temp buffer to hold text representation of a number before sending
    char num_str[10];

    // Initial conditions
    int fence = MAX/2;
    int increment = MAX/2;
    int number = 0;

    // Clear buffer to use
    memset(num_str, 0, sizeof(num_str));

    // Use sprintf to convert MAX number to string
    sprintf(num_str, "%u", (unsigned int)MAX);

    // Inform user what is the expected input
    write(client_socket, "\nEnter y for yes, n for no, or q for quit.\n\n", sizeof(char)*44);

    // Inform user what to do (initial number choice) between 1 and MAX
    write(client_socket, "Pick a number between 1 and ", sizeof(char)*28);
    write(client_socket, num_str, sizeof(num_str)); // Buffer currently holds MAX as string
    write(client_socket, ".\n\n", sizeof(char)*3);

    // Loop until the number has been "guessed"
    // Increment variable will become zero when the number has been determined
    while (increment) {

        // Initialize buffers to use
        memset(num_str, 0, sizeof(num_str));
        memset(input, 0, sizeof(input));

        // Using sprintf to convert a number to text and store in a char buffer
        sprintf(num_str, "%u", fence);

        // Ask user if their number is higher/lower than our "guess" (fence)
        write(client_socket, "Is your number greater than ", sizeof(char)*28);
        write(client_socket, num_str, sizeof(num_str));
        write(client_socket, "?\n => ", sizeof(char)*6);

        // Read input from socket. 100 bytes should be enough to grab all input
        // (including any potential extra characters sent by telnet)
        if (read(client_socket, &input, sizeof(input)) < 0) {

            // Something went wrong, try to close socket and exit with error.
            perror("Error Reading from network!\n");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        // Case: user inputs quit command, so return to the server code (next client)
        if (input[0] == 'q') {
            close(client_socket);
            return;
        }

        // Case: user input "y" or "yes", their number is larger than the current fence
        if (input[0] == 'y') {

            // So increase the current number guess by increment
            number += increment;

            // Divide increment in half to narrow in on the final number
            increment /= 2;

            // Move the fence to the new midpoint
            fence += increment;

        }

        // Case: assume user input "no", their number is smaller than current fence
        else {

            // Divide increment in half to narrow in on the final number
            increment /= 2;

            // Fence was too high, move it back to midpoint of previous guess
            fence -= increment;

        }

        // Move to next loop iteration: ask user and make next guess
    }

    // Last iteration ends up with number being short by one
    // due to initial restriction that the number must be at least one.
    // Worst case: number is one. Keep moving fence to the left,
    // number variable is never increased, number = 0, so need to increment by one.
    number++;

    // Clear out buffer one last time
    memset(num_str, 0, sizeof(num_str));

    // Use sprintf again to convert number to text and store in a char buffer
    sprintf(num_str, "%u", number);

    // Send the final guessed number back to client
    write(client_socket, "Your number is: ", sizeof(char)*16);
    write(client_socket, &num_str, sizeof(num_str));
    write(client_socket, "\n\n", sizeof(char)*2);

    // Cleanup: close socket
    if (close(client_socket) == -1) {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }
}
