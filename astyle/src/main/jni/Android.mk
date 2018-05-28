LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE  := astyle

LOCAL_SRC_FILES := ASBeautifier.cpp \
                   ASEnhancer.cpp \
                   ASFormatter.cpp \
                   ASLocalizer.cpp \
                   ASResource.cpp \
                   astyle_main.cpp

LOCAL_LDLIBS := -llog
LOCAL_CXX_FLAGS+=-std=c++11

include $(BUILD_SHARED_LIBRARY)
