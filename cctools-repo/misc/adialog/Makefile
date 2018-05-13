TARGET = adialog

CFLAGS = -g -O2 -Wall -Ilib -DNOSSL
OBJS = main.o lib/tcp.o

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

clean:
	rm -f $(TARGET) $(OBJS)
