#
# C console application example
# type run to execute hello binary
# type clean to clean sources
#

TARGET = cin.exe

all: $(TARGET)

CXX = i686-w64-mingw32-g++
CXXFLAGS = -O2 -Wall

OBJS = cin.o

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
