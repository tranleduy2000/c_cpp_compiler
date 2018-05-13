LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := myutils

LOCAL_SRC_FILES := libutils.c libexec.cpp ../unzip11/libminiunz.c ../unzip11/unzip.c ../unzip11/ioapi.c

LOCAL_CFLAGS := -O2 -DUSE_FILE32API -Wall
LOCAL_LDLIBS := -lz -llog

include $(BUILD_SHARED_LIBRARY)
