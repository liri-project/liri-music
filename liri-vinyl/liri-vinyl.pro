TEMPLATE = app

QT += qml quick core gui sql

QTPLUGIN += qsvg
CONFIG += c++11
QMAKE_CXXFLAGS += -ltag

SOURCES += main.cpp \
    utilities.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =
# Default rules for deployment.
include(deployment.pri)

DISTFILES += \

INCLUDEPATH += /usr/local/include/ \
 /usr/include

HEADERS += \
    main.h \
    albumobject.h \
    artistobject.h \
    musicfolders.h \
    utilities.h

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += taglib

# As far as I can tell, taglib needs to be statically linked on windows.  See readme for more info.
win32:CONFIG(release, debug|release): LIBS += "C:/Program Files (x86)/taglib/lib/libtag.dll.a"
INCLUDEPATH += "C:/Program Files (x86)/taglib/include"
DEPENDPATH += "C:/Program Files (x86)/taglib/include"
