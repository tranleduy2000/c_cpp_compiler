#
# Fortran console application example
# type run to execute hello binary
# type clean to clean sources
# example from http://www.esm.psu.edu/~ajm138/fortranexamples.html
#

TARGET = recurs77

all: $(TARGET)

FCFLAGS = -O2 -Wall

OBJS = recurs.o

$(TARGET): $(OBJS)
		$(FC) -o $@ $^ $(LDFLAGS)

clean:
		rm $(OBJS) $(TARGET)

run: $(TARGET)
		run_ca $(TARGET)
