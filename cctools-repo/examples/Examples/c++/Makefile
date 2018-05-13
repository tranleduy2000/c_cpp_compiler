#
# C++ console application example
# type run to execute cin
# type clean to clean sources
#
# touch screen in console mode to show keyboard
#

TARGET = cin

all: $(TARGET)

CFLAGS = -O2 -Wall

OBJS = cin.o

$(TARGET): $(OBJS)
		$(CXX) -o $@ $^ $(LDFLAGS)

clean:
		rm $(OBJS) $(TARGET)

run: $(TARGET)
		run_ca $(TARGET)
