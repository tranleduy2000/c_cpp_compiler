TARGET	= libutils.a

OBJS = \
	BasicHashtable.o \
	BlobCache.o \
	CallStack.o \
	FileMap.o \
	JenkinsHash.o \
	LinearAllocator.o \
	LinearTransform.o \
	Log.o \
	Looper.o \
	Printer.o \
	ProcessCallStack.o \
	PropertyMap.o \
	RefBase.o \
	SharedBuffer.o \
	Static.o \
	StopWatch.o \
	String8.o \
	String16.o \
	SystemClock.o \
	Threads.o \
	Timers.o \
	Tokenizer.o \
	Unicode.o \
	VectorImpl.o \
	misc.o

CFLAGS = \
	-O2 \
	-Wall \
	-DHAVE_SYS_UIO_H \
	-DHAVE_PTHREADS \
	-DHAVE_SCHED_H \
	-DHAVE_SYS_UIO_H \
	-DHAVE_IOCTL \
	-DHAVE_TM_GMTOFF \
	-DANDROID_SMP=0  \
	-DHAVE_ENDIAN_H \
	-DHAVE_POSIX_FILEMAP \
	-DHAVE_OFF64_T

CFLAGS += -I../include

ifeq (mips, $(CPU))
CFLAGS += -DALIGN_DOUBLE
endif

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
