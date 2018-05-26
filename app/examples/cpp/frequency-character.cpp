// C++ Program to Find the Frequency of Characters in a String

// In this example, frequency of occurrence of a character is checked for both (String object and C-style string).

#include <iostream>
using namespace std;

int main()
{
    string str = "C++ Programming is awesome";
    char checkCharacter = 'a';
    int count = 0;

    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] ==  checkCharacter)
        {
            ++ count;
        }
    }

    cout << "Number of " << checkCharacter << " = " << count;

    return 0;
}

// https://www.programiz.com/cpp-programming/examples/frequency-character