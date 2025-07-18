LOCAL_PATH := $(call my-dir)

# ============================================================================#
include $(CLEAR_VARS)
LOCAL_MODULE    := libdobby
LOCAL_SRC_FILES := Dobby/$(TARGET_ARCH_ABI)/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)
# ============================================================================#

include $(CLEAR_VARS)
LOCAL_MODULE           := main

LOCAL_CFLAGS           := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w
LOCAL_CFLAGS           += -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS         := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -Werror -s -std=c++23
LOCAL_CPPFLAGS         += -finput-charset=UTF-8 -fexec-charset=UTF-8
LOCAL_CPPFLAGS         += -Wno-error=c++11-narrowing -fms-extensions -fno-rtti -fno-exceptions -fpermissive -Os
LOCAL_LDFLAGS          += -Wl,--gc-sections,--strip-all, -llog
LOCAL_ARM_MODE         := arm
LOCAL_LDLIBS           := -llog -landroid -lEGL -lGLESv3 -lGLESv2 -lGLESv1_CM -lz

LOCAL_C_INCLUDES       := $(LOCAL_PATH)
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/Dooby
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/ImGui
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/Includes
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/And64InlineHook
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/Substrate
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/KittyMemory
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/ByNameModding
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/ByNameModding/include
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/ByNameModding/include/BNM
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/ByNameModding/src/private
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/ByNameModding/external/include
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/ByNameModding/external/utf8
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/Struct
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/Unity

FILE_LIST               := $(wildcard $(LOCAL_PATH)/ImGui/*.c*)
FILE_LIST               += $(wildcard $(LOCAL_PATH)/ImGui/backends/*.c*)
FILE_LIST               += $(wildcard $(LOCAL_PATH)/xdl/*.c*)
FILE_LIST               += $(wildcard $(LOCAL_PATH)/Struct/*.c*)
FILE_LIST               += $(wildcard $(LOCAL_PATH)/KittyMemory/*.c*)
FILE_LIST               += $(wildcard $(LOCAL_PATH)/ByNameModding/src/*.c*)
FILE_LIST               += $(wildcard $(LOCAL_PATH)/Unity/*.c*)
FILE_LIST               += $(wildcard $(BNM_PATH)/src/*.c*)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/*.c*)

LOCAL_SRC_FILES        := $(FILE_LIST:$(LOCAL_PATH)/%=%)


ifeq ($(TARGET_ARCH_ABI), arm64-v8a)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/And64InlineHook
    HOOK_SRC := $(wildcard $(LOCAL_PATH)/And64InlineHook/*.c*)
    LOCAL_SRC_FILES += $(HOOK_SRC:$(LOCAL_PATH)/%=%)
else ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/Substrate
    HOOK_SRC := $(wildcard $(LOCAL_PATH)/Substrate/*.c*)
    LOCAL_SRC_FILES += $(HOOK_SRC:$(LOCAL_PATH)/%=%)
endif

LOCAL_STATIC_LIBRARIES := libdobby
LOCAL_CPP_FEATURES     := exceptions

include $(BUILD_SHARED_LIBRARY)
