TARGET	= libpng.a

OBJS = \
	png.o \
	pngerror.o \
	pnggccrd.o \
	pngget.o \
	pngmem.o \
	pngpread.o \
	pngread.o \
	pngrio.o \
	pngrtran.o \
	pngrutil.o \
	pngset.o \
	pngtrans.o \
	pngvcrd.o \
	pngwio.o \
	pngwrite.o \
	pngwtran.o \
	pngwutil.o

CFLAGS = \
	-O2 \
	-Wall \
	-std=gnu89 \
	-fvisibility=hidden \
	-I../include/png \
	-I.

ifeq (arm,$(CPU))
#CFLAGS += -march=armv6
CFLAGS += -march=armv5te
endif

ifeq (x86, $(CPU))
CFLAGS += -m32
endif

CFLAGS += -g

$(TARGET): $(OBJS)
	$(AR) rcs $@ $^

clean:
	rm -f $(OBJS) $(TARGET)
