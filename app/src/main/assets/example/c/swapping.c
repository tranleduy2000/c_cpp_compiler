#include <stdio.h>
int main()
{
      double firstNumber, secondNumber, temporaryVariable;

      printf("Enter first number: ");
      scanf("%lf", &firstNumber);

      printf("Enter second number: ");
      scanf("%lf",&secondNumber);

      // Value of firstNumber is assigned to temporaryVariable
      temporaryVariable = firstNumber;

      // Value of secondNumber is assigned to firstNumber
      firstNumber = secondNumber;

      // Value of temporaryVariable (which contains the initial value of firstNumber) is assigned to secondNumber
      secondNumber = temporaryVariable;

      printf("\nAfter swapping, firstNumber = %.2lf\n", firstNumber);
      printf("After swapping, secondNumber = %.2lf", secondNumber);

      return 0;
}