all: all_targets

BUILD_SHARED_LIBRARY := Makefile.shared
BUILD_STATIC_LIBRARY := Makefile.static

CC	:= $(CROSS_COMPILE)gcc
CXX	:= $(CROSS_COMPILE)g++
AR	:= $(CROSS_COMPILE)ar
RANLIB	:= $(CROSS_COMPILE)ranlib

my-dir := $(PWD)

include Android.mk

all_targets: $(ALL_LOCAL_MODULES)
	@echo $(ALL_LOCAL_MODULES)

clean:
	rm -f $(ALL_LOCAL_MODULES) $(OBJS)
