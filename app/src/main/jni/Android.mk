LOCAL_PATH := $(call my-dir)
ROOT_PATH  := $(LOCAL_PATH)
#include $(LOCAL_PATH)/unzip11/Android.mk

include $(ROOT_PATH)/utils/Android.mk
include $(ROOT_PATH)/native-loader/Android.mk
