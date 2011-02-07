#-------------------------------------------------
#
# Project created by QtCreator 2010-12-25T16:50:54
#
#-------------------------------------------------

QT       += core gui

TARGET = CodeRoom
TEMPLATE = app
RC_FILE = coderoom.rc

SOURCES += main.cpp\
        editor.cpp \
    textedit.cpp \
    highlighter.cpp \
    textfind.cpp

HEADERS  += editor.h \
    textedit.h \
    highlighter.h \
    textfind.h

OTHER_FILES += \
    syntax/cpp.syn \
    readme.txt \
    style.sty \
    syntax/latex.syn \
    config.cfg \
    syntax/text.syn \
    coderoom.rc \
    syntax/coderoom.syn
