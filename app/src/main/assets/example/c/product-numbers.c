#include <stdio.h>
int main()
{
    double firstNumber, secondNumber, product;
    printf("Enter two numbers: ");

    // Stores two floating point numbers in variable firstNumber and secondNumber respectively
    scanf("%lf %lf", &firstNumber, &secondNumber);  
 
    // Performs multiplication and stores the result in variable productOfTwoNumbers
    product = firstNumber * secondNumber;  

    // Result up to 2 decimal point is displayed using %.2lf
    printf("Product = %.2lf", product);
    
    return 0;
}