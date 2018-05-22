// C++ Program to Check Whether a Number is Prime or Not

// Example to check whether an integer (entered by the user) is a prime number or not using for loop and if...else statement.

#include <iostream>

using namespace std;

int main() {
    int n, i;
    bool isPrime = true;

    cout << "Enter a positive integer: ";
    cin >> n;

    for (i = 2; i <= n / 2; ++i) {
        if (n % i == 0) {
            isPrime = false;
            break;
        }
    }
    if (isPrime)
        cout << "This is a prime number";
    else
        cout << "This is not a prime number";

    return 0;
}