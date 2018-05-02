// C++ Program to Concatenate Two Strings

// In this example, you will learn to concatenate (join) two strings (both string objects and C-style strings).

#include <iostream>

using namespace std;

int main() {
    string s1, s2, result;

    cout << "Enter string s1: ";
    getline(cin, s1);

    cout << "Enter string s2: ";
    getline(cin, s2);

    result = s1 + s2;

    cout << "Resultant String = " << result;

    return 0;
}