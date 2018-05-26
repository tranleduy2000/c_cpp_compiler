// C++ Program to Find GCD

// Examples on different ways to calculate GCD of two integers (for both positive and negative integers) using loops and decision making statements.

#include <iostream>
using namespace std;

int main()
{
    int n1, n2;

    cout << "Enter two numbers: ";
    cin >> n1 >> n2;
    
    while(n1 != n2)
    {
        if(n1 > n2)
            n1 -= n2;
        else
            n2 -= n1;
    }

    cout << "HCF = " << n1;
    return 0;
}

// https://www.programiz.com/cpp-programming/examples/hcf-gcd