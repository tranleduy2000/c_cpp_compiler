// C Program to Generate Multiplication Table

// Example on how to generate the multiplication table of a number (entered by the user) using for loop.

#include <stdio.h>

int main() {
    int n, i;

    printf("Enter an integer: ");
    scanf("%d", &n);

    for (i = 1; i <= 10; ++i) {
        printf("%d * %d = %d \n", n, i, n * i);
    }

    return 0;
}