#include <QtWidgets/QApplication>

#include <QTranslator>
#include <QFile>
#include <QDateTime>
#include <QDesktopWidget>
#include <QRect>

#include "udavc.h"
#include "elvactools/utils.h"
#include "gui/client_main_window.h"

const QString CompanyName               = Core::CompanyName;
const QString ClientProgramName         = QString("udavc");
const QString ClientUserFriendlyName    = QString(QObject::tr("UDAV client"));
const QString ClientLogFileName         = QString("udavc.log");

Core::ClientSettings* clientSettings;
elvactools::Logger* clientLogger;

Core::PacketType packetType = Core::PacketType();
Core::ConnectionType connectionType = Core::ConnectionType();

QTextCodec* utf8Codec;

QDesktopWidget* desktop;

#ifdef UDAV_COMBO

#include "server.h"
#include "server_settings.h"

Core::Server* server;
Core::ServerSettings* serverSettings;
elvactools::Logger* serverLogger;

const QString ServerProgramName         = QString("udavs");
const QString ServerUserFriendlyName    = QString(QObject::tr("UDAV server"));
const QString ServerLogFileName         = QString("udavs.log");


#endif

//-----------------------------------------------------------------------------

void
messageOutput_(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    // see issue #24

    QByteArray localMsg = msg.toLocal8Bit();
    switch (type)
    {
        case QtDebugMsg:

        break;
        case QtWarningMsg:

        break;
        case QtCriticalMsg:

        break;
        case QtFatalMsg:

        break;
    }
}

//-----------------------------------------------------------------------------

int
main(int argc, char *argv[])
{

    utf8Codec = QTextCodec::codecForName("UTF-8");

    QTranslator appTranslator;
    appTranslator.load(ClientProgramName + "_" + QLocale::system().name(), ":/translate");
#ifdef UDAV_COMBO
    appTranslator.load(ServerProgramName + "_" + QLocale::system().name(), ":/translate");
#endif
    Core::initTranslator();

    QApplication app(argc, argv);
    app.installTranslator(&appTranslator);
    app.installTranslator(&Core::translator);

#ifdef UDAV_COMBO

    //
    // Create server object if using combo mode
    //

    serverSettings = new Core::ServerSettings(CompanyName, ClientProgramName);
    serverSettings->save(); // Принудительное сохранение обеспечит запись вновь появившихся настроек значениями по умолчанию

    QString serverLogPath = Core::checkBackSlash(serverSettings->logPath());

    serverLogger = new elvactools::Logger(serverLogPath + ServerLogFileName);

    if (serverSettings->clearLogAtStart()) {
        serverLogger->clearLogFile();
    }

    serverLogger->useStdOut(true);

    serverLogger->writeEmptyLine();
    serverLogger->writeInfoMessage(QCoreApplication::tr("Starting UDAV server..."));

    Core::initNetworkPacketNames();

    server = Factories::createServer();

    //QObject::connect(server_, SIGNAL(serverStopped()), this, SLOT(quit()));

    if (!server->listen(QHostAddress::Any, serverSettings->port()))
    {
        QString message = QCoreApplication::tr("Failed bind server to port %1").arg(serverSettings->port());

        serverLogger->writeCriticalMessage(message);
        serverLogger->writeEmptyLine();

        delete server;
        delete serverSettings;
        delete serverLogger;

    }
    serverLogger->writeInfoMessage(QCoreApplication::tr("UDAV server is now listening at port %1").arg(serverSettings->port()));

#endif // #ifdef UDAV_COMBO

    clientSettings = new Core::ClientSettings(CompanyName, ClientProgramName);

    QString logPath = Core::checkBackSlash(clientSettings->logPath());

    clientLogger = new elvactools::Logger(logPath + "udavc.log");
    clientLogger->useStdOut(true);

    if (clientSettings->clearLogAtStart()) clientLogger->clearLogFile();

    clientLogger->writeInfoMessage(QObject::tr("Creating UDAV client..."));

    Core::Client* client = new Core::Client();
    QObject::connect(client, SIGNAL(criticalMessage(QString)), clientLogger, SLOT(writeCriticalMessage(QString)));
    QObject::connect(client, SIGNAL(debugMessage(QString)), clientLogger, SLOT(writeDebugMessage(QString)));
    QObject::connect(client, SIGNAL(infoMessage(QString)), clientLogger, SLOT(writeInfoMessage(QString)));

    clientLogger->writeInfoMessage(QObject::tr("Initialising packet names..."));
    Core::initNetworkPacketNames();

    desktop = app.desktop();
    clientLogger->writeInfoMessage(QObject::tr("Found %1 screen(s)").arg(desktop->screenCount()));
    if (desktop->isVirtualDesktop())
    {
        clientLogger->writeInfoMessage(QObject::tr("Desktop is in virtual mode"));
    }

    for (int i = 1; i <= desktop->screenCount(); ++i)
    {
        QRect screen = desktop->screenGeometry(i);
        clientLogger->writeInfoMessage(QObject::tr("Geometry for screen #%1: x=%2; y=%3; width=%4; height=%5").
                                 arg(i).
                                 arg(screen.x()).
                                 arg(screen.y()).
                                 arg(screen.width()).
                                 arg(screen.height()));
    }

    clientLogger->writeInfoMessage(QObject::tr("Creating main window..."));
    udav_gui::ClientMainWindow mainWindow(client);
    QObject::connect(&mainWindow, SIGNAL(criticalMessage(QString)), clientLogger, SLOT(writeCriticalMessage(QString)));
    QObject::connect(&mainWindow, SIGNAL(debugMessage(QString)), clientLogger, SLOT(writeDebugMessage(QString)));
    QObject::connect(&mainWindow, SIGNAL(infoMessage(QString)), clientLogger, SLOT(writeInfoMessage(QString)));

    mainWindow.show();
    if (clientSettings->startInFullScreenMode())
    {
        mainWindow.switchFullScreen();
    }

    clientLogger->writeInfoMessage(QObject::tr("Starting UDAV client..."));
    int res = app.exec();

    delete client;
    delete clientSettings;

    clientLogger->writeInfoMessage(QObject::tr("Exiting..."));
    clientLogger->writeEmptyLine();
    delete clientLogger;

    return res;
}
