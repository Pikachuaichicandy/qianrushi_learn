QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
# Linux ARM 平台配置
unix {
    # ========== i.MX6 OpenCV 配置 ==========
    IMX6_SYSROOT = /opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi

    # OpenCV 头文件路径（来自 i.MX6 SDK）
    INCLUDEPATH += $${IMX6_SYSROOT}/usr/include
    INCLUDEPATH += $${IMX6_SYSROOT}/usr/include/opencv
    INCLUDEPATH += $${IMX6_SYSROOT}/usr/include/opencv2

    # OpenCV 库路径（来自 i.MX6 SDK）
    LIBS += -L$${IMX6_SYSROOT}/usr/lib

    # OpenCV 核心库
    LIBS += -lopencv_core
    LIBS += -lopencv_imgproc
    LIBS += -lopencv_imgcodecs
    LIBS += -lopencv_highgui        # 包含 cvLoadImage
    LIBS += -lopencv_videoio
    LIBS += -lopencv_objdetect
    LIBS += -lopencv_features2d
    LIBS += -lopencv_calib3d

    # ========== SeetaFace 配置 ==========
    # SeetaFace 头文件可能在 i.MX6 SDK 中，也可能在别处
    # 首先检查 i.MX6 SDK 中是否有
    # 如果没有，使用单独的路径

    # SeetaFace 库路径（单独的 ARM 库）
    SEETAFACE_LIB_PATH = /home/alientek/opencv/opencv4-arm/lib
    LIBS += -L$${SEETAFACE_LIB_PATH}

    # SeetaFace 库
    LIBS += -lSeetaFaceDetector
    LIBS += -lSeetaFaceLandmarker
    LIBS += -lSeetaFaceRecognizer
    LIBS += -lSeetaFaceTracker
    LIBS += -lSeetaNet
    LIBS += -lSeetaQualityAssessor

    # ========== 系统库 ==========
    LIBS += -ldl
    LIBS += -lpthread
    LIBS += -lrt
    LIBS += -lm

    # 设置运行时库路径
    QMAKE_RPATHDIR += $${IMX6_SYSROOT}/usr/lib
    QMAKE_RPATHDIR += $${SEETAFACE_LIB_PATH}
}

SOURCES += \
    camera.cpp \
    distance.cpp \
    icm20608.cpp \
    light.cpp \
    main.cpp \
    motor.cpp \
    people.cpp \
    temper.cpp \
    widget.cpp

HEADERS += \
    camera.h \
    distance.h \
    icm20608.h \
    light.h \
    motor.h \
    people.h \
    temper.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
