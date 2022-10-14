#-------------------------------------------------
#
# Project created by QtCreator 2022-08-10T20:56:14
#
#-------------------------------------------------

QT       += core gui widgets opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl openglwidgets

TARGET = 002_Triangle_VAO_VBO
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    axbopenglwidget.cpp

HEADERS += \
        mainwindow.h \
    axbopenglwidget.h

FORMS += \
        mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

