#-------------------------------------------------
#
# Project created by QtCreator 2014-11-09T14:22:21
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Praktika_1-2
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

# LIBS += -L/home/marc/Entwicklungsumgebungen/opencv-2.4.9/release/lib
unix:!macx: LIBS += -lopencv_core -lopencv_calib3d -lopencv_contrib -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts

HEADERS += main.h
