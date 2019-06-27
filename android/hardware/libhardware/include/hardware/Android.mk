LOCAL_PATH := $(call my-dir)
 
# HAL module implemenation stored in
# hw/<POWERS_HARDWARE_MODULE_ID>.<ro.hardware>.so
include $(CLEAR_VARS)
 
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_RELATIVE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_PROPRIETARY_MODULE := true
LOCAL_C_INCLUDES += hardware/libhardware/include
LOCAL_SHARED_LIBRARIES := liblog libcutils libhardware
LOCAL_SRC_FILES := huins.c
LOCAL_MODULE := huins.default
include $(BUILD_SHARED_LIBRARY)
