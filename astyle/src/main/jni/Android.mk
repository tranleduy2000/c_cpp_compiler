LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE  := astyle

LOCAL_SRC_FILES := /src/ASBeautifier.cpp \
                   /src/ASEnhancer.cpp \
                   /src/ASFormatter.cpp \
                   /src/ASLocalizer.cpp \
                   /src/ASResource.cpp \
                   /src/astyle_main.cpp

LOCAL_LDLIBS := -llog
LOCAL_CXX_FLAGS+=-std=c++11

include $(BUILD_SHARED_LIBRARY)
