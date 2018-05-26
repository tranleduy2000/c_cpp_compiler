// C++ Program to Reverse a Number

// Source code to reverse an positive integer entered by user in C++ Programming...

#include <iostream>
using namespace std;

int main()
{
    int n, reversedNumber = 0, remainder;

    cout << "Enter an integer: ";
    cin >> n;

    while(n != 0)
    {
        remainder = n%10;
        reversedNumber = reversedNumber*10 + remainder;
        n /= 10;
    }

    cout << "Reversed Number = " << reversedNumber;

    return 0;
}

// https://www.programiz.com/cpp-programming/examples/reverse-number