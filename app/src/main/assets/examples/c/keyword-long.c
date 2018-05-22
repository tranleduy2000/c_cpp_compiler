// C Program to Demonstrate the Working of Keyword long

// This page contains the tutorial on how long keyword is used in C programming with source code and example...

#include <stdio.h>

int main() {
    int a;
    long b;
    long long c;

    double e;
    long double f;


    printf("Size of int = %ld bytes \n", sizeof(a));
    printf("Size of long = %ld bytes\n", sizeof(b));
    printf("Size of long long = %ld bytes\n", sizeof(c));

    printf("Size of double = %ld bytes\n", sizeof(e));
    printf("Size of long double = %ld bytes\n", sizeof(f));

    return 0;
}