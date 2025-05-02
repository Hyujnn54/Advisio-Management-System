QT += core gui sql printsupport charts network widgets serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += console



SOURCES += \
    connection.cpp \
    main.cpp \
    mainwindow.cpp \
    resource.cpp \
    searchdialog.cpp

HEADERS += \
    connection.h \
    mainwindow.h \
    resource.h \
    searchdialog.h

FORMS += \
    mainwindow.ui \
    searchdialog.ui

TRANSLATIONS += \
    Interfaces_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
