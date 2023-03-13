#------------------------------------------------------------------------------
#  modbus4qt Library
#  Author: Leonid Kolesnik, l.kolesnik@m-i.ru
#  Copyright (C) 2012-2021
#  https://mt11.net.ru
#------------------------------------------------------------------------------
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
#------------------------------------------------------------------------------

#MODBUS4QT_VER_MAJ = 1
#MODBUS4QT_VER_MIN = 0
#MODBUS4QT_VER_PAT = 0
#MODBUS4QT_VERSION = $${MODBUS4QT_VER_MAJ}.$${MODBUS4QT_VER_MIN}.$${MODBUS4QT_VER_PAT}

#------------------------------------------------------------------------------
# Install paths
#------------------------------------------------------------------------------

#MODBUS4QT_INSTALL_PREFIX = $$[QT_INSTALL_PREFIX]

#unix {
#    MODBUS4QT_INSTALL_PREFIX    = /usr/local
#}

#win32 {
#    MODBUS4QT_INSTALL_PREFIX = c:/lib/modbus4qt-$$MODBUS4QT_VERSION
#}

#MODBUS4QT_INSTALL_BIN       = $${MODBUS4QT_INSTALL_PREFIX}/bin
#MODBUS4QT_INSTALL_DOCS      = $${MODBUS4QT_INSTALL_PREFIX}/doc/modbus4qt
#MODBUS4QT_INSTALL_HEADERS   = $${MODBUS4QT_INSTALL_PREFIX}/include/modbus4qt
#MODBUS4QT_INSTALL_LIBS      = $${MODBUS4QT_INSTALL_PREFIX}/lib

#------------------------------------------------------------------------------
# Features
#
# When building modbus4qt application with qmake you might want to load
# the compiler/linker flags, that are required to build a modbus4qt application
# from modbus4qt.prf. Therefore all you need to do is to add "CONFIG += modbus4qt"
# to your project file and take care, that modbus4qt.prf can be found by qmake.
#
# I recommend not to install the modbus4qt features together with the
# Qt features, because you will have to reinstall the modbus4qt features,
# with every Qt upgrade.

#MODBUS4QT_INSTALL_FEATURES  = $${MODBUS4QT_INSTALL_PREFIX}/features

#------------------------------------------------------------------------------
# Build the static/shared libraries
#
# If dll is enabled, a shared library is built, otherwise
# it will be a static library.

MODBUS4QT_CONFIG += modbus4qt_dll

#------------------------------------------------------------------------------
# Build demo suite
#
# If you want to auto build the demo suite, enable the line below
# Otherwise you have to build them from the examples directory.

MODBUS4QT_CONFIG     += modbus4qt_demo
