#-------------------------------------------------
#
# Project created by QtCreator 2018-11-22T10:47:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Demo
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
BASEDIR = ..
INCLUDEPATH += $$BASEDIR/Demo/GLTools/include
SRC_PATH = $$BASEDIR/Demo/GLTools/

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    glwidget.cpp \
    $$SRC_PATH/src/math3d.cpp \
    $$SRC_PATH/src/GLTriangleBatch.cpp \
    $$SRC_PATH/src/GLTools.cpp \
    $$SRC_PATH/src/GLShaderManager.cpp \
    $$SRC_PATH/src/glew.c \
    $$SRC_PATH/src/GLBatch.cpp

HEADERS += \
        mainwindow.h \
    glwidget.h \
    $$SRC_PATH/include/math3d.h \
    $$SRC_PATH/include/GLTriangleBatch.h \
    $$SRC_PATH/include/GLTools.h \
    $$SRC_PATH/include/GLShaderManager.h \
    $$SRC_PATH/include/GL/glew.h \
    $$SRC_PATH/include/GLBatch.h

FORMS += \
        mainwindow.ui

win32{
    LIBS += -lopengl32 -lglu32
}

unix{
    LIBS += -L/usr/local/lib -lGLU
}
