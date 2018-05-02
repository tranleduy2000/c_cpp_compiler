// C Program to Display its own Source Code as Output

// C program to display its own source code as it output with explanation....

#include <stdio.h>

int main() {
    FILE *fp;
    char c;
    fp = fopen(__FILE__, "r");
    do {
        c = getc(fp);
        putchar(c);
    } while (c != EOF);
    fclose(fp);
    return 0;
}