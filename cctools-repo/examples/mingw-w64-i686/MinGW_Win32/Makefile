# This Makefile will build the MinGW Win32 application.

HEADERS = include/callbacks.h include/resource.h
OBJS =	obj/winmain.o obj/callbacks.o obj/resource.o
INCLUDE_DIRS = -I./include

WARNS = -Wall

CC = i686-w64-mingw32-gcc
LDFLAGS = -s -lcomctl32 -Wl,--subsystem,windows
RC = i686-w64-mingw32-windres

# Compile ANSI build only if CHARSET=ANSI
ifeq (${CHARSET}, ANSI)
  CFLAGS= -O3 -std=c99 -D _WIN32_IE=0x0500 -D WINVER=0x500 ${WARNS}
else
  CFLAGS= -O3 -std=c99 -D UNICODE -D _UNICODE -D _WIN32_IE=0x0500 -D WINVER=0x500 ${WARNS}
endif


all: Win32App.exe

Win32App.exe: ${OBJS}
	${CC} -o "$@" ${OBJS} ${LDFLAGS}

clean:
	rm -f obj/*.o "Win32App.exe"

obj/%.o: src/%.c ${HEADERS}
	${CC} ${CFLAGS} ${INCLUDE_DIRS} -c $< -o $@

obj/resource.o: res/resource.rc res/Application.manifest res/Application.ico include/resource.h
	${RC} -I./include -I./res -i $< -o $@
