#------------------------------------------------------------------------------
# modbus4qt Library
# Author: Leonid Kolesnik, l.kolesnik@m-i.ru
# Copyright (C) 2012-2021
# https://mt11.net.ru
#------------------------------------------------------------------------------
#
# This library is free software.
#
# The contents of this file are subject to the Mozilla Public License Version 1.1
# (the "License"); you may not use this file except in compliance with the
# License. You may obtain a copy of the License at http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
# the specific language governing rights and limitations under the License.
#
# Alternatively, the contents of this file may be used under the terms of the
# GNU General Public License Version 2 or later (the "GPL"), in which case
# the provisions of the GPL are applicable instead of those above. If you wish to
# allow use of your version of this file only under the terms of the GPL and not
# to allow others to use your version of this file under the MPL, indicate your
# decision by deleting the provisions above and replace them with the notice and
# other provisions required by the GPL. If you do not delete the provisions
# above, a recipient may use your version of this file under either the MPL or
# the GPL.
#
#------------------------------------------------------------------------------

MODBUS4QT_ROOT = $${PWD}/..

include( $${MODBUS4QT_ROOT}/modbus4qt_config.pri )
include( $${MODBUS4QT_ROOT}/modbus4qt_build.pri )
include( $${MODBUS4QT_ROOT}/modbus4qt_paths.pri )

TEMPLATE = lib
TARGET   = $$qtLibraryTarget(modbus4qt)

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#
DEFINES += QT_DEPRECATED_WARNINGS


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#
# disables all the APIs deprecated before Qt 5.0.0
#
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050000

CONFIG += c++11

DESTDIR     = $${MODBUS4QT_BUILD}/lib
MOC_DIR     = $${MOC_DIR}/modbus4qt
OBJECTS_DIR = $${OBJECTS_DIR}/modbus4qt

QT += network serialport

contains(MODBUS4QT_CONFIG, modbus4qt_dll) {
    CONFIG += dll
    win32: DEFINES += QT_DLL MODBUS4QT_DLL MODBUS4QT_MAKEDLL
}
else {
    CONFIG += staticlib
}

#------------------------------------------------------------------------------
# Source and header files
#

SOURCES += abstract_device.cpp \
    client.cpp \
    memory_utils.cpp \
    rtu_device.cpp \
    server_internal_data.cpp \
    rtu_client.cpp \
    rtu_server.cpp \
    server.cpp \
    tcp_client.cpp \
    tcp_device.cpp \
    tcp_server.cpp

HEADERS += global.h \
    abstract_device.h \
    client.h \
    memory_utils.h \
    rtu_device.h \
    server_internal_data.h \
    rtu_client.h \
    rtu_server.h \
    server.h \
    tcp_client.h \
    tcp_device.h \
    tcp_server.h

#------------------------------------------------------------------------------
# Install directives
#

#target.path     = $${MODBUS4QT_INSTALL_LIBS}

#doc.files       = $${MODBUS4QT_ROOT}/doc/html
#unix:doc.files += $${MODBUS4QT_ROOT}/doc/man
#doc.path        = $${MODBUS4QT_INSTALL_DOCS}

#INSTALLS        = target doc

#headers.files   = $${HEADERS}
#headers.path    = $${MODBUS4QT_INSTALL_HEADERS}

#INSTALLS       += headers
