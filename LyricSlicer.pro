QT += core gui widgets
CONFIG += c++17

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES +=  main.cpp
SOURCES += AboutDialog.cpp
SOURCES += LyricSlicer.cpp
SOURCES += SlideWidget.cpp

HEADERS += AboutDialog.h
HEADERS += LyricSlicer.h
HEADERS += SlideWidget.h

FORMS += AboutDialog.ui
FORMS += LyricSlicer.ui
FORMS += SlideWidget.ui

RESOURCES += Resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
