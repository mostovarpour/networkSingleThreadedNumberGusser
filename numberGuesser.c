#define MAX 1024

#include <stdio.h>
#include <stdlib.h>
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
