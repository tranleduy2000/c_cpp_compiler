// C Program to Compute Quotient and Remainder

// C programming example to compute the remainder and quotient when dividend and divisor are entered by user with output and explanation...

#include <stdio.h>

int main() {

    int dividend, divisor, quotient, remainder;

    printf("Enter dividend: ");
    scanf("%d", &dividend);

    printf("Enter divisor: ");
    scanf("%d", &divisor);

    // Computes quotient
    quotient = dividend / divisor;

    // Computes remainder
    remainder = dividend % divisor;

    printf("Quotient = %d\n", quotient);
    printf("Remainder = %d", remainder);

    return 0;
}