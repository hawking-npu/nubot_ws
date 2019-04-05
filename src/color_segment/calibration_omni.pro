#-------------------------------------------------
#
# Project created by QtCreator 2014-04-15T08:52:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = calibration_omni
TEMPLATE = app


SOURCES += src/main.cpp\
           src/calibration_dialog.cpp \
           src/image_subscribe.cpp    \
           src/whites.cpp             \
           src/scanpoints.cpp          \
           src/omniimage.cpp \
           src/fieldinformation.cpp \
           src/errortable.cpp


LIBS +=/usr/local/lib/libroscpp.so          \
#-/usr/lib/x86_64-linux-gnu/libpthread.so          \
#-/usr/lib/libboost_signals-mt.so                  \
#-/usr/lib/libboost_filesystem-mt.so              \
#-/usr/lib/libboost_system-mt.so                   \
/usr/local/lib/libcpp_common.so             \
/usr/local/lib/libroscpp_serialization.so   \
/usr/local/lib/librostime.so                \
/usr/lib/libboost_date_time-mt.so                \
/usr/lib/libboost_thread-mt.so                   \
/usr/local/lib/librosconsole.so             \
/usr/lib/libboost_regex-mt.so                    \
/usr/lib/liblog4cxx.so                           \
/usr/local/lib/libxmlrpcpp.so               \
/usr/local/lib/libmessage_filters.so        \
/usr/local/lib/libimage_transport.so        \
/usr/lib/libtinyxml.so                           \
/usr/local/lib/libclass_loader.so           \
/usr/lib/libPocoFoundation.so                    \
/usr/lib/x86_64-linux-gnu/libdl.so               \
/usr/local/lib/libconsole_bridge.so         \
/usr/local/lib/libroslib.so                 \
/usr/local/lib/libcv_bridge.so              \
/usr/local/lib/libopencv_calib3d.so         \
/usr/local/lib/libopencv_contrib.so         \
/usr/local/lib/libopencv_core.so            \
/usr/local/lib/libopencv_features2d.so      \
/usr/local/lib/libopencv_flann.so           \
/usr/local/lib/libopencv_gpu.so             \
/usr/local/lib/libopencv_highgui.so         \
/usr/local/lib/libopencv_imgproc.so         \
/usr/local/lib/libopencv_legacy.so          \
/usr/local/lib/libopencv_ml.so              \
/usr/local/lib/libopencv_nonfree.so         \
/usr/local/lib/libopencv_objdetect.so       \
/usr/local/lib/libopencv_photo.so           \
/usr/local/lib/libopencv_stitching.so       \
/usr/local/lib/libopencv_superres.so        \
/usr/local/lib/libopencv_video.so           \
/usr/local/lib/libopencv_videostab.so       \
/usr/local/lib/libimage_geometry.so

INCLUDEPATH+= /usr/local/include  \
              /usr/local/include/opencv \
              /usr/local/include/opencv2 \
              /usr/include    \
              include/core    \
              include

HEADERS  += include/calibration_dialog.h \
            include/image_subscribe.h    \
            include/whites.h             \
            include/scanpoints.h         \
            include/core/core.hpp        \
            include/omniimage.h          \
            include/errortable.h         \
            include/fieldinformation.h   \


FORMS    += calibration_dialog.ui
