// C Program to Calculate the Sum of Natural Numbers

// This page contains example to find sum of natural numbers in C programming with output...

#include <stdio.h>
int main()
{
    int n, i, sum = 0;
    
    printf("Enter a positive integer: ");
    scanf("%d",&n);

    for(i=1; i <= n; ++i)
    {
        sum += i;   // sum = sum+i;
    }

    printf("Sum = %d",sum);

    return 0;
}

// https://www.programiz.com/c-programming/examples/sum-natural-numbers