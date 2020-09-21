QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    kernel/calc_similarity.cc \
    kernel/cfg_dfs.cc \
    kernel/lexical_analyzer.cc \
    kernel/longest_common_subsequence.cc \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    kernel/calc_similarity.h \
    kernel/cfg_dfs.h \
    kernel/lexical_analyzer.h \
    kernel/longest_common_subsequence.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
