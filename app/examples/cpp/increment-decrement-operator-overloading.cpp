// Increment ++ and Decrement -- Operator Overloading in C++ Programming

// In this example, you'll learn to overload increment ++ and decrement -- operators in C++.

#include <iostream>
using namespace std;

class Check
{
    private:
       int i;
    public:
       Check(): i(0) {  }
       void operator ++() 
          { ++i; }
       void Display() 
          { cout << "i=" << i << endl; }
};

int main()
{
    Check obj;

    // Displays the value of data member i for object obj
    obj.Display();

    // Invokes operator function void operator ++( )
    ++obj; 
  
    // Displays the value of data member i for object obj
    obj.Display();

    return 0;
}

// https://www.programiz.com/cpp-programming/increment-decrement-operator-overloading