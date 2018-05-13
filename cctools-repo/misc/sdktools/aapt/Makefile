TARGETS = aapt zipalign

CFLAGS = -Iinclude -Iinclude/png \
	-DHAVE_SYS_UIO_H \
	-DHAVE_PTHREADS \
	-DHAVE_SCHED_H \
	-DHAVE_SYS_UIO_H \
	-DHAVE_IOCTL \
	-DHAVE_TM_GMTOFF \
	-DANDROID_SMP=0  \
	-DHAVE_ENDIAN_H \
	-DHAVE_POSIX_FILEMAP \
	-DHAVE_OFF64_T \
	-Wno-format-y2k \
	-DHAVE_ENDIAN_H \
	-DHAVE_SCHED_H

#	-DHAVE_PTHREADS \
#	-DHAVE_OFF64_T \
#	-DHAVE_ENDIAN_H \
#	-DHAVE_SYS_UIO_H \

CFLAGS += -Wno-format-y2k
CFLAGS += -DSTATIC_ANDROIDFW_FOR_TOOLS

ifeq (arm,$(CPU))
CFLAGS += -march=armv6
endif

ifeq (x86, $(CPU))
CFLAGS += -m32
endif

CFLAGS += -g

CFLAGS += $(EXTRA_CFLAGS)

CXXFLAGS = $(CFLAGS)

LDFLAGS = \
	-Landroidfw \
	-Llibutils \
	-Llibcutils \
	-Lliblog \
	-Llibpng

LDFLAGS += $(EXTRA_LDFLAGS)

LIBS = \
	-landroidfw \
	-lutils \
	-lcutils \
	-llog \
	-lpng \
	-lexpat \
	-lz \
	-lm

EXTLIBS = \
	liblog/liblog.a \
	libutils/libutils.a \
	androidfw/libandroidfw.a \
	libcutils/libcutils.a \
	libpng/libpng.a

all: $(EXTLIBS) $(TARGETS)

OBJS = \
	AaptAssets.o \
	Command.o \
	CrunchCache.o \
	FileFinder.o \
	Main.o \
	Package.o \
	StringPool.o \
	XMLNode.o \
	ResourceFilter.o \
	ResourceIdCache.o \
	ResourceTable.o \
	Images.o \
	Resource.o \
	pseudolocalize.o \
	SourcePos.o \
	WorkQueue.o \
	ZipEntry.o \
	ZipFile.o \
	qsort_r_compat.o

aapt: $(OBJS)
	$(CXX) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

zipalign: ZipAlign.o ZipEntry.o ZipFile.o
	$(CXX) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

liblog/liblog.a:
	make -C liblog CC=$(CC) CXX=$(CXX) CPU=$(CPU)

libutils/libutils.a:
	make -C libutils CC=$(CC) CXX=$(CXX) CPU=$(CPU)

androidfw/libandroidfw.a:
	make -C androidfw CC=$(CC) CXX=$(CXX) CPU=$(CPU)

libcutils/libcutils.a:
	make -C libcutils CC=$(CC) CXX=$(CXX) CPU=$(CPU)

libpng/libpng.a:
	make -C libpng CC=$(CC) CXX=$(CXX) CPU=$(CPU)

clean:
	rm -f $(OBJS) $(TARGETS) ZipAlign.o
	make -C liblog clean
	make -C libutils clean
	make -C androidfw clean
	make -C libcutils clean
	make -C libpng clean
