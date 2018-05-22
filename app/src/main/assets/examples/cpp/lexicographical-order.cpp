// C++ Program to Sort Elements in Lexicographical Order (Dictionary Order)

// This program sorts the 10 strings (entered by the user) in lexicographical order (dictionary order).

#include <iostream>

using namespace std;

int main() {
    string str[10], temp;

    cout << "Enter 10 words: " << endl;
    for (int i = 0; i < 10; ++i) {
        getline(cin, str[i]);
    }

    for (int i = 0; i < 9; ++i)
        for (int j = i + 1; j < 10; ++j) {
            if (str[i] > str[j]) {
                temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }

    cout << "In lexicographical order: " << endl;

    for (int i = 0; i < 10; ++i) {
        cout << str[i] << endl;
    }
    return 0;
}