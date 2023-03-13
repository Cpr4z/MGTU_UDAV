#------------------------------------------------------------------------------
#
# Copyright (C) 2012-2014 MELZ-Invest JSC
# http://www.m-i.ru
# Author: Leonid Kolesnik, l.kolesnik@m-i.ru
#
#------------------------------------------------------------------------------

UDAV_ROOT = $${PWD}/..
include( $${UDAV_ROOT}/udav_config.pri )
include( $${UDAV_ROOT}/udav_build.pri )

TEMPLATE = lib
TARGET = $$qtLibraryTarget(udavcore)
DESTDIR = $${UDAV_BUILD}/lib

QT += network widgets

CONFIG += serialport

INCLUDEPATH += $${UDAV_ROOT}/drivers/driver

include($${UDAV_ROOT}/elvactech-lab/elvactools/elvactools_depend.pri)

contains(UDAV_CONFIG, udav_dll) {
    CONFIG += dll
    DEFINES += UDAV_CORE_DLL
}
else {
    CONFIG += staticlib
}

SOURCES +=  \
        core.cpp \
	udav_settings.cpp

HEADERS += \
        core.h \
	udav_settings.h

TRANSLATIONS = udav_core_ru.ts

#------------------------------------------------------
# Automatic translation compilation

tr.commands = lrelease $$_PRO_FILE_
QMAKE_EXTRA_TARGETS += tr
PRE_TARGETDEPS += tr

#------------------------------------------------------
# Install directives

target.path = $${UDAV_INSTALL_LIBS}

doc.files = $${UDAV_ROOT}/doc/html
unix:doc.files += $${UDAV_ROOT}/doc/man
doc.path = $${UDAV_DOCS}

INSTALLS = target doc

headers.files  = $${HEADERS}
headers.path   = $${UDAV_INSTALL_HEADERS}

RESOURCES += \
    core.qrc

