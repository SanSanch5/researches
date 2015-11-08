#-------------------------------------------------
#
# Project created by QtCreator 2015-11-08T16:10:31
#
#-------------------------------------------------

#QT       += core

QT       -= gui

TARGET = sorts
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11
QMAKE_CXXFLAGS_WARN_ON = -Wno-reorder

TEMPLATE = app


SOURCES += \
    sorts.cpp \
    resultsanalyser.cpp

HEADERS += \
    heap.h \
    heap_sort.h \
    insertion_sorts.h \
    merge_sort.h \
    quick_sorts.h \
    resultsanalyser.h
