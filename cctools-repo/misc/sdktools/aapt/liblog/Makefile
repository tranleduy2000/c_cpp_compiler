TARGET	= liblog.a

OBJS = \
	event_tag_map.o  \
	fake_log_device.o  \
	logd_write.o  \
	logprint.o

#	logd_write.o \
#	fake_log_device.o \
#	uio.o

CFLAGS = \
	-O2 \
	-Wall \
	-DHAVE_SYS_UIO_H \
	-DFAKE_LOG_DEVICE=1

CFLAGS += -I../include

#	-DHAVE_PTHREADS \
#	-DHAVE_OFF64_T \
#	-DHAVE_ENDIAN_H \

ifeq (arm,$(CPU))
CFLAGS += -march=armv6
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
