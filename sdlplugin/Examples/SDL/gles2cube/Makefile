#
# SDL application Makefile
# type without args to build
# type run to execute
# type clean to clean sources
# type log to check application log
#

TARGET = gles2cube

all: $(TARGET)

CFLAGS = -Os -Wall -DANDROID -I${EXTERNAL_STORAGE}/CCTools/SDL/include

CXXFLAGS = $(CFLAGS)

LDFLAGS = -shared ${EXTERNAL_STORAGE}/CCTools/SDL/lib/SDL_android_main.o -L${EXTERNAL_STORAGE}/CCTools/SDL/lib -lSDL2 -lGLESv2 -llog -lm

SDLRUN = am start $(shell am 2>&1| grep -q '\-\-user' && echo '--user 0') -n com.pdaxrom.cctools.sdlplugin/.sdlpluginActivity -e sdlmain

OBJS = $(TARGET).o shader.o matrix.o

$(TARGET): $(OBJS)
	$(CC) -o lib$@.so $^ $(LDFLAGS)

clean:
		rm -f $(OBJS) lib$(TARGET).so

run: $(TARGET)
		$(SDLRUN) $(PWD)/lib$(TARGET).so

log:
	logcat -d | grep SDL
