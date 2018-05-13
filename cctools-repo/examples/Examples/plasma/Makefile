#
# Native Activity plasma example
# type run to execute plasma
# type clean to clean sources
#

TARGET = libplasma.so

all: $(TARGET)

NAG = $(CCTOOLSDIR)/sources/native_app_glue

CFLAGS = -Os -Wall -I$(NAG)
OBJS = plasma.o $(NAG)/android_native_app_glue.o
LDFLAGS = -Wl,-soname,$(TARGET) -shared -Wl,--no-undefined -Wl,-z,noexecstack
LIBS = -llog -landroid -lm

$(TARGET): $(OBJS)
		$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
		rm $(OBJS) $(TARGET)

run: $(TARGET)
		run_na $(TARGET)
