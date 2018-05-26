// C++ Programs To Create Pyramid and Pattern

// Examples to print half pyramid, pyramid, inverted pyramid, Pascal's Triangle and Floyd's triangle in C++ Programming using control statements.

#include <iostream>
using namespace std;

int main()
{
    int rows;

    cout << "Enter number of rows: ";
    cin >> rows;

    for(int i = 1; i <= rows; ++i)
    {
        for(int j = 1; j <= i; ++j)
        {
            cout << "* ";
        }
        cout << "\n";
    }
    return 0;
}

// https://www.programiz.com/cpp-programming/examples/pyramid-pattern