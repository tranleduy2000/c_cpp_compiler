LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := miniunz

LOCAL_SRC_FILES := libminiunz.c unzip.c ioapi.c

LOCAL_CFLAGS := -O2 -DUSE_FILE32API -Wall
LOCAL_LDLIBS := -lz -llog

include $(BUILD_SHARED_LIBRARY)
