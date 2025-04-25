QT       += core gui
QT       += sql
QT       += charts
QT       += printsupport
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += console
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arduino.cpp \
    connection.cpp \
    employee.cpp \
    imageDelegate.cpp \
    main.cpp \
    mainwindow.cpp \
    sign.cpp \
    updateemployeedialog.cpp \
    qrcodegen/qrcodegen.cpp

HEADERS += \
    arduino.h \
    connection.h \
    employee.h \
    imageDelegate.h \
    mainwindow.h \
    sign.h \
    updateemployeedialog.h

FORMS += \
    UpdateClientDialog.ui \
    mainwindow.ui \
    sign.ui

TRANSLATIONS += \
    testing_fr_TN.ts
CONFIG += lrelease
CONFIG += embed_translations
CONFIG += debug
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# Include Paths
INCLUDEPATH += C:\Users\Admin\Documents\testing\qrcodegen
INCLUDEPATH += C:/opencv/build/install/include

LIBS += -LC:/opencv/build/lib \
        -lopencv_core480 \
        -lopencv_imgproc480 \
        -lopencv_imgcodecs480 \
        -lopencv_highgui480 \
        -lopencv_videoio480 \
        -lopencv_objdetect480 \
        -lopencv_face480 \



RESOURCES += \
    resources.qrc
