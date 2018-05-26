// C++ Program to Check Whether Number is Even or Odd

// In this example, if...else statement is used to check whether a number entered by the user is even or odd.

#include <iostream>
using namespace std;

int main()
{
    int n;

    cout << "Enter an integer: ";
    cin >> n;

    if ( n % 2 == 0)
        cout << n << " is even.";
    else
        cout << n << " is odd.";

    return 0;
}

// https://www.programiz.com/cpp-programming/examples/even-odd