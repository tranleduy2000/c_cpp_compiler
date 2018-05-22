// C++ Program to Swap Two Numbers

// This example contains two different techniques to swap numbers in C programming. The first program uses temporary variable to swap numbers, whereas the second program doesn't use temporary variables.

#include <iostream>

using namespace std;

int main() {
    int a = 5, b = 10, temp;

    cout << "Before swapping." << endl;
    cout << "a = " << a << ", b = " << b << endl;

    temp = a;
    a = b;
    b = temp;

    cout << "\nAfter swapping." << endl;
    cout << "a = " << a << ", b = " << b << endl;

    return 0;
}