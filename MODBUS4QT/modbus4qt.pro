#-------------------------------------------------
#  modbus4qt Library
#  Author: Leonid Kolesnik, l.kolesnik@m-i.ru
#  Copyright (C) 2012-2021
#  https://mt11.net.ru/elvactech-lab/modbus4qt
#-------------------------------------------------
#
# This file is part of modbus4qt.
#
# modbus4qt is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.

# modbus4qt is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# modbus4qt.
#
# If not, see <https://www.gnu.org/licenses/>.
#
#-------------------------------------------------

include (modbus4qt_config.pri)

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = \
    modbus4qt

contains (MODBUS4QT_CONFIG, modbus4qt_demo) {
    SUBDIRS += demo
}

#modbus4qtspec.files  = modbus4qt_config.pri modbus4qt_functions.pri modbus4qt.prf
#modbus4qtspec.path  = $${MODBUS4QT_INSTALL_FEATURES}

#INSTALLS += modbus4qtspec
