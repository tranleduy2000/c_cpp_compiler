// C Program to Check Whether a Number can be Expressed as Sum of Two Prime Numbers

// Example to check if an integer (entered by the user) can be expressed as sum of two prime numbers of all possible combinations by making a function.

#include <stdio.h>

int checkPrime(int n);

int main() {
    int n, i, flag = 0;

    printf("Enter a positive integer: ");
    scanf("%d", &n);

    for (i = 2; i <= n / 2; ++i) {
        // condition for i to be a prime number
        if (checkPrime(i) == 1) {
            // condition for n-i to be a prime number
            if (checkPrime(n - i) == 1) {
                // n = primeNumber1 + primeNumber2
                printf("%d = %d + %d\n", n, i, n - i);
                flag = 1;
            }

        }
    }

    if (flag == 0)
        printf("%d cannot be expressed as the sum of two prime numbers.", n);

    return 0;
}

// Function to check prime number
int checkPrime(int n) {
    int i, isPrime = 1;

    for (i = 2; i <= n / 2; ++i) {
        if (n % i == 0) {
            isPrime = 0;
            break;
        }
    }

    return isPrime;
}