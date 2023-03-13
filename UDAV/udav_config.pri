#------------------------------------------------------------------------------
#
# UDAV System
# Copyright (C) 2014 MELZ-Invest JSC
# http://www.m-i.ru
# Author: Leonid Kolesnik, l.kolesnik@m-i.ru
#
#------------------------------------------------------------------------------
UDAV_VER_MAJ = 1
UDAV_VER_MIN = 0
UDAV_VER_PAT = 0
UDAV_VERSION = $${UDAV_VER_MAJ}.$${UDAV_VER_MIN}.$${UDAV_VER_PAT}

#------------------------------------------------------------------------------
# Install paths
#------------------------------------------------------------------------------

unix: {
    UDAV_INSTALL_PREFIX    = /usr/local
    UDAV_INSTALL_BIN       = $${UDAV_INSTALL_PREFIX}/bin
    UDAV_INSTALL_DRIVERS   = $${UDAV_INSTALL_PREFIX}/lib/udav
    UDAV_INSTALL_DOCS      = $${UDAV_INSTALL_PREFIX}/doc/udav
    UDAV_INSTALL_HEADERS   = $${UDAV_INSTALL_PREFIX}/include/udav
    UDAV_INSTALL_LIBS      = $${UDAV_INSTALL_PREFIX}/lib
}

#------------------------------------------------------------------------------
# Features
# When building application with qmake you might want to load
# the compiler/linker flags, that are required to build it
# from udav.prf. Therefore all you need to do is to add "CONFIG += udav"
# to your project file and take care, that udav.prf can be found by qmake.
# ( see http://doc.trolltech.com/4.7/qmake-advanced-usage.html#adding-new-configuration-features )
# I recommend not to install the udav features together with the
# Qt features, because you will have to reinstall the udav features,
# with every Qt upgrade.

UDAV_INSTALL_FEATURES  = $${UDAV_INSTALL_PREFIX}/features

#------------------------------------------------------------------------------
# Build the static/shared libraries.
# If dll is enabled, a shared library is built, otherwise
# it will be a static library.

UDAV_CONFIG += udav_dll

#------------------------------------------------------------------------------
# Build client as a combo application
#
# Both client and server will be linked in one app called udavc
# It is usefull for testing and running simulators on one PC

UDAV_CONFIG += udav_combo
