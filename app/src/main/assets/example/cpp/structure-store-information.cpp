// C++ Program to Store Information of a Student in a Structure

// This program stores the information (name, roll and marks entered by the user) of a student in a structure and displays it on the screen.

#include <iostream>

using namespace std;

struct student {
    char name[50];
    int roll;
    float marks;
};

int main() {
    student s;
    cout << "Enter information," << endl;
    cout << "Enter name: ";
    cin >> s.name;
    cout << "Enter roll number: ";
    cin >> s.roll;
    cout << "Enter marks: ";
    cin >> s.marks;

    cout << "\nDisplaying Information," << endl;
    cout << "Name: " << s.name << endl;
    cout << "Roll: " << s.roll << endl;
    cout << "Marks: " << s.marks << endl;
    return 0;
}