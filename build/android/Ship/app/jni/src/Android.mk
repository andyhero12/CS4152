LOCAL_PATH  := $(call my-dir)
CURR_DEPTH  := ..
CUGL_OFFSET := ../../../../../../cugl

########################
#
# Main Application Entry
#
########################
CUGL_PATH := $(LOCAL_PATH)/$(CURR_DEPTH)/$(CUGL_OFFSET)
SDL2_PATH := $(CUGL_PATH)/sdlapp
PROJ_PATH := $(LOCAL_PATH)/${CURR_DEPTH}/../../../../..

include $(CLEAR_VARS)

LOCAL_MODULE := main
LOCAL_C_INCLUDES := $(CUGL_PATH)/include
LOCAL_C_INCLUDES += $(SDL2_PATH)/include
LOCAL_C_INCLUDES += $(CUGL_PATH)/external/box2d/include
LOCAL_C_INCLUDES += $(CUGL_PATH)/external/poly2tri
LOCAL_C_INCLUDES += $(PROJ_PATH)/source


# Add your application source files here.
LOCAL_PATH = $(PROJ_PATH)
LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,, \
	$(LOCAL_PATH)/source/SLShip.cpp \
	$(LOCAL_PATH)/source/SLAsteroidSet.cpp \
	$(LOCAL_PATH)/source/Monster.cpp \
	$(LOCAL_PATH)/source/BaseSet.cpp \
	$(LOCAL_PATH)/source/SLPhotonSet.cpp \
	$(LOCAL_PATH)/source/SpawnerController.cpp \
	$(LOCAL_PATH)/source/Base.cpp \
	$(LOCAL_PATH)/source/SLGameScene.cpp \
	$(LOCAL_PATH)/source/SLInputController.cpp \
	$(LOCAL_PATH)/source/Animation.cpp \
	$(LOCAL_PATH)/source/Spawner.cpp \
	$(LOCAL_PATH)/source/SLLoadingScene.cpp \
	$(LOCAL_PATH)/source/SLApp.cpp \
	$(LOCAL_PATH)/source/SLCollisionController.cpp \
	$(LOCAL_PATH)/source/main.cpp)

# Link in SDL2
LOCAL_SHARED_LIBRARIES := SDL2
LOCAL_SHARED_LIBRARIES += SDL2_image
LOCAL_SHARED_LIBRARIES += SDL2_ttf
LOCAL_SHARED_LIBRARIES += SDL2_atk
LOCAL_SHARED_LIBRARIES += SDL2_app

LOCAL_STATIC_LIBRARIES := CUGL

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lGLESv3 -lOpenSLES -llog -landroid
include $(BUILD_SHARED_LIBRARY)
