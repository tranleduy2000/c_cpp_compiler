// C++ Program to Add Two Numbers

// Source code to add two numbers using three variables and two variables in C++ programming..

#include <iostream>

using namespace std;

int main() {
    int firstNumber, secondNumber, sumOfTwoNumbers;

    cout << "Enter two integers: ";
    cin >> firstNumber >> secondNumber;

    // sum of two numbers in stored in variable sumOfTwoNumbers
    sumOfTwoNumbers = firstNumber + secondNumber;

    // Prints sum 
    cout << firstNumber << " + " << secondNumber << " = " << sumOfTwoNumbers;

    return 0;
}