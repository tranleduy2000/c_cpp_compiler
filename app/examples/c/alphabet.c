// C Program to Check Whether a Character is an Alphabet or not

// In this example, you will learn to check whether a character entered by the user is an alphabet or not.

#include <stdio.h>
int main()
{
    char c;
    printf("Enter a character: ");
    scanf("%c",&c);

    if( (c>='a' && c<='z') || (c>='A' && c<='Z'))
        printf("%c is an alphabet.",c);
    else
        printf("%c is not an alphabet.",c);

    return 0;
}

// https://www.programiz.com/c-programming/examples/alphabet