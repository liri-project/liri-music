TEMPLATE = app

QT += qml quick
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

INCLUDEPATH += /usr/local/include/

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/release/ -ltag
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/debug/ -ltag
else:unix: LIBS += -L$$PWD/../../../../../usr/local/lib/ -ltag

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Qt5.5.1/5.5/gcc_64/lib/release/ -lQt5Sql
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Qt5.5.1/5.5/gcc_64/lib/debug/ -lQt5Sql
else:unix: LIBS += -L$$PWD/../../../Qt5.5.1/5.5/gcc_64/lib/ -lQt5Sql

INCLUDEPATH += $$PWD/../../../Qt/5.5/gcc_64/include
DEPENDPATH += $$PWD/../../../Qt/5.5/gcc_64/include
INCLUDEPATH += /home/nickg/Qt/5.5/gcc_64/include

HEADERS += \
    main.h \
    albumobject.h \
    artistobject.h \
    musicfolders.h \
    utilities.h
