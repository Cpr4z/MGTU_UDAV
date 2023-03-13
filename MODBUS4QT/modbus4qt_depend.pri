#---------------------------------------------------------------
#
# modbus4qt library
# Copyright (C) 2012-2021
# Author: Leonid Kolesnik
#
#---------------------------------------------------------------
#
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
#
#---------------------------------------------------------------

MODBUS4QT_ROOT = $${PWD}
MODBUS4QT_BUILD = $${MODBUS4QT_ROOT}/build

include($${MODBUS4QT_ROOT}/modbus4qt_config.pri)
include($${MODBUS4QT_ROOT}/modbus4qt_build.pri)

QT += network serialport

INCLUDEPATH += $${MODBUS4QT_ROOT}
LIBS += -L$${MODBUS4QT_BUILD}/lib -lmodbus4qt
