#include <stdio.h>
#include <conio.h>

int main(void)
{
  int c;

  clrscr();

  for (c = BLACK; c <= WHITE; c++) {
    textcolor(c);
    cprintf("This is a test\n");
  }

  cprintf("\nPress any key.\n");
  getch();

  return 0;
}
