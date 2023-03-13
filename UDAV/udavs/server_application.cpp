#include <QTranslator>
#include <QTextCodec>

#include "udavs.h"

#include "server_application.h"

#include "core.h"
#include "utils.h"

#include "server.h"
#include "server_settings.h"
#include "elvactools/logger.h"

#include "factories.h"

const QString CompanyName = Core::CompanyName;
const QString ClientProgramName = QString("udavs");
const QString ClientLogFileName = QString("udavs.log");

Core::Logger* localLogger;

//-----------------------------------------------------------------------------

void
messageOutput_(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
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

ServerApplication::ServerApplication(int &argc, char ** argv) :
    QCoreApplication(argc, argv)
{
    serverApp = this;

    //! @todo check call in Qt5!
    // qInstallMessageHandler(Core::messageOutput);

    // Устанавливаем кодеки для корректного вывода строк
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForLocale(codec);

    QTranslator appTranslator;
    appTranslator.load("udavs_" + QLocale::system().name(), ":/");
    installTranslator(&appTranslator);

    Core::initTranslator();
    installTranslator(&Core::translator);

    //srand(time(NULL)); // Инициализируем генератор случайных чисел. Пригодится при моделировании процессов.

    settings_ = new Core::ServerSettings(CompanyName, ClientProgramName);
    settings_->save(); // Принудительное сохранение обеспечит запись вновь появившихся настроек значениями по умолчанию

    QString logPath = Core::checkBackSlash(settings_->logPath());

    logger_ = new Core::Logger(logPath + ClientLogFileName);
    if (settings_->clearLogAtStart()) logger_->clearLogFile();

    logger_->useStdOut(true);
    localLogger = logger_;

    logger_->writeEmptyLine();
    logger_->writeInfoMessage(QCoreApplication::tr("Starting UDAV server..."));

    Core::initNetworkPacketNames();

    server_ = Factories::createServer();

    QObject::connect(server_, SIGNAL(serverStopped()), this, SLOT(quit()));

    if (!server_->listen(QHostAddress::Any, settings_->port()))
    {
        QString message = QCoreApplication::tr("Failed bind to port %1").arg(settings_->port());

        logger_->writeCriticalMessage(message);
        logger_->writeEmptyLine();

        delete server_;
        delete settings_;
        delete logger_;

        qFatal(message.toStdString().c_str());
    }
    logger_->writeInfoMessage(QCoreApplication::tr("UDAV server is now listening at port %1").arg(settings_->port()));
}

//-----------------------------------------------------------------------------

ServerApplication::~ServerApplication()
{
    logger_->writeInfoMessage(tr("Exiting from UDAV server application..."));
    logger_->writeEmptyLine();

    delete server_;
    delete settings_;
    delete logger_;
}

//-----------------------------------------------------------------------------
