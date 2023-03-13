#include "client_settings_dialog.h"

//#include "../udavc.h"
#include "elvactools/utils.h"

#include "core/udav_settings.h"

#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

namespace udav_gui
{

//-----------------------------------------------------------------------------

ClientSettingsDialog::ClientSettingsDialog(Core::UdavSettings* settings, /*Core::Client* client, */QWidget *parent) :
    QDialog(parent),
    settings_(settings)
    //client_(client)
{
    setupUi(this);
    readSettings();
    enableUiItems_();
}

//-----------------------------------------------------------------------------

void
ClientSettingsDialog::enableUiItems_()
{
    on_autoConnectBox_stateChanged(autoConnectBox->isChecked());
    on_writeProtocolCheckBox_stateChanged(writeProtocolCheckBox->isChecked());
}

//-----------------------------------------------------------------------------

void
ClientSettingsDialog::fillDriversListComboBox(const QList<Core::UniversalNamedMap>& driversList)
{
    while (driversListCombo->count())
        driversListCombo->removeItem(0);

    Core::UniversalNamedMap item;
    foreach(item, driversList)
        driversListCombo->addItem(item["desc"].toString(), item["uuid"]);

    disconnect(this, SLOT(fillDriversListComboBox(QList<Core::UniversalNamedMap>)));
}

//-----------------------------------------------------------------------------

void
ClientSettingsDialog::on_buttonBox_accepted()
{
    writeSettings();
    accept();
}

//-----------------------------------------------------------------------------

void ClientSettingsDialog::on_buttonBox_clicked(QAbstractButton * button)
{
    QDialogButtonBox::StandardButton stdButton = buttonBox->standardButton(button);
    if (stdButton == QDialogButtonBox::Apply)
    {
        writeSettings();
    }
}

//-----------------------------------------------------------------------------

void
ClientSettingsDialog::on_clearCacheButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Do you really want to clear cache?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::No);

    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes)
    {
        QString tempPath = Core::getTempPath();

        QDir dir;
        QString msg;
        bool ok = dir.rmpath(tempPath);
        if (!ok)
        {
            msg = tr("Cannot clear cache directory!");
            QMessageBox::critical(this, tr("Error"), msg);
            emit infoMessage(msg);
        }
        else
        {
            msg = tr("Cache has been cleared!");
            QMessageBox::information(this, tr("Information"), msg);
            emit infoMessage(msg);
        }
    }
}

//-----------------------------------------------------------------------------

void
ClientSettingsDialog::on_autoConnectBox_stateChanged(int state)
{
    if (state)
    {
        driversListCombo->setEnabled(true);
        loadDriversButton->setEnabled(true);
    }
    else
    {
        driversListCombo->setEnabled(false);
        loadDriversButton->setEnabled(false);

        while (driversListCombo->count())
            driversListCombo->removeItem(0);
    }

}

//-----------------------------------------------------------------------------

void
ClientSettingsDialog::on_loadDriversButton_clicked()
{
//    connect(client_, SIGNAL(driversListReaded(QList<Core::UniversalNamedMap>)), this, SLOT(fillDriversListComboBox(QList<Core::UniversalNamedMap>)));

//    client_->setConnectionMode(Core::ConnectionType::DriversListRequest);
//    client_->connectToServer();
}

//-----------------------------------------------------------------------------

void
ClientSettingsDialog::on_protocolPathSelectButton_clicked()
{
    QFileDialog dialog(this, tr("Select path for saving protcols"), QString("~/"));
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    if (dialog.exec())
        protocolPathEdit->setText(dialog.directory().absolutePath());
}

//-----------------------------------------------------------------------------

void
ClientSettingsDialog::on_protocolSaveToBrowseButton_clicked()
{
    QFileDialog dialog(this, tr("Select default path for external device"), QString("/media"));
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    if (dialog.exec())
        protocolSaveToPathEdit->setText(dialog.directory().absolutePath());
}

//-----------------------------------------------------------------------------

void
ClientSettingsDialog::on_writeProtocolCheckBox_stateChanged(int state)
{
    bool enable = false;
    if (state) enable = true;

    protocolPathEdit->setEnabled(enable);
    protocolPathSelectButton->setEnabled(enable);
    protocolTimerSpinBox->setEnabled(enable);
    deleteProtocolFilesAfterSpinBox->setEnabled(enable);
    protocolSaveToPathEdit->setEnabled(enable);
    protocolSaveToBrowseButton->setEnabled(enable);
}

//-----------------------------------------------------------------------------

void
ClientSettingsDialog::readSettings()
{
//    cacheFilesBox->setChecked(settings_->cacheFiles());
//    serverIPLineEdit->setText(settings_->serverAddress().toString());
//    serverPortSpinBox->setValue(settings_->serverPort());

    autoConnectBox->setChecked(settings_->autoConnect());
    driversListCombo->setEnabled(autoConnectBox->isChecked());
    loadDriversButton->setEnabled(autoConnectBox->isChecked());

    driversListCombo->addItem(settings_->defaultDriverName(), settings_->defaultDriverUuid());

    //-------------------
    // Protocol tab (bof)

    writeProtocolCheckBox->setChecked(settings_->writeProtocol());
    protocolPathEdit->setText(settings_->protocolPath());
    protocolTimerSpinBox->setValue(settings_->protocolTimer());
    deleteProtocolFilesAfterSpinBox->setValue(settings_->deleteProtocolFilesAfterDays());
    protocolSaveToPathEdit->setText(settings_->protocolSaveToPath());

    // Protocol tab (eof)
    //-------------------

    //-------------------
    // Iterface tab (bof)

    startInFullScreenCheckBox->setChecked(settings_->startInFullScreenMode());
    saveWindowsPosCheckBox->setChecked(settings_->saveWindowsPos());
    restoreWindowsPosCheckBox->setChecked(settings_->restoreWindowsPos());
    showGridCheckBox->setChecked(settings_->showGrid());

    switch (settings_->toolBarButtonSize())
    {
        case 100 :
            toolBarButtonSizeComboBox->setCurrentIndex(0);
        break;
        case 75 :
            toolBarButtonSizeComboBox->setCurrentIndex(1);
        break;
        case 50 :
            toolBarButtonSizeComboBox->setCurrentIndex(2);
        break;
        case 25 :
            toolBarButtonSizeComboBox->setCurrentIndex(3);
        break;
        default:
            toolBarButtonSizeComboBox->setCurrentIndex(1);
    }

    showToolBarButtonsTextCheckBox->setChecked(settings_->showToolBarText());
    showDebugPageCheckBox->setChecked(settings_->showDebugPage());
    showLogMessagesPageCheckBox->setChecked(settings_->showLogMessagesPage());

    // Iterface tab (eof)
    //-------------------

    //-------------------
    // Logging tab (bof)

    clearLogsCheckBox->setChecked(settings_->clearLogAtStart());

    // Logging tab (eof)
    //-------------------

}

//-----------------------------------------------------------------------------

void
ClientSettingsDialog::writeSettings()
{
//    settings_->setCacheFiles(cacheFilesBox->isChecked());
//    settings_->setServerAddress(QHostAddress(serverIPLineEdit->text()));
//    settings_->setPortNumber(serverPortSpinBox->value());
    settings_->setAutoConnect(autoConnectBox->isChecked());

    settings_->setDefaultDriver(
                driversListCombo->currentText(),
                driversListCombo->itemData(driversListCombo->currentIndex()).toString());

    //-------------------
    // Protocol tab (bof)

    settings_->setWriteProtocol(writeProtocolCheckBox->isChecked());
    settings_->setProtocolPath(protocolPathEdit->text());
    settings_->setProtocolTimer(protocolTimerSpinBox->value());
    settings_->setDeleteProtocolFilesAfterDays(deleteProtocolFilesAfterSpinBox->value());
    settings_->setProtocolSaveToPath(protocolSaveToPathEdit->text());

    // Protocol tab (eof)
    //-------------------

    //-------------------
    // Iterface tab (bof)

    settings_->setStartInFullScreenMode(startInFullScreenCheckBox->isChecked());
    settings_->setSaveWindowsPos(saveWindowsPosCheckBox->isChecked());
    settings_->setRestoreWindowsPos(restoreWindowsPosCheckBox->isChecked());
    settings_->setShowGrid(showGridCheckBox->isChecked());

    int index = toolBarButtonSizeComboBox->currentIndex();
    int size = 100;

    switch (index)
    {
        case 0 :
            size = 100;
        break;
        case 1 :
            size = 75;
        break;
        case 2 :
            size = 50;
        break;
        case 3 :
            size = 25;
        break;
    }

    settings_->setToolBarButtonSize(size);

    settings_->setShowToolBarText(showToolBarButtonsTextCheckBox->isChecked());
    settings_->setShowDebugPage(showDebugPageCheckBox->isChecked());
    settings_->setShowLogMessagesPage(showLogMessagesPageCheckBox->isChecked());

    // Iterface tab (eof)
    //-------------------

    //-------------------
    // Logging tab (bof)

    settings_->setClearLogAtStart(clearLogsCheckBox->isChecked());

    // Logging tab (eof)
    //-------------------


    settings_->save();

    QMessageBox::warning(this, tr("Information"),
                         tr("Setting have been saved.\n"
                            "You need restart program for interface settings take effect."),
                         QMessageBox::Ok);
}

}
