// C++ Program to Multiply two Numbers

// In this program, user is asked to enter two numbers (floating point numbers). Then, the product of those two numbers is stored in a variable and displayed on the screen.

#include <iostream>
using namespace std;

int main()
{
    double firstNumber, secondNumber, productOfTwoNumbers;
    cout << "Enter two numbers: ";

    // Stores two floating point numbers in variable firstNumber and secondNumber respectively
    cin >> firstNumber >> secondNumber;
 
    // Performs multiplication and stores the result in variable productOfTwoNumbers
    productOfTwoNumbers = firstNumber * secondNumber;  

    cout << "Product = " << productOfTwoNumbers;    
    
    return 0;
}

// https://www.programiz.com/cpp-programming/examples/product-numbers