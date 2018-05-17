LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE:= jackpal-androidterm5
LOCAL_SRC_FILES:= common.cpp \
                  fileCompat.cpp \
                  termExec.cpp
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)
