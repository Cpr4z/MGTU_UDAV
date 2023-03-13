UDAV_ROOT = $${PWD}/..
include( $${UDAV_ROOT}/udav_config.pri )
include( $${UDAV_ROOT}/udav_build.pri )

TEMPLATE = lib
TARGET = $$qtLibraryTarget(udavgui)
DESTDIR = $${UDAV_BUILD}/lib

QT += svg widgets serialport
CONFIG += QWT

INCLUDEPATH += $${UDAV_ROOT}

LIBS += -ludavdriver

include($${UDAV_ROOT}/elvactech-lab/elvactools/elvactools_depend.pri)

contains(UDAV_CONFIG, udav_dll) {
    CONFIG += dll
    DEFINES += UDAV_GUI_DLL
}
else {
    CONFIG += staticlib
}

SOURCES +=  \
    client_settings_dialog.cpp \
    equipment/cells/abstract_ui_cell.cpp \
    equipment/cells/circuit/circuit_abstract_ui_item.cpp \
    equipment/cells/circuit/circuit_abstract_widget_ui_cell.cpp \
    equipment/cells/circuit/circuit_push_button_ui_cell.cpp \
    equipment/cells/circuit/leak_button_ui_cell.cpp \
    equipment/cells/circuit/pipe_button_ui_cell.cpp \
    equipment/cells/circuit/pump_button_ui_cell.cpp \
    equipment/cells/circuit/turbo_pump_button.cpp \
    equipment/cells/circuit/vaccum_chamber_button.cpp \
    equipment/cells/circuit/vaccum_sensor_button_ui_cell.cpp \
    equipment/cells/circuit/vent_button_ui_cell.cpp \
    equipment/cells/group_ui_cell.cpp \
    equipment/cells/image_ui_item.cpp \
    equipment/cells/knob_ui_item.cpp \
    equipment/cells/lcd_ui_cell.cpp \
    equipment/cells/led_ui_item.cpp \
    equipment/cells/push_button_ui_cell.cpp \
    equipment/cells/svg_image_ui_item.cpp \
    equipment/cells/text_message_ui_item.cpp \
    equipment/cells/value_edit_ui_cell.cpp \
    equipment/driver_settings_model.cpp \
    equipment/equipment_config_widget.cpp \
    equipment/equipment_control_page.cpp \
    equipment/equipment_control_panel.cpp \
    equipment/equipment_debug_widget.cpp \
    equipment/equipment_help_widget.cpp \
    equipment/equipment_ui_reader.cpp \
    gui.cpp \
    message_window.cpp \
    save_protocol_dialog.cpp \
    select_driver_dialog.cpp \
    udav_main_window.cpp \
    udav_main_window_creator.cpp

HEADERS += \
    client_settings_dialog.h \
    equipment/cells/abstract_ui_cell.h \
    equipment/cells/circuit/circuit_abstract_ui_item.h \
    equipment/cells/circuit/circuit_abstract_widget_ui_cell.h \
    equipment/cells/circuit/circuit_push_button_ui_cell.h \
    equipment/cells/circuit/leak_button_ui_cell.h \
    equipment/cells/circuit/pipe_button_ui_cell.h \
    equipment/cells/circuit/pump_button_ui_cell.h \
    equipment/cells/circuit/turbo_pump_button.h \
    equipment/cells/circuit/vaccum_chamber_button.h \
    equipment/cells/circuit/vaccum_sensor_button_ui_cell.h \
    equipment/cells/circuit/vent_button_ui_cell.h \
    equipment/cells/group_ui_cell.h \
    equipment/cells/image_ui_item.h \
    equipment/cells/knob_ui_item.h \
    equipment/cells/lcd_ui_cell.h \
    equipment/cells/led_ui_item.h \
    equipment/cells/push_button_ui_cell.h \
    equipment/cells/svg_image_ui_item.h \
    equipment/cells/text_message_ui_item.h \
    equipment/cells/value_edit_ui_cell.h \
    equipment/driver_settings_model.h \
    equipment/equipment_config_widget.h \
    equipment/equipment_control_page.h \
    equipment/equipment_control_panel.h \
    equipment/equipment_debug_widget.h \
    equipment/equipment_help_widget.h \
    equipment/equipment_ui_reader.h \
    gui.h \
    message_window.h \
    save_protocol_dialog.h \
    select_driver_dialog.h \
    udav_main_window.h \
    udav_main_window_creator.h

TRANSLATIONS = udav_gui_ru.ts

#------------------------------------------------------
# Automatic translation compilation

#tr.commands = lrelease $$_PRO_FILE_
#QMAKE_EXTRA_TARGETS += tr
#PRE_TARGETDEPS += tr

#------------------------------------------------------
# Install directives

target.path = $${UDAV_INSTALL_LIBS}

doc.files = $${UDAV_ROOT}/doc/html
unix:doc.files += $${UDAV_ROOT}/doc/man
doc.path = $${UDAV_DOCS}

INSTALLS = target doc

headers.files  = $${HEADERS}
headers.path   = $${UDAV_INSTALL_HEADERS}

RESOURCES += \
    gui.qrc

FORMS += \
    client_settings_dialog.ui \
    save_protocol_dialog.ui \
    select_driver_dialog.ui
