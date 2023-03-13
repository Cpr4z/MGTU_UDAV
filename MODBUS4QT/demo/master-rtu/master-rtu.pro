#*****************************************************************************
# modbus4qt Library
# Author: Leonid Kolesnik, l.kolesnik@m-i.ru
# Copyright (C) 2012-2015
# http://mt11.net.ru
#*****************************************************************************/

#*****************************************************************************
# This file is part of modbus4qt.
#
# modbus4qt is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# modbus4qt is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# modbus4qt.
#
# If not, see <https://www.gnu.org/licenses/>.
#****************************************************************************/

MODBUS4QT_ROOT = $${PWD}/../../

include( $${MODBUS4QT_ROOT}/demo/demo.pri )

TEMPLATE = app

TARGET = master-rtu

MOC_DIR = $${MOC_DIR}/demo/master-rtu
OBJECTS_DIR = $${OBJECTS_DIR}/demo/master-rtu

SOURCES += \
    main.cpp \
    main_window.cpp \

HEADERS  += \
    main.h \
    main_window.h

#target.path = $${MODBUS4QT_INSTALL_BIN}

#
INSTALLS = target
