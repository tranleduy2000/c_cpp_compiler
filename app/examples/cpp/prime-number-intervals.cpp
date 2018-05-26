// C++ Program to Display Prime Numbers Between Two Intervals

// Example to print all prime numbers between two numbers (entered by the user) in C++ Programming. This problem is solved using nested for loop and if...else statement.

#include <iostream>
using namespace std;

int main()
{
    int low, high, i, flag;

    cout << "Enter two numbers(intervals): ";
    cin >> low >> high;

    cout << "Prime numbers between " << low << " and " << high << " are: ";

    while (low < high)
    {
        flag = 0;

        for(i = 2; i <= low/2; ++i)
        {
            if(low % i == 0)
            {
                flag = 1;
                break;
            }
        }

        if (flag == 0)
            cout << low << " ";

        ++low;
    }

    return 0;
}

// https://www.programiz.com/cpp-programming/examples/prime-number-intervals