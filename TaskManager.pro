QT       += core gui sql widgets printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    taskdialog.cpp \
    datamanager.cpp \
    reminderworker.cpp \
    networkmanager.cpp \
    categorydialog.cpp

HEADERS += \
    mainwindow.h \
    taskdialog.h \
    datamanager.h \
    reminderworker.h \
    networkmanager.h \
    categorydialog.h

FORMS += \
    mainwindow.ui \
    taskdialog.ui \
    categorydialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
