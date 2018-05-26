// C++ Program to Find Quotient and Remainder

// In this example, you will learn to find the quotient and remainder of a given dividend and divisor.

#include <iostream>
using namespace std;

int main()
{    
    int divisor, dividend, quotient, remainder;

    cout << "Enter dividend: ";
    cin >> dividend;

    cout << "Enter divisor: ";
    cin >> divisor;

    quotient = dividend / divisor;
    remainder = dividend % divisor;

    cout << "Quotient = " << quotient << endl;
    cout << "Remainder = " << remainder;

    return 0;
}

// https://www.programiz.com/cpp-programming/examples/quotient-remainder