#include <QtGui>

#include <QPlainTextEdit>
#include <QTabWidget>

#include <QMenuBar>
#include <QMessageBox>

#include "udavc.h"
#include "elvactools/utils.h"

#include "client_main_window.h"
#include "client_settings_dialog.h"
#include "message_window.h"
#include "save_protocol_dialog.h"
#include "grid_tool_button_widget.h"

#include "equipment/equipment_control_panel.h"

namespace udav_gui
{

//-----------------------------------------------------------------------------

ClientMainWindow::ClientMainWindow(Core::Client* client, QWidget *parent) :
    QMainWindow(parent),
    client_(client),
    fullScreen_(false)
{
    setObjectName("MainWindow");

    //----------------------------------------
    // (BOF) Стандартные интерфейсные элементы

    createActions_();
    createMenus_();
    statusBar();

    // (EOF) Стандартные интерфейсные элементы
    //----------------------------------------

    //----------------------------------------
    // (BOF) Создаем интерфейсные элементы и настраиваем внешний вид

    //>- Панель управления программой

    createToolsWidget_();

    //<- Панель управления программой

    //>- Панель управления оборудованием

    equipmentControlPanel_ = new EquipmentControlPanel(client);

    // Подключаем систему вывода сообщений
    //! @todo В панель управления!
    connect(equipmentControlPanel_, SIGNAL(infoMessage(QString)), clientLogger, SLOT(writeInfoMessage(QString)));
    connect(equipmentControlPanel_, SIGNAL(criticalMessage(QString)), clientLogger, SLOT(writeCriticalMessage(QString)));
    connect(equipmentControlPanel_, SIGNAL(debugMessage(QString)), clientLogger, SLOT(writeDebugMessage(QString)));

    //<--- Панель управления оборудованием

    //>- Окно вывода сообщений

    logMessagesView_ = new QPlainTextEdit();

    //<- Окно вывода сообщений

    // Размещаем все это в окне
    centralTabWidget_ = new QTabWidget();
    centralTabWidget_->setTabPosition(QTabWidget::East);
    centralTabWidget_->addTab(toolsWidget_, tr("Program control panel"));
    centralTabWidget_->addTab(logMessagesView_, tr("Program logging"));

    setCentralWidget(centralTabWidget_);

    // (EOF) Создаем интерфейсные элементы и настраиваем внешний вид
    //----------------------------------------

    //---------------------------------------
    // (BOF) Устанавливаем размеры и положение окна

    setMinimumSize(800,	600);
    readGeometry_();

    // (EOF) Устанавливаем размеры и положение окна
    //---------------------------------------

    //setWindowIcon(":/images/icon.png");

    //----------------------------------------
    // (BOF) Связываем сигналы и слоты с объкетом клиента

    connect(client_, SIGNAL(connectionEstablished(QHostAddress)), this, SLOT(connectedToServer_(QHostAddress)));
    connect(client_, SIGNAL(connectionClosed()), this, SLOT(disconnectedFromServer_()));

    connect(client_, SIGNAL(openMessageWindow(QUuid,QString)), this, SLOT(openMessageWindow(QUuid,QString)));
    connect(client_, SIGNAL(writeTextMessage2Window(QUuid,quint16,QString)), this, SLOT(writeTextMessage2Window(QUuid,quint16,QString)));
    connect(client_, SIGNAL(closeMessageWindow(QUuid,quint16,bool)), this, SLOT(closeMessageWindow(QUuid,quint16,bool)));

    // (EOF) Связываем сигналы и слоты
    //----------------------------------------

    disconnectedFromServer_(); // Устанавливаем заголовок окна. По умолчанию - отключен от сервера

    if (clientSettings->autoConnect()) connectToServer_();
}

//-----------------------------------------------------------------------------

void
ClientMainWindow::closeEvent(QCloseEvent *event)
{
    if (disconnectFromServer_())
    {
        delete equipmentControlPanel_;

        writeGeometry_();
        event->accept();
    }
    else event->ignore();
}


//-----------------------------------------------------------------------------

void
ClientMainWindow::closeMessageWindow(const QUuid &uuid, quint16 delay, bool autoClose)
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
ClientMainWindow::connectToServer_()
{
    emit infoMessage(tr("Connection to server will be established!"));

    client_->setConnectionMode(Core::ConnectionType::WithoutTicket);
    client_->connectToServer();
}

//-----------------------------------------------------------------------------

void
ClientMainWindow::connectedToServer_(const QHostAddress& address)
{
    setWindowTitle(tr("%1 :: Connected to %2").arg(ClientUserFriendlyName).arg(address.toString()));

    centralTabWidget_->insertTab(1, equipmentControlPanel_, tr("Equipment control panel"));
    centralTabWidget_->setCurrentWidget(equipmentControlPanel_);
}

//-----------------------------------------------------------------------------

void
ClientMainWindow::createActions_()
{
    QList<QAction*> actions;

    aboutQtAction_ = new QAction(tr("About &Qt"), this);
    aboutQtAction_->setShortcut(tr("Ctrl+Shift+F1"));
    aboutQtAction_->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction_, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    actions.append(aboutQtAction_);

    connectToServerAction_ = new QAction(tr("&Connect..."), this);
    connectToServerAction_->setStatusTip(tr("Connect to server"));
    connectToServerAction_->setIcon(QIcon(":/resource/gui/icons/128x128/connected.png"));
    connect(connectToServerAction_, SIGNAL(triggered()), this, SLOT(connectToServer_()));
    actions.append(connectToServerAction_);

    disconnectFromServerAction_ = new QAction(tr("&Disconnect..."), this);
    disconnectFromServerAction_->setStatusTip(tr("Disconnect to server"));
    disconnectFromServerAction_->setIcon(QIcon(":/resource/gui/icons/128x128/disconnected.png"));
    connect(disconnectFromServerAction_, SIGNAL(triggered()), this, SLOT(disconnectFromServer_()));
    actions.append(disconnectFromServerAction_);

    reloadIfaceAction_ = new QAction(tr("&Reload interface"), this);
    reloadIfaceAction_->setStatusTip((tr("Reload user interface from server")));
    reloadIfaceAction_->setIcon(QIcon(":/resource/gui/icons/reload.png"));
    connect(reloadIfaceAction_, SIGNAL(triggered()), client_, SLOT(sendInterfaceReloadRequest()));
    actions.append(reloadIfaceAction_);

    editSettingsAction_ = new QAction(tr("&Settings..."), this);
    editSettingsAction_->setStatusTip(tr("Edit settings"));
    editSettingsAction_->setIcon(QIcon(":/resource/gui/icons/128x128/settings.png"));
    connect(editSettingsAction_, SIGNAL(triggered()), this, SLOT(editSettings_()));
    actions.append(editSettingsAction_);

    exitAction_ = new QAction(tr("E&xit..."), this);
    exitAction_->setShortcut(tr("Ctrl+Q"));
    exitAction_->setStatusTip(tr("Exit the application"));
    exitAction_->setIcon(QIcon(":/resource/gui/icons/128x128/logout.png"));
    connect(exitAction_, SIGNAL(triggered()), this, SLOT(close()));
    actions.append(exitAction_);

    fullScreenAction_ = new QAction(tr("Full screen"), this);
    fullScreenAction_->setShortcut(tr("F11"));
    fullScreenAction_->setStatusTip(tr("Switch full screen mode"));
    fullScreenAction_->setIcon(QIcon(":/resource/gui/icons/128x128/fullscreen.png"));
    connect(fullScreenAction_, SIGNAL(triggered()), this, SLOT(switchFullScreen()));
    actions.append(fullScreenAction_);

    newProtocolAction_ = new QAction(tr("New protocol"), this);
    newProtocolAction_->setStatusTip(tr("Create new working protocol"));
    newProtocolAction_->setIcon(QIcon(":/resource/gui/icons/128x128/filenew.png"));
    newProtocolAction_->setEnabled(clientSettings->writeProtocol());
    connect(newProtocolAction_, SIGNAL(triggered()), client_, SLOT(createNewProtocol()));
    actions.append(newProtocolAction_);

    saveProtocolAction_ = new QAction(tr("Save protocol..."), this);
    saveProtocolAction_->setStatusTip(tr("Save protocol to external storage..."));
    saveProtocolAction_->setIcon(QIcon(":/resource/gui/icons/128x128/filesave.png"));
    connect(saveProtocolAction_, SIGNAL(triggered()), this, SLOT(saveProtocolToDisk_()));
    actions.append(saveProtocolAction_);

    swicth2ProgramControlPanelAction_ = new QAction(tr("Control panel"), this);
    swicth2ProgramControlPanelAction_->setShortcut(tr("F12"));
    swicth2ProgramControlPanelAction_->setStatusTip(tr("Switch to program control panel..."));
    connect(swicth2ProgramControlPanelAction_, SIGNAL(triggered()), this, SLOT(swicth2ProgramControlPanel_()));
    actions.append(swicth2ProgramControlPanelAction_);

    addActions(actions);
}

//-----------------------------------------------------------------------------

void
ClientMainWindow::createMenus_()
{
    fileMenu_ = menuBar()->addMenu(tr("&File"));
    fileMenu_->addAction(connectToServerAction_);
    fileMenu_->addAction(disconnectFromServerAction_);
    fileMenu_->addSeparator();
    fileMenu_->addAction(exitAction_);

    editMenu_ = menuBar()->addMenu(tr("&Edit"));
    editMenu_->addAction(editSettingsAction_);

    viewMenu_ = menuBar()->addMenu(tr("&View"));
    viewMenu_->addAction(fullScreenAction_);

    protocolMenu_ = menuBar()->addMenu(tr("&Protocol"));
    protocolMenu_->addAction(newProtocolAction_);
    protocolMenu_->addAction(saveProtocolAction_);

    helpMenu_ = menuBar()->addMenu(tr("&Help"));
    helpMenu_->addAction(aboutQtAction_);
}

//-----------------------------------------------------------------------------

void
ClientMainWindow::createToolsWidget_()
{
    int buttonSize = clientSettings->toolBarButtonSize();
    bool showText = clientSettings->showToolBarText();

    toolsWidget_ = new GridToolButtonWidget(this, buttonSize, 15, showText);

    toolsWidget_->addAction(connectToServerAction_);
    toolsWidget_->addAction(disconnectFromServerAction_);
    toolsWidget_->addAction(reloadIfaceAction_);

    toolsWidget_->addAction(fullScreenAction_);

    toolsWidget_->addAction(newProtocolAction_);
    toolsWidget_->addAction(saveProtocolAction_);

    toolsWidget_->addAction(editSettingsAction_);
    toolsWidget_->addAction(exitAction_);
}

//-----------------------------------------------------------------------------

void
ClientMainWindow::deleteMessageWindow_(QWidget *window)
{
    //! @todo В панель управления!

    QUuid key = messageWindows_.key((MessageWindow*)window);

    if (!key.isNull()) messageWindows_.remove(key);

    delete window;
}

//-----------------------------------------------------------------------------

bool
ClientMainWindow::disconnectFromServer_()
{
    if (client_->isConnected())
    {
        int res = QMessageBox::question(this,
                                        tr("Question"),
                                        tr("Connection with server will be closed! Continue?"),
                                        QMessageBox::Yes | QMessageBox::No);
        if (res == QMessageBox::Yes)
        {
            emit infoMessage(tr("Conection with server will be closed!"));
            client_->closeConnection();
            return true;
        }
        else
            return false;
    }
    return true;
}

//-----------------------------------------------------------------------------

void
ClientMainWindow::disconnectedFromServer_()
{
    // Если закладок 3 штуки, значит ранее действительно было установлено подключение к серверу
    // и надо удалить закладку панели управления оборудованием
    if (centralTabWidget_->count() == 3) centralTabWidget_->removeTab(1);

    setWindowTitle(tr("%1 :: Not connected").arg(ClientUserFriendlyName));
}

//-----------------------------------------------------------------------------

void
ClientMainWindow::editSettings_()
{
    ClientSettingsDialog dialog(client_, this);
    dialog.exec();
}

//-----------------------------------------------------------------------------

void
ClientMainWindow::openMessageWindow(const QUuid &uuid, const QString &header)
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
ClientMainWindow::saveProtocolToDisk_()
{
    SaveProtocolDialog dialog(this);
    int res = dialog.exec();
    if (res == QDialog::Accepted)
    {
        QStringList fileNames = dialog.selectedFiles();
        QString source = Core::checkBackSlash(clientSettings->protocolPath());
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
                    continue;
                else
                    targetFile.remove();
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
ClientMainWindow::readGeometry_()
{
    if (clientSettings->restoreWindowsPos())
    {
        QRect geom = clientSettings->readWindowGeometry(objectName());
        if (!geom.isEmpty())
            setGeometry(geom.x(), geom.y(), geom.width(), geom.height());
    }
}

//-----------------------------------------------------------------------------

void
ClientMainWindow::swicth2ProgramControlPanel_()
{
    centralTabWidget_->setCurrentWidget(toolsWidget_);
}

//-----------------------------------------------------------------------------

void
ClientMainWindow::switchFullScreen()
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
ClientMainWindow::writeGeometry_()
{
    if (clientSettings->saveWindowsPos())
        clientSettings->writeWindowGeometry(objectName(), geometry());
}

//-----------------------------------------------------------------------------

void
ClientMainWindow::writeTextMessage2Window(const QUuid &uuid, quint16 type, const QString &message)
{
    //! @todo В панель управления!

    MessageWindow* messageWindow = messageWindows_[uuid];
    if (messageWindow) messageWindow->showMessage(type, message);
}

//-----------------------------------------------------------------------------

} // namespace Ui
