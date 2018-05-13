#
# Fortran console application example
# type run to execute hello binary
# type clean to clean sources
# example from http://en.wikibooks.org/wiki/Fortran/Hello_world
#

TARGET = hello

all: $(TARGET)

FCFLAGS = -O2 -Wall

OBJS = hello.o

$(TARGET): $(OBJS)
		$(FC) -o $@ $^ $(LDFLAGS)

clean:
		rm $(OBJS) $(TARGET)

run: $(TARGET)
		run_ca $(TARGET)
