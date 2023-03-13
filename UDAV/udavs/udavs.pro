UDAV_ROOT = $${PWD}/..
include( $${UDAV_ROOT}/udav_config.pri)
include( $${UDAV_ROOT}/udav_build.pri)

QT += core network serialport

DEFINES -= UDAV_COMBO

TEMPLATE = app
TARGET = udavs

INCLUDEPATH += $${UDAV_ROOT}/core
INCLUDEPATH += $${UDAV_ROOT}/driver

LIBS += -ludavcore -ludavdriver

include($${UDAV_ROOT}/elvactech-lab/elvactools/elvactools_depend.pri)

SOURCES +=\
    udavs.cpp \
    server.cpp \
    server_settings.cpp \
    server_side_socket.cpp \
    server_application.cpp \
    factories.cpp

HEADERS  += \
    udavs.h \
    server.h \
    server_settings.h \
    server_side_socket.h \
    server_application.h \
    factories.h

TRANSLATIONS = udavs_ru.ts

#------------------------------------------------------------------------------
# Automatic translation compilation

tr.commands = lrelease $$_PRO_FILE_
QMAKE_EXTRA_TARGETS += tr
PRE_TARGETDEPS += tr

#------------------------------------------------------------------------------
# Install

target.path = $${UDAV_INSTALL_BIN}
INSTALLS = target

RESOURCES += \
    udavs.qrc
