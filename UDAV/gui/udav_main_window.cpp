#include <QtGui>

#include <QPlainTextEdit>
#include <QTabWidget>

#include <QMenuBar>
#include <QMessageBox>

//#include "udavc.h"
#include "elvactools/utils.h"
#include "core/udav_settings.h"
#include "gui/gui.h"

#include "udav_main_window.h"
#include "select_driver_dialog.h"

#include "client_settings_dialog.h"
#include "message_window.h"
#include "save_protocol_dialog.h"
#include "elvactools/ui/grid_tool_button_widget.h"

#include "equipment/equipment_control_panel.h"

#include "driver/drivers_manager.h"
#include "driver/driver_impl.h"

namespace udav_gui
{

//-----------------------------------------------------------------------------

UdavMainWindow::UdavMainWindow(Core::UdavSettings* settings, elvactools::Logger* logger) :
    QMainWindow(),
    settings_(settings),
    logger_(logger),
    fullScreen_(false)
{
    ELVACTOOLS_LOGGER_SYSTEM_CONNECT(this, logger)

    setObjectName("UdavMainWindow");

    connect(this, SIGNAL(connectedToEquipment(udav_drivers::DriverImpl*)),
            this, SLOT(onConnectedToEquipment_(udav_drivers::DriverImpl*)));

//    connect(client_, SIGNAL(connectionClosed()), this, SLOT(disconnectedFromServer_()));

//    connect(client_, SIGNAL(openMessageWindow(QUuid,QString)), this, SLOT(openMessageWindow(QUuid,QString)));
//    connect(client_, SIGNAL(writeTextMessage2Window(QUuid,quint16,QString)), this, SLOT(writeTextMessage2Window(QUuid,quint16,QString)));
//    connect(client_, SIGNAL(closeMessageWindow(QUuid,quint16,bool)), this, SLOT(closeMessageWindow(QUuid,quint16,bool)));

    driversManager_ = new udav_drivers::DriversManager(logger, this);

//    disconnectedFromServer_(); // Устанавливаем заголовок окна. По умолчанию - отключен от сервера

//    if (clientSettings->autoConnect()) connectToServer_();
}

//-----------------------------------------------------------------------------

void
UdavMainWindow::closeEvent(QCloseEvent *event)
{
    emit infoMessage(tr("User inited exiting from application..."));

    if (disconnectFromServer_())
    {
////        delete equipmentControlPanel_;

        writeGeometry_();

        event->accept();
    }
    else
    {
        event->ignore();
    }
}


//-----------------------------------------------------------------------------

void
UdavMainWindow::closeMessageWindow(const QUuid &uuid, quint16 delay, bool autoClose)
{
    emit debugMessage(tr("Window with id=%1 will be closed or allowed to close in %2 s").arg(uuid.toString()).arg(delay));

    MessageWindow* messageWindow = messageWindows_[uuid];

    if (messageWindow)
    {
        messageWindow->closeWindow(delay, autoClose);
    }
}

//-----------------------------------------------------------------------------

void
UdavMainWindow::connectToServer_()
{
    emit infoMessage(tr("Connection to server will be established!"));

    const auto driversList = driversManager_->availableDriversList();

    udav_gui::SelectDriverDialog dialog(driversList);

    if (dialog.exec())
    {
        QUuid selUuid = dialog.getSelection();
        for (const auto& item : driversList)
        {
            if (item["uuid"].toString() == selUuid.toString())
            {
                udav_drivers::DriverImpl* driverImpl = driversManager_->createDriverImpl(item["uuid"].toString());

                if (driverImpl)
                {
                    connectedEquipment_.append(driverImpl);

                    // Read and load driver settings
                    // Start driver

                    emit connectedToEquipment(driverImpl);
                }
                else
                {
                    QMessageBox::critical(this,
                                          tr("Error"),
                                          tr("Cannot create driver implementation for %1").
                                          arg(item["name"].toString()));
                }

                break;
            }
        }
    }
}

//-----------------------------------------------------------------------------

void
UdavMainWindow::deleteMessageWindow_(QWidget *window)
{
    //! @todo В панель управления!

    QUuid key = messageWindows_.key((MessageWindow*)window);

    if (!key.isNull())
    {
        messageWindows_.remove(key);
    }

    delete window;
}

//-----------------------------------------------------------------------------

bool
UdavMainWindow::disconnectFromServer_()
{
//    if (client_->isConnected())
//    {
//        int res = QMessageBox::question(this,
//                                        tr("Question"),
//                                        tr("Connection with server will be closed! Continue?"),
//                                        QMessageBox::Yes | QMessageBox::No);
//        if (res == QMessageBox::Yes)
//        {
//            emit infoMessage(tr("Conection with server will be closed!"));
//            client_->closeConnection();
//            return true;
//        }
//        else
//        {
//            return false;
//        }
//    }
    return true;
}

//-----------------------------------------------------------------------------

void
UdavMainWindow::disconnectedFromServer_()
{
    // Если закладок 3 штуки, значит ранее действительно было установлено подключение к серверу
    // и надо удалить закладку панели управления оборудованием
    if (centralTabWidget_->count() == 3) centralTabWidget_->removeTab(1);

    setWindowTitle(tr("%1 :: Not connected").arg("ClientUserFriendlyName"));
}

//-----------------------------------------------------------------------------

void
UdavMainWindow::editSettings_()
{
    ClientSettingsDialog dialog(settings_, this);
    dialog.exec();
}

//-----------------------------------------------------------------------------

void
UdavMainWindow::onConnectedToEquipment_(udav_drivers::DriverImpl* driverImpl)
{
    EquipmentControlPanel* driverControlPanel = new EquipmentControlPanel(driverImpl, settings_, logger_);
    centralTabWidget_->addTab(driverControlPanel, driverImpl->name());
    centralTabWidget_->setCurrentWidget(driverControlPanel);
}

//-----------------------------------------------------------------------------

void
UdavMainWindow::openMessageWindow(const QUuid &uuid, const QString &header)
{

    //! @todo В панель управления!
    emit infoMessage(tr("Window with title \'%1\' and uuid=%2 will be open").arg(header).arg(uuid.toString()));

    MessageWindow* messageWindow = new MessageWindow();

    connect(messageWindow, SIGNAL(windowClosed(QWidget*)), this, SLOT(deleteMessageWindow_(QWidget*)));

    messageWindows_.insert(uuid, messageWindow);

    messageWindow->setWindowTitle(header);
    messageWindow->show();
}

//-----------------------------------------------------------------------------

void
UdavMainWindow::saveProtocolToDisk_()
{
    SaveProtocolDialog dialog(settings_, this);
    int res = dialog.exec();
    if (res == QDialog::Accepted)
    {
        QStringList fileNames = dialog.selectedFiles();
        QString source = Core::checkBackSlash(settings_->protocolPath());
        QString target = Core::checkBackSlash(dialog.target());

        int count = 0;
        foreach (QString fileName, fileNames)
        {
            QFile targetFile(target + fileName);
            if (targetFile.exists())
            {
                int res = QMessageBox::question(this, tr("Question"), tr("File %1 exists! Overwrite?").
                                                arg(target + fileName),
                                                QMessageBox::Yes | QMessageBox::No);
                if (res != QMessageBox::Yes)
                {
                    continue;
                }
                else
                {
                    targetFile.remove();
                }
            }

            QFile file(source + fileName);
            if (!file.copy(target + fileName))
            {
                QMessageBox::critical(this, tr("Error"), tr("Cannot copy file %1!\nSource path: %2\n Target path: %3").
                                      arg(fileName).arg(source).arg(target));
            }
            else
            {
                count += 1;
                file.flush();
            }
        }
        if (count) QMessageBox::information(this, tr("Information"), tr("%1 file(s) copied.").arg(count));
    }
}

//-----------------------------------------------------------------------------

void
UdavMainWindow::readGeometry_()
{
    if (settings_->restoreWindowsPos())
    {
        QRect geom = settings_->readWindowGeometry(objectName());
        if (!geom.isEmpty())
        {
            setGeometry(geom.x(), geom.y(), geom.width(), geom.height());
        }
    }
}

//-----------------------------------------------------------------------------

void
UdavMainWindow::swicth2ProgramControlPanel_()
{
    //centralTabWidget_->setCurrentWidget(toolsWidget_);
}

//-----------------------------------------------------------------------------

void
UdavMainWindow::switchFullScreen()
{
    if (fullScreen_)
    {
        showNormal();
        fullScreen_ = false;
    }
    else
    {
        showFullScreen();
        menuBar()->setVisible(false);
        fullScreen_ = true;
    }
}

//-----------------------------------------------------------------------------

void
UdavMainWindow::writeGeometry_()
{
    if (settings_->saveWindowsPos())
    {
        settings_->writeWindowGeometry(objectName(), geometry());
    }
}

//-----------------------------------------------------------------------------

void
UdavMainWindow::writeTextMessage2Window(const QUuid &uuid, quint16 type, const QString &message)
{
    //! @todo В панель управления!

    MessageWindow* messageWindow = messageWindows_[uuid];
    if (messageWindow)
    {
        messageWindow->showMessage(type, message);
    }
}

//-----------------------------------------------------------------------------

} // namespace Ui
