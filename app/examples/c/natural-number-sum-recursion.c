// C Program to Find the Sum of Natural Numbers using Recursion

// Example to find the sum of natural numbers by using recursion.

#include <stdio.h>
int addNumbers(int n);

int main()
{
    int num;
    printf("Enter a positive integer: ");
    scanf("%d", &num);
    printf("Sum = %d",addNumbers(num));
    return 0;
}

int addNumbers(int n)
{
    if(n != 0)
        return n + addNumbers(n-1);
    else
        return n;
}

// https://www.programiz.com/c-programming/examples/natural-number-sum-recursion