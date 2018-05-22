// C Program to Calculate the Power of a Number

// C program to calculate the power of an integer with source code, example and explanation....

#include <stdio.h>

int main() {
    int base, exponent;

    long long result = 1;

    printf("Enter a base number: ");
    scanf("%d", &base);

    printf("Enter an exponent: ");
    scanf("%d", &exponent);

    while (exponent != 0) {
        result *= base;
        --exponent;
    }

    printf("Answer = %lld", result);

    return 0;
}