// C++ Program to Check Whether a Number can be Express as Sum of Two Prime Numbers

// Example to check if an integer (entered by the user) can be expressed as the sum of two prime numbers of all possible combinations with the use of functions.

#include <iostream>

using namespace std;

bool checkPrime(int n);

int main() {
    int n, i;
    bool flag = false;

    cout << "Enter a positive  integer: ";
    cin >> n;

    for (i = 2; i <= n / 2; ++i) {
        if (checkPrime(i)) {
            if (checkPrime(n - i)) {
                cout << n << " = " << i << " + " << n - i << endl;
                flag = true;
            }
        }
    }

    if (!flag)
        cout << n << " can't be expressed as sum of two prime numbers.";

    return 0;
}

// Check prime number
bool checkPrime(int n) {
    int i;
    bool isPrime = true;

    for (i = 2; i <= n / 2; ++i) {
        if (n % i == 0) {
            isPrime = false;
            break;
        }
    }

    return isPrime;
}