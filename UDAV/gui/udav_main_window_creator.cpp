#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QVBoxLayout>

#include "core/udav_settings.h"

#include "gui.h"

#include "equipment/equipment_control_panel.h"

#include "elvactools/ui/grid_tool_button_widget.h"

#include "udav_main_window.h"
#include "udav_main_window_creator.h"


namespace udav_gui
{

//-----------------------------------------------------------------------------

UdavMainWindowCreator::UdavMainWindowCreator(Core::UdavSettings* settings, elvactools::Logger* logger) :
    QObject(),
    settings_(settings),
    logger_(logger)
{
    ELVACTOOLS_LOGGER_SYSTEM_CONNECT(this, logger_)

    // First of all create log message view to catch log messages
    // and show them when window will be shown to a user
    //
    logMessagesView_ = new QPlainTextEdit();
    logMessagesView_->setReadOnly(true);
    connect(logger_, SIGNAL(messageLogged(QString)), logMessagesView_, SLOT(insertPlainText(QString)));
}


//-----------------------------------------------------------------------------

void
UdavMainWindowCreator::createActions_()
{
    QList<QAction*> actions;

    aboutQtAction_ = new QAction(tr("About &Qt"));
    aboutQtAction_->setShortcut(tr("Ctrl+Shift+F1"));
    aboutQtAction_->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction_, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    actions.append(aboutQtAction_);

    connectToServerAction_ = new QAction(tr("&Connect..."));
    connectToServerAction_->setStatusTip(tr("Connect to equipment"));
    connectToServerAction_->setIcon(getConnectIcon());
    connect(connectToServerAction_, SIGNAL(triggered()), mainWindow_, SLOT(connectToServer_()));
    actions.append(connectToServerAction_);

    disconnectFromServerAction_ = new QAction(tr("&Disconnect..."));
    disconnectFromServerAction_->setStatusTip(tr("Disconnect from equipment"));
    disconnectFromServerAction_->setIcon(getDisconnectIcon());
    connect(disconnectFromServerAction_, SIGNAL(triggered()), mainWindow_, SLOT(disconnectFromServer_()));
    actions.append(disconnectFromServerAction_);

    reloadIfaceAction_ = new QAction(tr("&Reload interface"));
    reloadIfaceAction_->setStatusTip((tr("Reload user interface from equipment")));
    reloadIfaceAction_->setIcon(getReloadIcon());
    //connect(reloadIfaceAction_, SIGNAL(triggered()), client_, SLOT(sendInterfaceReloadRequest()));
    actions.append(reloadIfaceAction_);

    editSettingsAction_ = new QAction(tr("&Settings..."));
    editSettingsAction_->setStatusTip(tr("Edit settings"));
    editSettingsAction_->setIcon(getSettingsIcon());
    connect(editSettingsAction_, SIGNAL(triggered()), mainWindow_, SLOT(editSettings_()));
    actions.append(editSettingsAction_);

    exitAction_ = new QAction(tr("E&xit..."));
    exitAction_->setShortcut(tr("Ctrl+Q"));
    exitAction_->setStatusTip(tr("Exit the application"));
    exitAction_->setIcon(getExitIcon());
    connect(exitAction_, SIGNAL(triggered()), mainWindow_, SLOT(close()));
    actions.append(exitAction_);

    fullScreenAction_ = new QAction(tr("Full screen"));
    fullScreenAction_->setShortcut(tr("F11"));
    fullScreenAction_->setStatusTip(tr("Switch full screen mode"));
    fullScreenAction_->setIcon(getFullScreenIcon());
    connect(fullScreenAction_, SIGNAL(triggered()), mainWindow_, SLOT(switchFullScreen()));
    actions.append(fullScreenAction_);

    newProtocolAction_ = new QAction(tr("New protocol"));
    newProtocolAction_->setStatusTip(tr("Create new protocol"));
    newProtocolAction_->setIcon(udav_gui::getNewProtocolIcon());
    //newProtocolAction_->setEnabled(clientSettings->writeProtocol());
    //connect(newProtocolAction_, SIGNAL(triggered()), client_, SLOT(createNewProtocol()));
    actions.append(newProtocolAction_);

    saveProtocolAction_ = new QAction(tr("Save protocol..."));
    saveProtocolAction_->setStatusTip(tr("Save protocol to external storage..."));
    saveProtocolAction_->setIcon(getSaveProtocolIcon());
    connect(saveProtocolAction_, SIGNAL(triggered()), mainWindow_, SLOT(saveProtocolToDisk_()));
    actions.append(saveProtocolAction_);

    swicth2ProgramControlPanelAction_ = new QAction(tr("Control panel"));
    swicth2ProgramControlPanelAction_->setShortcut(tr("F12"));
    swicth2ProgramControlPanelAction_->setStatusTip(tr("Switch to program control panel..."));
    connect(swicth2ProgramControlPanelAction_, SIGNAL(triggered()), mainWindow_, SLOT(swicth2ProgramControlPanel_()));
    actions.append(swicth2ProgramControlPanelAction_);

    mainWindow_->addActions(actions);
}

//-----------------------------------------------------------------------------

UdavMainWindow*
UdavMainWindowCreator::createMainWindow()
{
    mainWindow_ = new UdavMainWindow(settings_, logger_);

    createActions_();
    createMenus_();
    mainWindow_->statusBar();

    createCentralWidget_();

    mainWindow_->setMinimumSize(800, 600);
    mainWindow_->readGeometry_();

    //mainWindow_->setWindowIcon(":/images/icon.png");

    return mainWindow_;
}

//-----------------------------------------------------------------------------

void
UdavMainWindowCreator::createMenus_()
{
    QMenu* fileMenu_ = mainWindow_->menuBar()->addMenu(tr("&File"));
    fileMenu_->addAction(connectToServerAction_);
    fileMenu_->addAction(disconnectFromServerAction_);
    fileMenu_->addSeparator();
    fileMenu_->addAction(exitAction_);

    QMenu* editMenu_ = mainWindow_->menuBar()->addMenu(tr("&Edit"));
    editMenu_->addAction(editSettingsAction_);

    QMenu* viewMenu_ = mainWindow_->menuBar()->addMenu(tr("&View"));
    viewMenu_->addAction(fullScreenAction_);

    QMenu* protocolMenu_ = mainWindow_->menuBar()->addMenu(tr("&Protocol"));
    protocolMenu_->addAction(newProtocolAction_);
    protocolMenu_->addAction(saveProtocolAction_);

    QMenu* helpMenu_ = mainWindow_->menuBar()->addMenu(tr("&Help"));
    helpMenu_->addAction(aboutQtAction_);
}

//-----------------------------------------------------------------------------

void
UdavMainWindowCreator::createCentralWidget_()
{
    int buttonSize = settings_->toolBarButtonSize();
    bool showText = settings_->showToolBarText();

    GridToolButtonWidget* buttonsWidget_ = new GridToolButtonWidget(buttonSize, 15, showText);

    buttonsWidget_->addAction(1, connectToServerAction_);
    //buttonsWidget_->addAction(2, disconnectFromServerAction_);
    //buttonsWidget_->addAction(3, reloadIfaceAction_);

    buttonsWidget_->addAction(4, fullScreenAction_);

    //buttonsWidget_->addAction(5, newProtocolAction_);
    //buttonsWidget_->addAction(6, saveProtocolAction_);

    buttonsWidget_->addAction(7, editSettingsAction_);
    buttonsWidget_->addAction(8, exitAction_);

    QVBoxLayout* pageLayout = new QVBoxLayout();
    pageLayout->addWidget(buttonsWidget_);
    pageLayout->addWidget(logMessagesView_);

    QWidget* tabPage = new QWidget();
    tabPage->setLayout(pageLayout);

    mainWindow_->centralTabWidget_ = new QTabWidget();
    mainWindow_->centralTabWidget_->setTabPosition(QTabWidget::North);
    mainWindow_->centralTabWidget_->setTabShape(QTabWidget::Rounded);
    mainWindow_->centralTabWidget_->addTab(tabPage, tr("Application"));

    mainWindow_->setCentralWidget(mainWindow_->centralTabWidget_);
}

//-----------------------------------------------------------------------------

} // namespace udav_gui
