// C++ Program to Generate Multiplication Table

// Example to generate the multiplication table of a number (entered by the user) using for loop.

#include <iostream>
using namespace std;

int main()
{
    int n;

    cout << "Enter a positive integer: ";
    cin >> n;

    for (int i = 1; i <= 10; ++i) {
        cout << n << " * " << i << " = " << n * i << endl;
    }
    
    return 0;
}

// https://www.programiz.com/cpp-programming/examples/multiplication-table