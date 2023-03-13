#------------------------------------------------------------------------------
#
# Copyright (C) 2012-2021
# http://www.m-i.ru
# Author: Leonid Kolesnik, l.kolesnik@m-i.ru
#
#------------------------------------------------------------------------------

UDAV_ROOT = $${PWD}/..
include( $${UDAV_ROOT}/udav_config.pri )
include( $${UDAV_ROOT}/udav_build.pri )

TEMPLATE = lib
TARGET = $$qtLibraryTarget(udavdriver)
DESTDIR = $${UDAV_BUILD}/lib

INCLUDEPATH += $${UDAV_ROOT}

QT += serialport

#------------------------------------------------------------------------------
# Connect external libs

include( $${UDAV_ROOT}/elvactech-lab/modbus4qt/modbus4qt_depend.pri )
include( $${UDAV_ROOT}/elvactech-lab/elvacquip/elvacquip_depend.pri )
include( $${UDAV_ROOT}/elvactech-lab/elvactools/elvactools_depend.pri )

contains(UDAV_CONFIG, udav_dll) {
    CONFIG += dll
    DEFINES += UDAV_DRIVER_DLL
}
else {
    CONFIG += staticlib
}

LIBS += -L$${UDAV_ROOT}/elvactech-lab/elvacquip/build/lib

SOURCES +=  \
    driver_impl.cpp \
    driver_proxy.cpp \
    driver_settings.cpp \
    drivers_manager.cpp \
    meta_signal.cpp \
    process.cpp \
    driver_interface.cpp \
    devices/valve.cpp \
    devices/state_event.cpp \
    devices/state_transition.cpp \
    devices/device.cpp \
    devices/screw_nut_drive.cpp \
    devices/multi_axe_screw_nut_drive.cpp \
    devices/screw_nut_drive_command.cpp \
    devices/screw_nut_drive_transition.cpp


HEADERS += \
    driver_impl.h \
    driver_interface.h \
    driver_proxy.h \
    driver_settings.h \
    drivers.h \
    drivers_manager.h \
    meta_signal.h \
    process.h \
    devices/valve.h \
    devices/state_event.h \
    devices/state_transition.h \
    devices/device.h \
    devices/screw_nut_drive.h \
    devices/multi_axe_screw_nut_drive.h \
    devices/screw_nut_drive_command.h \
    devices/screw_nut_drive_transition.h



#------------------------------------------------------------------------------
# Automatic translation compilation

tr.commands = lrelease $$_PRO_FILE_
QMAKE_EXTRA_TARGETS += tr
PRE_TARGETDEPS += tr

#-------------------------------------------------------------------------------
# Install directives

target.path = $${UDAV_INSTALL_LIBS}

doc.files = $${UDAV_ROOT}/doc/html
unix:doc.files += $${UDAV_ROOT}/doc/man
doc.path = $${UDAV_DOCS}

INSTALLS = target doc

headers.files  = $${HEADERS}
headers.path   = $${UDAV_INSTALL_HEADERS}
