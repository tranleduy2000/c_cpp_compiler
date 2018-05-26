// C++ Program to Check Armstrong Number

// Example to check whether an integer (entered by the user) is an Armstrong number or not using while loop and if...else statement.

#include <iostream>
using namespace std;

int main()
{
  int origNum, num, rem, sum = 0;
  cout << "Enter a positive  integer: ";
  cin >> origNum;

  num = origNum;

  while(num != 0)
  {
      digit = num % 10;
      sum += digit * digit * digit;
      num /= 10;
  }

  if(sum == origNum)
    cout << origNum << " is an Armstrong number.";
  else
    cout << origNum << " is not an Armstrong number.";

  return 0;
}

// https://www.programiz.com/cpp-programming/examples/check-armstrong-number