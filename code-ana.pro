QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    kernel/buf_vuln_scan.cc \
    kernel/calc_similarity.cc \
    kernel/cfg_dfs.cc \
    kernel/func_scanner.cc \
    kernel/lexical_analyzer.cc \
    kernel/util/trie.cc \
    kernel/util/type.cc \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    kernel/buf_vuln_scan.h \
    kernel/calc_similarity.h \
    kernel/cfg_dfs.h \
    kernel/func_scanner.h \
    kernel/lexical_analyzer.h \
    kernel/util/trie.h \
    kernel/util/type.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
