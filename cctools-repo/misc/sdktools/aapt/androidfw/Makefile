TARGET	= libandroidfw.a

OBJS = \
	Asset.o \
	AssetDir.o \
	AssetManager.o \
	misc.o \
	ObbFile.o \
	ResourceTypes.o \
	StreamingZipInflater.o \
	ZipFileRO.o \
	ZipUtils.o

CFLAGS = \
	-O2 \
	-Wall \
	-DFAKE_LOG_DEVICE=1 \
	-DHAVE_PTHREADS \
	-DHAVE_OFF64_T \
	-DHAVE_ENDIAN_H \
	-DHAVE_SYS_UIO_H \
	-DHAVE_POSIX_FILEMAP \
	-DSTATIC_ANDROIDFW_FOR_TOOLS \
	-I../include

ifeq (arm,$(CPU))
#CFLAGS += -march=armv6
CFLAGS += -march=armv5te
endif

ifeq (x86, $(CPU))
CFLAGS += -m32
endif

CFLAGS += -g

CXXFLAGS = $(CFLAGS)

$(TARGET): $(OBJS)
	$(AR) rcs $@ $^

clean:
	rm -f $(OBJS) $(TARGET)
