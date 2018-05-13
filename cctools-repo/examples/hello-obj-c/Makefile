#
# Obj-C console application example
# type run to execute hello binary
# type clean to clean sources
#

TARGET = hello

all: $(TARGET)

CFLAGS = -O2 -Wall

OBJS = hello.o

LIBS = -lobjc

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm $(OBJS) $(TARGET)

run: $(TARGET)
	run_ca $(TARGET)

%.o: %.m
	$(CC) $(CFLAGS) -c $<
