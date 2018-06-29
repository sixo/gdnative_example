LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := android_gdnative
LOCAL_CPPFLAGS := -std=c++14
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_LDLIBS := -llog 

LOCAL_SRC_FILES := \
test.cpp

LOCAL_C_INCLUDES := \
D:/dev/godot/godot/modules/gdnative/include

include $(BUILD_SHARED_LIBRARY)
