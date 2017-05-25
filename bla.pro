#-------------------------------------------------
#
# Project created by QtCreator 2017-04-26T16:29:22
#
#-------------------------------------------------

QT       += core gui
QT       += widgets serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bla
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32{
    CHAI3D = C:/chai3d-3.2.0

    DEFINES += WIN64
    DEFINES += D_CRT_SECURE_NO_DEPRECATE
    QMAKE_CXXFLAGS += /EHsc /MP

    INCLUDEPATH += $${CHAI3D}/src
    INCLUDEPATH += $${CHAI3D}/external/Eigen
    INCLUDEPATH += $${CHAI3D}/external/glew/include
    INCLUDEPATH += $${CHAI3D}/extras/GLFW/include

    DEPENDPATH += $${CHAI3D}/src
    LIBS += -L$${CHAI3D}/lib/Release/x64/ -lchai3d -lglu32 -lopengl32 -lwinmm
 #   LIBS += -L$${CHAI3D}/extras/remotehaptics -lchai3d -lglu32 -lopengl32 -lwinmm -lremotehaptics
    LIBS += -L$${CHAI3D}/extras/GLFW/lib/Release/x64/ -lglfw
    LIBS += -lglu32 -lOpenGl32 -lglu32 -lOpenGl32 -lwinmm -luser32
    LIBS += kernel32.lib
    LIBS += user32.lib
    LIBS += gdi32.lib
    LIBS += winspool.lib
    LIBS += comdlg32.lib
    LIBS += advapi32.lib
    LIBS += shell32.lib
    LIBS += ole32.lib
    LIBS += oleaut32.lib
    LIBS += uuid.lib
    LIBS += odbc32.lib
    LIBS += odbccp32.lib
}



