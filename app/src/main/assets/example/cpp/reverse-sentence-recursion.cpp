// C++ program to Reverse a Sentence Using Recursion

// This program takes a sentence from user and reverses that sentence using recursion. This program does not use string to reverse the sentence or store the sentence.

#include <iostream>

using namespace std;

void reverse(const string &a);

int main() {
    string str;
    cout << " Please enter a string " << endl;
    getline(cin, str);
    reverse(str);
    return 0;
}

void reverse(const string &str) {
    size_t numOfChars = str.size();

    if (numOfChars == 1)
        cout << str << endl;
    else {
        cout << str[numOfChars - 1];
        reverse(str.substr(0, numOfChars - 1));
    }
}