// C Program to Display Fibonacci Sequence

// Example on how to display the Fibonacci sequence of first n numbers (entered by the user) using loop. Also in different example, you learn to generate the Fibonacci sequence up to a certain number.

#include <stdio.h>

int main() {
    int i, n, t1 = 0, t2 = 1, nextTerm;

    printf("Enter the number of terms: ");
    scanf("%d", &n);

    printf("Fibonacci Series: ");

    for (i = 1; i <= n; ++i) {
        printf("%d, ", t1);
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
    }
    return 0;
}