UDAV_ROOT = $${PWD}/..
include( $${UDAV_ROOT}/udav_config.pri)
include( $${UDAV_ROOT}/udav_build.pri)

QT += core gui widgets

CONFIG += c++17

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

INCLUDEPATH += $${UDAV_ROOT}/udavc
INCLUDEPATH += $${UDAV_ROOT}/core
INCLUDEPATH += $${UDAV_ROOT}/driver
INCLUDEPATH += $${UDAV_ROOT}


LIBS += -ludavcore -ludavdriver -ludavgui

include( $${UDAV_ROOT}/elvactech-lab/modbus4qt/modbus4qt_depend.pri )
include( $${UDAV_ROOT}/elvactech-lab/elvacquip/elvacquip_depend.pri )
include( $${UDAV_ROOT}/elvactech-lab/elvactools/elvactools_depend.pri )

SOURCES += \
    udav.cpp

HEADERS += \
    udav.h

TRANSLATIONS += \
    udav_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
