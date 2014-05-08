#-------------------------------------------------
#
# Project created by QtCreator 2013-11-12T19:21:05
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp \
    errormessage.cpp \
    message.cpp \
    textmessage.cpp \
    chatroominterface.cpp \
    chatroom.cpp \
    mainuser.cpp \
    messagestruct.cpp \
    user.cpp \
    progress.cpp

HEADERS  += mainwindow.h \
    login.h \
    errormessage.h \
    message.h \
    textmessage.h \
    chatroominterface.h \
    chatroom.h \
    mainuser.h \
    messagestruct.h \
    user.h \
    progress.h

FORMS    += mainwindow.ui \
    login.ui \
    errormessage.ui \
    chatroom.ui \
    mainuser.ui \
    progress.ui
