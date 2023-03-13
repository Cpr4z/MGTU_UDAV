TEMPLATE = subdirs

SUBDIRS += \
    elvacquip \
    elvactools \
    modbus4qt

elvacquip.depends = elvactools modbus4qt