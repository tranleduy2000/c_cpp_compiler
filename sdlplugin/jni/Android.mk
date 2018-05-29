LOCAL_PATH := $(call my-dir)
ROOT_PATH := ${LOCAL_PATH}
include $(CLEAR_VARS)
include $(LOCAL_PATH)/SDL/Android.mk

include $(ROOT_PATH)/SDL2_image/external/libwebp-0.3.0/Android.mk
include $(ROOT_PATH)/SDL2_image/Android.mk
include $(ROOT_PATH)/SDL2_mixer/external/libmikmod-3.1.12/Android.mk
include $(ROOT_PATH)/SDL2_mixer/external/smpeg2-2.0.0/Android.mk
include $(ROOT_PATH)/SDL2_mixer/Android.mk
include $(ROOT_PATH)/SDL2_net/Android.mk
include $(ROOT_PATH)/SDL2_ttf/Android.mk
include $(ROOT_PATH)/src/Android.mk
include $(ROOT_PATH)/utils/Android.mk
