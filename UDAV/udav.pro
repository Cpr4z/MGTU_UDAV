#-------------------------------------------------
#
# udav system
# Copyright (C) 2014-2021
# Author Leonid Kolesnik l.kolesnik@m-i.ru
#
#-------------------------------------------------
include (udav_config.pri)

TEMPLATE = subdirs

SUBDIRS = \
    core \
    driver \
    drivers \
    elvactech-lab \
    gui \
    udav

core.depends = elvactech-lab
driver.depends = core elvactech-lab
drivers.depends = core driver elvactech-lab
gui.depends = driver
udav.depends = core driver drivers elvactech-lab gui

#contains(UDAV_CONFIG, udav_combo) {
#    SUBDIRS += udavc
#    udavc.depends = core driver drivers gui
#}
#else {
#    SUBDIRS += udavc udavs
#    udavc.depends = core gui
#    udavs.depends = core driver drivers
#}

udav_spec.files  = udav_config.pri udav.prf
udav_spec.path  = $${UDAV_INSTALL_FEATURES}

INSTALLS += udav_spec
