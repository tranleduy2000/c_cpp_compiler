// C Program to Access Elements of an Array Using Pointer

// This program declares the array of five element and the elements of that array are accessed using pointer.

#include <stdio.h>

int main() {
    int data[5], i;
    printf("Enter elements: ");

    for (i = 0; i < 5; ++i)
        scanf("%d", data + i);

    printf("You entered: \n");
    for (i = 0; i < 5; ++i)
        printf("%d\n", *(data + i));

    return 0;
}