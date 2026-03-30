QT += core widgets

TARGET = test
TEMPLATE = app

SOURCES += test.cpp

# 设置目标平台为嵌入式 Linux
CONFIG += qt
CONFIG -= app_bundle

# 设置编译器选项
QMAKE_CXXFLAGS += -O2 -pipe

# 如果需要额外的库路径
# unix: LIBS += -L/path/to/libs -lmylib

# 如果需要额外的包含路径
# unix: INCLUDEPATH += /path/to/includes