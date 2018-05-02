// C Program to Find Factorial of a Number Using Recursion

// Example to find the factorial of a non-negative integer (entered by the user) using recursion.

#include <stdio.h>

long int multiplyNumbers(int n);

int main() {
    int n;
    printf("Enter a positive integer: ");
    scanf("%d", &n);
    printf("Factorial of %d = %ld", n, multiplyNumbers(n));
    return 0;
}

long int multiplyNumbers(int n) {
    if (n >= 1)
        return n * multiplyNumbers(n - 1);
    else
        return 1;
}