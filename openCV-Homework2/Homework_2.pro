#-------------------------------------------------
#
# Project created by QtCreator 2014-11-05T20:16:21
#
#-------------------------------------------------

QT       += core

QT       -= gui

QMAKE_CXXFLAGS = -std=c++11

TARGET = Homework_2
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    filter.cpp \
    multiset.cpp \
    harrisDetector.cpp

# LIBS += -L/home/marc/Entwicklungsumgebungen/opencv-2.4.9/release/lib
unix:!macx: LIBS += -lopencv_core -lopencv_calib3d -lopencv_contrib -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts

HEADERS += main.h \
    filter.h \
    multiset.h \
    harrisDetector.h
