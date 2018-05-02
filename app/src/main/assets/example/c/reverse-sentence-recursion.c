// C program to Reverse a Sentence Using Recursion

// Source code to reverse the sentence entered by user using recursion in C programming. Strings are not used for solving this program...

/* Example to reverse a sentence entered by user without using strings. */

#include <stdio.h>

void reverseSentence();

int main() {
    printf("Enter a sentence: ");
    reverseSentence();

    return 0;
}

void reverseSentence() {
    char c;
    scanf("%c", &c);

    if (c != '\n') {
        reverseSentence();
        printf("%c", c);
    }
}