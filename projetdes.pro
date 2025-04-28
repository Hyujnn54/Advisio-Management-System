QT += core gui sql printsupport charts network serialport widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Atelier_Connexion
TEMPLATE = app
QT += multimedia
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++17

SOURCES += \
    arduino.cpp \
    connection.cpp \
    customsqlquerymodel.cpp \
    formations.cpp \
    main.cpp \
    formationwindow.cpp \
    updatetrainingdialog.cpp \
    waitingroomdialog.cpp

HEADERS += \
    arduino.h \
    connection.h \
    customsqlquerymodel.h \
    formations.h \
    formationwindow.h \
    updatetrainingdialog.h \
    waitingroomdialog.h

FORMS += \
    formationwindow.ui

RESOURCES += \
    img.qrc

DISTFILES += \
    remove_person_icon.png
