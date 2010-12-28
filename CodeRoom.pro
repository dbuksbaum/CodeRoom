#-------------------------------------------------
#
# Project created by QtCreator 2010-12-25T16:50:54
#
#-------------------------------------------------

QT       += core gui

TARGET = CodeRoom
TEMPLATE = app


SOURCES += main.cpp\
        editor.cpp \
    textedit.cpp \
    highlighter.cpp

HEADERS  += editor.h \
    textedit.h \
    highlighter.h

OTHER_FILES += \
    syntax/cpp.syn \
    readme.txt \
    style.sty
