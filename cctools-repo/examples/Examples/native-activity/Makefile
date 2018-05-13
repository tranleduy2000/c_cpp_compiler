#
# Native Activity example
# type run to execute Native Activity
# type clean to clean sources
#

TARGET = libnative-activity.so

all: $(TARGET)

NAG = $(CCTOOLSDIR)/sources/native_app_glue

CFLAGS = -Os -Wall -I$(NAG)
OBJS = native-activity.o $(NAG)/android_native_app_glue.o
LDFLAGS = -Wl,-soname,$(TARGET) -shared -Wl,--no-undefined -Wl,-z,noexecstack
LIBS = -llog -landroid -lEGL -lGLESv1_CM

$(TARGET): $(OBJS)
		$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
		rm $(OBJS) $(TARGET)

run: $(TARGET)
		run_na $(TARGET)
