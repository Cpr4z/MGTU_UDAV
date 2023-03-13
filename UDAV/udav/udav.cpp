#include <QApplication>

#include <QDesktopWidget>
#include <QLocale>
#include <QTranslator>

#include "elvactools/utils.h"

#include "udav_settings.h"

#include "gui/udav_main_window_creator.h"
#include "gui/udav_main_window.h"
#include "udav.h"

const QString CompanyName                   = Core::CompanyName;
const QString PROGRAM_NAME                  = QString("udav");
const QString PROGRAM_USER_FRIENDLY_NAME    = QString(QObject::tr("UDAV"));
const QString LOG_FILE_NAME                 = QString("udav.log");

Core::UdavSettings* settings;
elvactools::Logger* logger;

QDesktopWidget* desktop;

int main(int argc, char *argv[])
{

    settings = new Core::UdavSettings(CompanyName, PROGRAM_NAME);

    QString logPath = Core::checkBackSlash(settings->logPath());

    logger = new elvactools::Logger(logPath + LOG_FILE_NAME);
    logger->useStdOut(true);

    if (settings->clearLogAtStart())
    {
        logger->clearLogFile();
    }

    logger->writeInfoMessage(QObject::tr("Initing %1 application...").arg(PROGRAM_USER_FRIENDLY_NAME));
    QApplication app(argc, argv);

    logger->writeInfoMessage(QObject::tr("Initing %1 trans...").arg(PROGRAM_USER_FRIENDLY_NAME));
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "udav_" + QLocale(locale).name();

        if (translator.load(":/i18n/" + baseName))
        {
            app.installTranslator(&translator);
            break;
        }
    }

    logger->writeInfoMessage(QObject::tr("Initialising packet names..."));
    Core::initNetworkPacketNames();

    logger->writeInfoMessage(QObject::tr("Starting %1 application...").arg(PROGRAM_USER_FRIENDLY_NAME));

    desktop = app.desktop();
    logger->writeInfoMessage(QObject::tr("Found %1 screen(s)").arg(desktop->screenCount()));
    if (desktop->isVirtualDesktop())
    {
        logger->writeInfoMessage(QObject::tr("Desktop is in virtual mode"));
    }

    for (int i = 1; i <= desktop->screenCount(); ++i)
    {
        QRect screen = desktop->screenGeometry(i);
        logger->writeInfoMessage(QObject::tr("Geometry for screen #%1: x=%2; y=%3; width=%4; height=%5").
                                 arg(i).
                                 arg(screen.x()).
                                 arg(screen.y()).
                                 arg(screen.width()).
                                 arg(screen.height()));
    }

    logger->writeInfoMessage(QObject::tr("Creating main window..."));
    udav_gui::UdavMainWindowCreator* mainWindowCreator = new udav_gui::UdavMainWindowCreator(settings, logger);
    udav_gui::UdavMainWindow* mainWindow = mainWindowCreator->createMainWindow();
    delete mainWindowCreator;


    logger->writeInfoMessage(QObject::tr("Main window created, opening..."));
    mainWindow->show();

    logger->writeInfoMessage(QObject::tr("Ready. Waiting to user actions..."));
    int res = app.exec();

    logger->writeInfoMessage(QObject::tr("Deleting main window..."));
    delete mainWindow;

    logger->writeInfoMessage(QObject::tr("Exiting %1 application...").arg(PROGRAM_USER_FRIENDLY_NAME));
    logger->writeEmptyLine();
    delete logger;

    return res;
}
