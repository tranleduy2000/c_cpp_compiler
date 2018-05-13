#include <iostream>
#include <conio.h>

using namespace std; 

int main(void)
{
  int c;

  clrscr();

  for (c = BLACK; c <= WHITE; c++) {
    textcolor(c);
    cout << "This is a test\n\r";
  }

  gotoxy(10, 10);
  cout << "\nPress any key.\n\r";

  getch();

  return 0;
}
