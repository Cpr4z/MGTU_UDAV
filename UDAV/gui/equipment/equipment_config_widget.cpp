#include "equipment_config_widget.h"
#include "driver_settings_model.h"

#include <QtGui>

#include <QTableView>
#include <QHBoxLayout>

namespace udav_gui
{

EquipmentConfigWidget::EquipmentConfigWidget(QWidget *parent)
    : QWidget(parent)
{
    QTableView* driverSettingView = new QTableView();
    DriverSettingsModel* driverSettingModel = new DriverSettingsModel();
    driverSettingView->setModel(driverSettingModel);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(driverSettingView);

    setLayout(mainLayout);
}

}

