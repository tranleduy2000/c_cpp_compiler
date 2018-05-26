// C Program to Find the Length of a String

// C programming source code to calculate the length of a string without using strlen() function....

#include <stdio.h>
int main()
{
    char s[1000], i;

    printf("Enter a string: ");
    scanf("%s", s);

    for(i = 0; s[i] != '\0'; ++i);

    printf("Length of string: %d", i);
    return 0;
}

// https://www.programiz.com/c-programming/examples/string-length