// C Program Swap Numbers in Cyclic Order Using Call by Reference

// This program takes three three integer value from user and swap the numbers in cyclic order using call by reference technique....

#include<stdio.h>
void cyclicSwap(int *a,int *b,int *c);

int main()
{
    int a, b, c;

    printf("Enter a, b and c respectively: ");
    scanf("%d %d %d",&a,&b,&c);

    printf("Value before swapping:\n");
    printf("a = %d \nb = %d \nc = %d\n",a,b,c);

    cyclicSwap(&a, &b, &c);

    printf("Value after swapping:\n");
    printf("a = %d \nb = %d \nc = %d",a, b, c);

    return 0;
}
void cyclicSwap(int *a,int *b,int *c)
{

    int temp;

    // swapping in cyclic order
    temp = *b;
    *b = *a;
    *a = *c;
    *c = temp;
}

// https://www.programiz.com/c-programming/examples/swapping-cyclic-order