#-------------------------------------------------
#
# Project created by QtCreator 2019-05-02T11:11:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = baidu_api_2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
        main.cpp \
        mainwindow.cpp \
    character_split.cpp \
    detect.cpp \
    ELM-master/elm_model.cpp \
    ELM-master/elm_in_elm_model.cpp \
    ELM-master/functions.cpp

HEADERS += \
        mainwindow.h \
    character_split.h \
    detect.h \
    ELM-master/elm_in_elm_model.h \
    ELM-master/elm_model.h \
    ELM-master/functions.h

FORMS += \
        mainwindow.ui


LIBS+=/home/manjasaka/Downloads/opencv-4.1.1/build/lib/libopencv_*.so
LIBS+= -L$$PWD/lib -ljsoncpp
LIBS+= -L$$PWD/lib -lcurl
LIBS+= -L$$PWD/lib -lcrypto
LIBS+= -L$$PWD/lib -lssl
LIBS+=-lpthread
LIBS+=-ldl

INCLUDEPATH+=/usr/local/include/opencv4
INCLUDEPATH+= $$PWD/aip-cpp-sdk-0.7.10
INCLUDEPATH+= $$PWD/curl-7.64.1/include
INCLUDEPATH+= $$PWD/jsoncpp-master/include

