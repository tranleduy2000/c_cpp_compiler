// C++ Program to Find G.C.D Using Recursion

// Example to find the GCD of two positive integers (entered by the user) using recursion in C programming.

#include <iostream>
using namespace std;

int hcf(int n1, int n2);

int main()
{
   int n1, n2;

   cout << "Enter two positive integers: ";
   cin >> n1 >> n2;

   cout << "H.C.F of " << n1 << " & " <<  n2 << " is: " << hcf(n1, n2);

   return 0;
}

int hcf(int n1, int n2)
{
    if (n2 != 0)
       return hcf(n2, n1 % n2);
    else 
       return n1;
}

// https://www.programiz.com/cpp-programming/examples/hcf-recursion