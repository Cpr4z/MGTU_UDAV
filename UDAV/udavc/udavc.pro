UDAV_ROOT = $${PWD}/..
include( $${UDAV_ROOT}/udav_config.pri)
include( $${UDAV_ROOT}/udav_build.pri)

QT += core gui network network svg
CONFIG += qwt

TEMPLATE = app
TARGET = udavc

INCLUDEPATH += $${UDAV_ROOT}/udavc
INCLUDEPATH += $${UDAV_ROOT}/core
INCLUDEPATH += $${UDAV_ROOT}/driver

contains(UDAV_CONFIG, udav_combo){
    INCLUDEPATH += $${UDAV_ROOT}/udavs
}

LIBS += -ludavcore -ludavdriver

include($${UDAV_ROOT}/elvactech-lab/modbus4qt/modbus4qt_depend.pri)
include($${UDAV_ROOT}/elvactech-lab/elvacquip/elvacquip_depend.pri)
include($${UDAV_ROOT}/elvactech-lab/elvactools/elvactools_depend.pri)

SOURCES +=\
    udavc.cpp \
    client.cpp \
    client_settings.cpp \
    gui/client_main_window.cpp \
    gui/client_select_driver_dialog.cpp \
    gui/client_settings_dialog.cpp \
    gui/message_window.cpp \
    gui/save_protocol_dialog.cpp \
    gui/equipment/equipment_control_page.cpp \
    gui/equipment/equipment_control_panel.cpp \
    gui/equipment/equipment_ui_reader.cpp \
    gui/equipment/cells/abstract_ui_cell.cpp \
    gui/equipment/cells/image_ui_item.cpp \
    gui/equipment/cells/knob_ui_item.cpp \
    gui/equipment/cells/led_ui_item.cpp \
    gui/equipment/cells/push_button_ui_cell.cpp \
    gui/equipment/cells/svg_image_ui_item.cpp \
    gui/equipment/cells/text_message_ui_item.cpp \
    gui/equipment/cells/circuit/circuit_abstract_ui_item.cpp \
    gui/equipment/cells/circuit/turbo_pump_button.cpp \
    gui/equipment/cells/circuit/vaccum_chamber_button.cpp \
    gui/equipment/equipment_config_widget.cpp \
    gui/equipment/equipment_debug_widget.cpp \
    gui/equipment/equipment_help_widget.cpp \
    gui/equipment/cells/circuit/circuit_push_button_ui_cell.cpp \
    gui/equipment/cells/circuit/vent_button_ui_cell.cpp \
    gui/equipment/cells/circuit/leak_button_ui_cell.cpp \
    gui/equipment/cells/circuit/pump_button_ui_cell.cpp \
    gui/equipment/cells/circuit/pipe_button_ui_cell.cpp \
    gui/equipment/cells/circuit/vaccum_sensor_button_ui_cell.cpp \
    gui/equipment/cells/circuit/circuit_abstract_widget_ui_cell.cpp \
    gui/grid_tool_button_widget.cpp \
    gui/equipment/cells/value_edit_ui_cell.cpp \
    gui/equipment/cells/group_ui_cell.cpp \
    gui/postioned_widget.cpp \
    gui/equipment/cells/lcd_ui_cell.cpp \
    gui/numeric_pad.cpp \
    gui/equipment/driver_settings_model.cpp

HEADERS  += \
    udavc.h \
    client.h \
    client_settings.h \
    gui/client_main_window.h \
    gui/client_select_driver_dialog.h \
    gui/client_settings_dialog.h \
    gui/message_window.h \
    gui/save_protocol_dialog.h \
    gui/equipment/equipment_control_page.h \
    gui/equipment/equipment_control_panel.h \
    gui/equipment/equipment_ui_reader.h \
    gui/equipment/cells/abstract_ui_cell.h \
    gui/equipment/cells/image_ui_item.h \
    gui/equipment/cells/knob_ui_item.h \
    gui/equipment/cells/led_ui_item.h \
    gui/equipment/cells/push_button_ui_cell.h \
    gui/equipment/cells/svg_image_ui_item.h \
    gui/equipment/cells/text_message_ui_item.h \
    gui/equipment/cells/circuit/circuit_abstract_ui_item.h \
    gui/equipment/cells/circuit/turbo_pump_button.h \
    gui/equipment/cells/circuit/vaccum_chamber_button.h \
    gui/equipment/equipment_config_widget.h \
    gui/equipment/equipment_debug_widget.h \
    gui/equipment/equipment_help_widget.h \
    gui/equipment/cells/circuit/circuit_push_button_ui_cell.h \
    gui/equipment/cells/circuit/vent_button_ui_cell.h \
    gui/equipment/cells/circuit/leak_button_ui_cell.h \
    gui/equipment/cells/circuit/pump_button_ui_cell.h \
    gui/equipment/cells/circuit/pipe_button_ui_cell.h \
    gui/equipment/cells/circuit/vaccum_sensor_button_ui_cell.h \
    gui/equipment/cells/circuit/circuit_abstract_widget_ui_cell.h \
    gui/grid_tool_button_widget.h \
    gui/equipment/cells/value_edit_ui_cell.h \
    gui/equipment/cells/group_ui_cell.h \
    gui/postioned_widget.h \
    gui/equipment/cells/lcd_ui_cell.h \
    gui/numeric_pad.h \
    gui/equipment/driver_settings_model.h

FORMS += \
    gui/client_select_driver_dialog.ui \
    gui/client_settings_dialog.ui \
    gui/save_protocol_dialog.ui \
    gui/numeric_pad.ui

RESOURCES = udavc.qrc
TRANSLATIONS = udavc_ru.ts

contains(UDAV_CONFIG, udav_combo){
    QT += serialport

    INCLUDEPATH += $${UDAV_ROOT}/udavs

    SOURCES +=\
        #$${UDAV_ROOT}/udavs/udavs.cpp \
        $${UDAV_ROOT}/udavs/server.cpp \
        $${UDAV_ROOT}/udavs/server_settings.cpp \
        $${UDAV_ROOT}/udavs/server_side_socket.cpp \
        #$${UDAV_ROOT}/udavs/server_application.cpp \
        $${UDAV_ROOT}/udavs/factories.cpp

    HEADERS  += \
        #$${UDAV_ROOT}/udavs/udavs.h \
        $${UDAV_ROOT}/udavs/server.h \
        $${UDAV_ROOT}/udavs/server_settings.h \
        $${UDAV_ROOT}/udavs/server_side_socket.h \
        #$${UDAV_ROOT}/udavs/server_application.h \
        $${UDAV_ROOT}/udavs/factories.h

    TRANSLATIONS += \
        $${UDAV_ROOT}/udavs/udavs_ru.ts
}

#------------------------------------------------------------------------------
# Automatic translation compilation

tr.commands = lrelease $$_PRO_FILE_
QMAKE_EXTRA_TARGETS += tr
PRE_TARGETDEPS += tr

#-------------------------------------------------------------------------------
# Install

target.path = $${UDAV_INSTALL_BIN}
INSTALLS = target
