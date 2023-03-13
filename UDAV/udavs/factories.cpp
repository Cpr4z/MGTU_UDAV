#include "factories.h"

#include <QObject>

#ifdef UDAV_COMBO
    #include "udavc.h"
#else
    #include "udavs.h"
    #include "server_application.h"
#endif

#include "server.h"
#include "elvactools/logger.h"

namespace Factories
{

Core::Server*
createServer(QObject *parent)
{
    Core::Server* server = new Core::Server(parent);

#ifdef UDAV_COMBO
    QObject::connect(server, SIGNAL(infoMessage(QString)), serverLogger, SLOT(writeInfoMessage(QString)));
    QObject::connect(server, SIGNAL(criticalMessage(QString)), serverLogger, SLOT(writeCriticalMessage(QString)));
    QObject::connect(server, SIGNAL(debugMessage(QString)), serverLogger, SLOT(writeDebugMessage(QString)));
#else
    QObject::connect(server, SIGNAL(infoMessage(QString)), serverApp->logger(), SLOT(writeInfoMessage(QString)));
    QObject::connect(server, SIGNAL(criticalMessage(QString)), serverApp->logger(), SLOT(writeCriticalMessage(QString)));
    QObject::connect(server, SIGNAL(debugMessage(QString)), serverApp->logger(), SLOT(writeDebugMessage(QString)));
#endif

    server->loadDrivers_();

    return server;
}


}
