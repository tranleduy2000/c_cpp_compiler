#
# C console application example
# type run to execute hello binary
# type clean to clean sources
#

TARGET = hello

all: $(TARGET)

CFLAGS = -O2 -Wall

OBJS = hello.o

$(TARGET): $(OBJS)
		$(CC) -o $@ $^ $(LDFLAGS)

clean:
		rm $(OBJS) $(TARGET)

run: $(TARGET)
		run_ca $(TARGET)
