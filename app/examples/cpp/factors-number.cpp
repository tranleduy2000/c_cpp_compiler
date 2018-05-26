// C++ Program to Display Factors of a Number

// Example to find all factors of an integer (entered by the user) using for loop and if statement.

#include <iostream>
using namespace std;

int main()
{
    int n, i;

    cout << "Enter a positive integer: ";
    cin >> n;

    cout << "Factors of " << n << " are: " << endl;  
    for(i = 1; i <= n; ++i)
    {
        if(n % i == 0)
            cout << i << endl;
    }

    return 0;
}

// https://www.programiz.com/cpp-programming/examples/factors-number