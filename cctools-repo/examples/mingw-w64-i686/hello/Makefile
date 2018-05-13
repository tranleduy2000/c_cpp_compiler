#
# C console application example
# type run to execute hello binary
# type clean to clean sources
#

TARGET = hello.exe

all: $(TARGET)

CC = i686-w64-mingw32-gcc
CFLAGS = -O2 -Wall

OBJS = hello.o

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
