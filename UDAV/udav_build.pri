#------------------------------------------------------------------------------
#
# Copyright (C) 2012-2014 MELZ-Invest JSC
# http://www.m-i.ru
# Author: Leonid Kolesnik, l.kolesnik@m-i.ru
#
#------------------------------------------------------------------------------

######################################################################
# qmake internal options
######################################################################

CONFIG           += qt
CONFIG           += warn_on
CONFIG           += silent

######################################################################
# release/debug mode
######################################################################

unix {

    CONFIG           += debug
    #CONFIG          += release

    VER_MAJ           = $${UDAV_VER_MAJ}
    VER_MIN           = $${UDAV_VER_MIN}
    VER_PAT           = $${UDAV_VER_PAT}
    VERSION           = $${UDAV_VERSION}
}

CONFIG(release){
    DEFINES += QT_NO_DEBUG QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
}

CONFIG(debug){
    DEFINES -= QT_NO_DEBUG QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
    DEFINES += DEBUG
}

contains(UDAV_CONFIG, udav_combo){
    DEFINES += UDAV_COMBO
}


######################################################################
# paths for building
######################################################################

UDAV_ROOT = $${PWD}
UDAV_BUILD = $${UDAV_ROOT}/build

DESTDIR = $${UDAV_BUILD}/bin
MOC_DIR = $${UDAV_BUILD}/moc
OBJECTS_DIR = $${UDAV_BUILD}/obj
UI_DIR = $${UDAV_BUILD}/ui
QMAKE_RPATHDIR *= $${UDAV_BUILD}/lib

LIBS += -L$${UDAV_BUILD}/lib
LIBS += -L$${UDAV_ROOT}/elvactech-lab/elvactools/build/lib
LIBS += -L$${UDAV_ROOT}/elvactech-lab/elvacquip/build/lib
LIBS += -L$${UDAV_ROOT}/elvactech-lab/modbus4qt/build/lib

unix {

    exists( $${QMAKE_LIBDIR_QT}/libmodbus4qt.* ) {

        # On some Linux distributions the Qwt libraries are installed
        # in the same directory as the Qt libraries. Unfortunately
        # qmake always adds QMAKE_LIBDIR_QT at the beginning of the
        # linker path, so that the installed libraries will be
        # used instead of the local ones.

        error( "local build will conflict with $${QMAKE_LIBDIR_QT}/libmodbus4qt.*" )
    }
}

#OTHER_FILES += \
#    ../doc/modbus-ap-v1_1.dox \
#    ../doc/modbus4qt.dox
#

