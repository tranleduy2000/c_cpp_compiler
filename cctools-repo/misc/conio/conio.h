// Copyright (C) 2005  by Piotr He³ka (piotr.helka@nd.e-wro.pl)
// Linux C++ (not full) implementation of Borland's conio.h 
// v 1.01
// It uses Ncurses lib, so accept also its terms.



// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

// ----------------------------- krotki opis ------------------

// Biblioteka obs³uguje mniej lub bardziej zgodnie:
//
// cgets()
// cputs()
// clreol()
// clrscr()
// cprintf()
// cscanf()
// getch() (chyba nie wszystkie kody tak jak w conio.h)
// getche()  
// gotoxy()
// kbhit()
// putch()
// textbackground()
// textcolor()
// wherex()
// wherey()
// window()
//
// kompatbyilno¶æ w kierunku Linux CONIO.H -> DOS CONIO.H
// bedzie zachowana

// Aby skompilowac
// $g++ nazwa_progsa.cpp -lncurses -o nazwa_progsa.o

// ------------------------------- define ---------------------

#ifndef __CONIO_H
#define __CONIO_H

#include <stdlib.h>
#include <ncurses.h>

#define MAX_OKIEN 256

#define BLACK       0
#define RED         1
#define GREEN       2
#define BROWN       3
#define BLUE        4
#define MAGENTA     5
#define CYAN        6
#define LIGHTGRAY   7
#define DARKGRAY    0
#define LIGHTRED    1
#define LIGHTGREEN  2
#define YELLOW      3
#define LIGHTBLUE   4
#define PINK        5
#define LIGHTCYAN   6
#define WHITE       7

// -------------------------------- globalne ------------------

//int (* wsk_f)(void) = getch;

#undef getch
#define getch CURSgetch

#undef getche
#define getche CURSgetche

#ifdef __cplusplus
extern "C" {
#endif
static void inicjuj();
static void finalizuj();

typedef struct
{
	int 	xup;
	int 	yup;
	int 	xdown;
	int 	ydown;
	WINDOW*	okno;
} Okno;

static bool	zainicjowane = FALSE; //czy juz po initscr() ?
static int	znakSpecjalny = -1; //potrzebne do getch'a
static int	n = 0; //liczba uzytych okienek

static short	kolorTekstu = COLOR_WHITE;
static short	kolorTla = COLOR_BLACK;
static short	biezacaPara;

static Okno	okienka[MAX_OKIEN];	//tablica struktur aktywnych okienek
static WINDOW*	aktywneOkno = NULL;	//wsk na aktywne okno
    
    

// ----------------------------- koniec globalnych ------------

static void inicjuj()
{
	initscr();
	start_color(); //wlaczmy kolorki
	cbreak(); //wylaczmy buforowanie wejscia
	noecho(); //bez wyswietlania na ekran
	//raw(); //nadpisywane i tak przez noecho
	keypad(stdscr, TRUE);
	scrollok(stdscr, TRUE);
	
	//domyslne okno
	aktywneOkno = stdscr;
	zainicjowane = TRUE;
	
	//utworzmy macierz 8x8 kolorow tla i tekstu
	short kolor = 1;
	short i;
	for(i=0; i<8; i++)
	{
		short j;
		for(j=0; j<8; j++, kolor++)
		{
			init_pair(kolor,i,j);
			if(i == COLOR_WHITE && j == COLOR_BLACK)	
			//ustawmy czarne tlo i bialey tekst jako standard
			{
				biezacaPara = kolor;
			}  
		}
	}
	
	wrefresh(aktywneOkno);
	atexit(finalizuj);
}

static void finalizuj()
{
	endwin();
}

static int simple_strlen(const char* str)
{
//	char* p;
//	for(p = str; *p != 0; p++);
//	return p-str;
	int len = 0;
	while (str[len]) len++;
	return len;
}

static void cputs(const char* str)
{
	waddstr(aktywneOkno, str);
	wrefresh(aktywneOkno);
}

static char* cgets(char* str)
{ // nie wiem dokladnie jak dziala orginalna f. cgets bo nie mam
  // do niej referencji..
	if(str == NULL || *str == 0)
	{
		*(str+1) = 0;
		return NULL;
	}
	
	int max = (int)(*str);
	
	echo();
	
	if(wgetnstr(aktywneOkno, (str + 2), max) == ERR)
	{
		*(str+1) = 0;
		return NULL;
	}
	
	noecho();
	
	*(str+1) = (char)simple_strlen(str+2);
	
	return str+2;
}

static void clreol()
{
	wclrtoeol(aktywneOkno);
	wrefresh(aktywneOkno);
}

static void clrscr()
{
	if(!zainicjowane) inicjuj();
	wbkgd(aktywneOkno, COLOR_PAIR(biezacaPara));
	//trzeba przesunac kursor? chyba nie...
	wclear(aktywneOkno);
}

static int cprintf(const char *fmt, ...)
// czysty hardcore ;-)
{
	if(!zainicjowane) inicjuj();
	
	va_list ap; 
	va_start(ap, fmt);
	
        int i = vwprintw(aktywneOkno,fmt, ap);	//jakie proste ;-)
	
	va_end(ap);
	
	wrefresh(aktywneOkno);
	
	return i;
}

static int cscanf(char *fmt, ...)
{
	if(!zainicjowane) inicjuj();
	
	echo();
	
	va_list ap;
	va_start(ap, fmt);
	
	int i = vwscanw(aktywneOkno, fmt, ap);
	
	va_end(ap);
	
	wrefresh(aktywneOkno);
	noecho();
	
	return i;
}

static int CURSgetch()
{
	if(!zainicjowane) inicjuj();
	
	int znak;
	
	if(znakSpecjalny>0) //drugi czlon znaku specjalnego 0x00 i 0x??
	{
		//zamieniamy znak na kod DOSowy - conio.h
		znak = znakSpecjalny;
		znakSpecjalny = -1;
			
		return znak-265+59;
	}

	znak = wgetch(aktywneOkno);
	
	if(znak > 255) //to mamy znak specjalny 0x00
	{
		znakSpecjalny = znak;
		return 0;
	}

	return znak;
}

static int CURSgetche()
{
	echo();
	int znak = getch();
	noecho();
	return znak;
}

static int gotoxy(int x, int y)
{
	if(!zainicjowane) inicjuj();
	wmove(aktywneOkno, y - 1, x - 1);
	return 0;
}

static int kbhit()
{
	int znak;
	wtimeout(aktywneOkno, 0);
	znak = wgetch(aktywneOkno);
	//wtimeout(aktywneOkno, -1);
	nodelay(aktywneOkno, FALSE);
	if (znak == ERR) return 0;
	ungetch(znak);
	return 1;
}

static int putch(int znak)
{
	wechochar(aktywneOkno,znak);
}

static void textbackground(short kolor)
{
	if(!zainicjowane) inicjuj();
	kolorTla = kolor%8;
	short k=1;
	short i;
	for(i=0; i<8; i++) //wyszukajmy numer pary dla kolorow
	{
		short j;
		for(j=0; j<8; j++, k++)
		{
			if(kolorTekstu == i && kolorTla == j)
			{
				biezacaPara = k;
				wbkgd(aktywneOkno, COLOR_PAIR(k));
			}
		}
	}
	
	wrefresh(aktywneOkno);
}

static void textcolor(short kolor)
{
	if(!zainicjowane) inicjuj();
	kolorTekstu = kolor%8;
	
	short k=1;
	short i;
	for(i=0; i<8; i++) //wyszukajmy numer pary dla kolorow
	{
		short j;
		for(j=0; j<8; j++, k++)
		{
			if(kolorTekstu == i && kolorTla == j)
			{
				biezacaPara = k;
				wcolor_set(aktywneOkno,k, NULL);
			}
		}
	}
	
	wrefresh(aktywneOkno);
}

static int wherex(void)
{
	if(!zainicjowane) inicjuj();
	int x, y;
	getyx(aktywneOkno, y, x);
	return x + 1;
}

static int wherey(void)
{
	if(!zainicjowane) inicjuj();
	int x, y;
	getyx(aktywneOkno, y, x);
	return y + 1;
}

static void window(int xup, int yup, int xdown, int ydown)
{
	if( xup<1 || yup<1 || xdown>COLS || ydown>LINES)
	{ //jesli zle dane podano...
		xdown = COLS - xup;
		ydown = LINES - yup;
		//return;
	}

	bool istnieje = FALSE;

	if(!zainicjowane) inicjuj();
	
	/* 
	Istnieje alternatywne rozwiazanie tworzenia nowych okien,
	w momencie tworzenia nowego okna, usuwa sie okno poprzednie,
	tzn zwalnia pamiec tego okna, komenda delwin(nzw_okna) i tworzy
	sie nowe okno, ustawiajac jego jako domyslne-biezace. Jednak
	poniewaz moze to zabierac za duzo czasu i niepotrzebnie spowolniac,
	majac na uwadze rozmiar dzisiejszych pamieci, postanowilem, uzyc
	tablicy, ktora przechowywuje wsk. na adresy okien i wykorzystuje
	zaalokowana juz przestrzen. Aczkolwiek mozna to w kazdej chwili zmienic. 
	*/
	int i;
	for(i=0; i<n && !istnieje; i++) //sprawdzimy czy podane okno juz nie
	// zostalo wczesniej stworzone
	{
		if( okienka[i].xup == xup && okienka[i].yup == yup
		&& okienka[i].xdown == xdown && okienka[i].ydown == ydown)
		{
			aktywneOkno = okienka[i].okno;
			istnieje = TRUE;
			clrscr();
		}
	}
	
	if(!istnieje && n < MAX_OKIEN) //jesli nie ma takiego okna to tworzymy je
	{
		aktywneOkno = newwin(ydown - yup + 1, xdown - xup + 1, yup - 1, xup - 1);
		//nie dam glowy czy dokladnie tak wyswietla conio.h
		
		//do tablicy zapisac...
		okienka[n].okno = aktywneOkno;
		okienka[n].xup = xup;
		okienka[n].yup = yup;
		okienka[n].xdown = xdown;
		okienka[n].ydown = ydown;
		
		wcolor_set(aktywneOkno,biezacaPara, NULL);
		wbkgd(aktywneOkno, COLOR_PAIR(biezacaPara));
		
		//przywrocenie ustawien klawiszy
		cbreak(); //wylaczmy buforowanie wejscia
		noecho(); //bez wyswietlania na ekran
		keypad(aktywneOkno, TRUE); //pelne kody klawiszy
		scrollok(aktywneOkno, TRUE);
		
		n++;
	}
	
	wrefresh(aktywneOkno);

	return;
}

#ifdef __cplusplus
}

class Startuj   // konstruktor i destruktor klasy beda odpowiedzalni
{	public:     // za automagiczna inicjalizacje ustawien ;-)
	Startuj(){ inicjuj(); }
	~Startuj(){ finalizuj(); }
} Start;	    			// inicjuj!
#endif

#endif
