QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#---------------------------------------------
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp
#----------------------------外部库-------------
#------------OpenCV----------------------------
INCLUDEPATH +=/usr/local/include/
              /usr/local/include/opencv2/
              /usr/local/include/opencv4/opencv2/

#/usr/local/include
#                /usr/local/include/opencv4

LIBS += /home/lwz/opencv-4.5.0/build/lib/libopencv_*

#  --------  ncnn----------------
INCLUDEPATH += /home/lwz/yolov5_ncnn/ncnn/build/install/include
               /home/lwz/yolov5_ncnn/ncnn/build/install/include/ncnn

LIBS += /home/lwz/yolov5_ncnn/ncnn/build/install/lib/libncnn.a
SOURCES += \
    detection.cpp \
    imageutil.cpp \
    main.cpp \
    mainwindow.cpp \
    segmentation.cpp \
    videoshow.cpp \
    videosource.cpp \
    weapon.cpp

HEADERS += \
    detection.h \
    imageutil.h \
    mainwindow.h \
    segmentation.h \
    threadpool.h \
    videoshow.h \
    videosource.h \
    weapon.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
