// C Program to Print an Integer (Entered by the User)

// Learn about how to print a integer entered by user in C programming with source code, output and explanation.

#include <stdio.h>

int main() {
    int number;

    // printf() dislpays the formatted output 
    printf("Enter an integer: ");

    // scanf() reads the formatted input and stores them
    scanf("%d", &number);

    // printf() displays the formatted output
    printf("You entered: %d", number);
    return 0;
}