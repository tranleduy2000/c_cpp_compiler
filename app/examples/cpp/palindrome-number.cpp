// C++ Program to Check Whether a Number is Palindrome or Not

// This program reverses an integer (entered by the user) using while loop. Then, if statement is used to check whether the reversed number is equal to the original number or not.

#include <iostream>
using namespace std;

int main()
{
     int n, num, digit, rev = 0;

     cout << "Enter a positive number: ";
     cin >> num;

     n = num;

     do
     {
         digit = num % 10;
         rev = (rev * 10) + digit;
         num = num / 10;
     } while (num != 0);

     cout << " The reverse of the number is: " << rev << endl;

     if (n == rev)
         cout << " The number is a palindrome";
     else
         cout << " The number is not a palindrome";

    return 0;
}

// https://www.programiz.com/cpp-programming/examples/palindrome-number