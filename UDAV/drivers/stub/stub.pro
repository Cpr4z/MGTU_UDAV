include(../drivers.pri)

QT -= gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += serialport
    CONFIG += modbus4qt
}
else {
    CONFIG += serialport modbus4qt
}

TARGET = stub

unix: {
    CONFIG(debug, debug|release) {
        LIBS += -lelvacquip -lmodbus4qt
    }
    else {
        LIBS += -lelvacquip -lmodbus4qt
    }
}
else {
    LIBS += qtAddLibrary(elvaquip)
}

HEADERS = \
    stub.h \
    stub_impl.h

SOURCES = \
    stub.cpp \
    stub_impl.cpp

include($${PWD}/../../elvactech-lab/elvactools/elvactools_depend.pri)
