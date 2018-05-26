// C++ Program to Display Armstrong Number Between Two Intervals

// Example to find all Armstrong numbers between two integers (entered by the user) using loops and if...else statement.

#include <iostream>
using namespace std;

int main()
{
  int num1, num2, i, num, digit, sum;

  cout << "Enter first number: ";
  cin >> num1;

  cout << "Enter second number: ";
  cin >> num2;

  cout << "Armstrong numbers between " << num1 << " and " << num2 << " are: " << endl;
  for(i = num1; i <= num2; i++)
  {
        sum = 0;
        num = i;

        for(; num > 0; num /= 10)
        {
            digit = num % 10;
            sum = sum + digit * digit * digit;
        }

        if(sum == i)
        {
            cout << i << endl;
        }
  }

  return 0;
}

// https://www.programiz.com/cpp-programming/examples/armstrong-number-interval