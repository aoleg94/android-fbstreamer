LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
# give module name
LOCAL_MODULE    := fbstreamer
# list your C files to compile
LOCAL_SRC_FILES := main.c loop.c\
libjpeg-turbo/jaricom.c\
libjpeg-turbo/jcapimin.c\
libjpeg-turbo/jcapistd.c\
libjpeg-turbo/jcarith.c\
libjpeg-turbo/jccoefct.c\
libjpeg-turbo/jccolor.c\
libjpeg-turbo/jcdctmgr.c\
libjpeg-turbo/jchuff.c\
libjpeg-turbo/jcinit.c\
libjpeg-turbo/jcmainct.c\
libjpeg-turbo/jcmarker.c\
libjpeg-turbo/jcmaster.c\
libjpeg-turbo/jcomapi.c\
libjpeg-turbo/jcparam.c\
libjpeg-turbo/jcphuff.c\
libjpeg-turbo/jcprepct.c\
libjpeg-turbo/jcsample.c\
libjpeg-turbo/jctrans.c\
libjpeg-turbo/jdapimin.c\
libjpeg-turbo/jdapistd.c\
libjpeg-turbo/jdarith.c\
libjpeg-turbo/jdatadst.c\
libjpeg-turbo/jdatadst-tj.c\
libjpeg-turbo/jdatasrc.c\
libjpeg-turbo/jdatasrc-tj.c\
libjpeg-turbo/jdcoefct.c\
libjpeg-turbo/jdcolor.c\
libjpeg-turbo/jddctmgr.c\
libjpeg-turbo/jdhuff.c\
libjpeg-turbo/jdinput.c\
libjpeg-turbo/jdmainct.c\
libjpeg-turbo/jdmarker.c\
libjpeg-turbo/jdmaster.c\
libjpeg-turbo/jdmerge.c\
libjpeg-turbo/jdphuff.c\
libjpeg-turbo/jdpostct.c\
libjpeg-turbo/jdsample.c\
libjpeg-turbo/jdtrans.c\
libjpeg-turbo/jerror.c\
libjpeg-turbo/jfdctflt.c\
libjpeg-turbo/jfdctfst.c\
libjpeg-turbo/jfdctint.c\
libjpeg-turbo/jidctflt.c\
libjpeg-turbo/jidctfst.c\
libjpeg-turbo/jidctint.c\
libjpeg-turbo/jidctred.c\
libjpeg-turbo/jmemmgr.c\
libjpeg-turbo/jmemnobs.c\
libjpeg-turbo/jquant1.c\
libjpeg-turbo/jquant2.c\
libjpeg-turbo/jsimd_none.c\
libjpeg-turbo/jutils.c\
libjpeg-turbo/tjutil.c\
libjpeg-turbo/transupp.c\
libjpeg-turbo/turbojpeg.c\

LOCAL_C_INCLUDES += libjpeg-turbo . jni
LOCAL_CFLAGS += -fPIE -std=c99 -fopenmp -flto
LOCAL_LDFLAGS += -fPIE -pie -fopenmp -flto
# this option will build executables instead of building library for android application.
include $(BUILD_EXECUTABLE)
