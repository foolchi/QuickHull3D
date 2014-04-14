#-------------------------------------------------
#
# Project created by QtCreator 2014-01-20T10:37:45
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
#QMAKE_CXXFLAGS += fno-rounding-math
QMAKE_CXXFLAGS += -frounding-math
QMAKE_CXXFLAGS = -frounding-math
#QT       += console
#QT      +=lGNU
#QT      +=lglut
#LIBS += -lCGAL
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Projet-IN204_Fu-Huber
TEMPLATE = app


SOURCES += main.cpp\
    zonotope.cpp \
    face.cpp \
    facet.cpp \
    vector3d.cpp \
    facelist.cpp \
    halfedge.cpp \
    quickhull3d.cpp \
    zonoview.cpp \
    mainwindow.cpp

HEADERS  += \
    zonotope.h \
    point.h \
    func.h \
    face.h \
    facet.h \
    vector3d.h \
    point3d.h \
    vertex.h \
    vertexlist.h \
    halfedge.h \
    facelist.h \
    quickhull3d.h \
    zonoview.h \
    calcthread.h \
    mainwindow.h
