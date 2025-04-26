# TrainingManagement.pro

QT += core gui sql printsupport network charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrainingManagement
TEMPLATE = app

CONFIG += c++17

# Debug output to verify flags
message("C++ flags: $$QMAKE_CXXFLAGS")

# Add include paths
INCLUDEPATH += $$PWD/managers
INCLUDEPATH += $$PWD/ui/mainwindow  # Added this line to include the mainwindow directory
INCLUDEPATH += $$PWD/core
INCLUDEPATH += $$PWD/dialog/updateclientdialog
INCLUDEPATH += $$PWD/dialog/updatetrainingdialog
INCLUDEPATH += $$PWD/ui/chartwindow

HEADERS += \
    core/connection.h \
    core/customsqlquerymodel.h \
    core/emailsender.h \
    core/notificationmanager.h \
    ui/chartwindow/chartwindow.h \
    ui/mainwindow/mainwindow.h \
    dialog/updateclientdialog/updateclientdialog.h \
    dialog/updatetrainingdialog/updatetrainingdialog.h \
    managers/client/client.h \
    managers/client/clientmanager.h \
    managers/training/formations.h \
    managers/training/trainingmanager.h

SOURCES += \
    core/connection.cpp \
    core/customsqlquerymodel.cpp \
    core/emailsender.cpp \
    core/main.cpp \
    core/notificationmanager.cpp \
    ui/chartwindow/chartwindow.cpp \
    ui/mainwindow/mainwindow.cpp \
    dialog/updateclientdialog/updateclientdialog.cpp \
    dialog/updatetrainingdialog/updatetrainingdialog.cpp \
    managers/client/client.cpp \
    managers/client/clientmanager.cpp \
    managers/training/formations.cpp \
    managers/training/trainingmanager.cpp


FORMS += \
    ui/chartwindow/ChartWindow.ui \
    ui/mainwindow/mainwindow.ui \
    dialog/updateclientdialog/updateclientdialog.ui \
    dialog/updatetrainingdialog/updatetrainingdialog.ui

RESOURCES += \
    resources/img.qrc

DISTFILES += \
    assets/background.jpg

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Include the custom project tree structure
include(client.files)
