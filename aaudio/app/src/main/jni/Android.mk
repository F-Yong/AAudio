
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := aaudio_test
LOCAL_SRC_FILES := aaudio_test.cpp
LOCAL_SRC_FILES += MyAAudio.cpp

LOCAL_LDLIBS := -laaudio -lm -llog
# LOCAL_SHARED_LIBRARIES := libaaudio


include $(BUILD_SHARED_LIBRARY)
