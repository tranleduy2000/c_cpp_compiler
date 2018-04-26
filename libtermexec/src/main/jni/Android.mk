LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE:= jackpal-termexec2
LOCAL_SRC_FILES := process.cpp
LOCAL_LDLIBS := -llog -lc
include $(BUILD_SHARED_LIBRARY)
