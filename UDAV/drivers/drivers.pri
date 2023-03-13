UDAV_ROOT 	= $${PWD}/..
include($${UDAV_ROOT}/udav_config.pri)
include($${UDAV_ROOT}/udav_build.pri)

include($${UDAV_ROOT}/elvactech-lab/elvactools/elvactools_depend.pri)

TEMPLATE 	= lib
CONFIG 		+= plugin

DESTDIR		= $${UDAV_BUILD}/bin/drivers

INCLUDEPATH += \
    $${UDAV_ROOT} \
    $${UDAV_ROOT}/core \
    $${UDAV_ROOT}/driver

LIBS += -ludavcore -ludavdriver
