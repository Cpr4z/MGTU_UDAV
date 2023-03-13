#include <QDir>
#include <QPluginLoader>

#ifdef UDAV_COMBO
#include "udavc.h"
#else
#include "udavs.h"
#include "server_application.h"
#endif

#include "server.h"
#include "core.h"
#include "server_side_socket.h"
#include "elvactools/utils.h"
#include "elvactools/logger.h"

#include "driver_interface.h"
#include "driver_impl.h"

namespace Core
{

//-----------------------------------------------------------------------------

Server::Server(QObject* parent) :
        QTcpServer(parent)
{
//    connect(this, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
}

//-----------------------------------------------------------------------------

Server::~Server()
{
//	Drivers::AbstractEquipmentDriver* curItem;
//	foreach(curItem, equipment)
//	{
//		delete curItem;
//	}
//	equipment.clear();

//    consoleKeyReader->wait();
//    delete consoleKeyReader;

    emit infoMessage(tr("Shutting down UDAV server..."));
    close();
}

//-----------------------------------------------------------------------------

QString
Server::addSession_(ServerSideSocket* s)
{
    QUuid newUuid = QUuid::createUuid();
    sessions_[newUuid] = s;

    emit infoMessage(tr("Session with UUID=%1 added to opened sessions list").arg(newUuid.toString()));

    return newUuid.toString();
}

//-----------------------------------------------------------------------------

void
Server::connectionClosed_(ServerSideSocket *socket)
{
    emit infoMessage(tr("Connection with client %1 closed").arg(socket->peerAddress().toString()));

    QUuid uuid = sessions_.key(socket);
    deleteSession_(uuid);

}

//-----------------------------------------------------------------------------

void
Server::consoleKeyPressed_(char ch)
{
    if (ch == 'q')
    {
        emit infoMessage(tr("Quit command recieved from keyboard!"));

        emit serverStopped();
    }
}

//-----------------------------------------------------------------------------

void
Server::deleteSession_(const QUuid& uuid)
{
    sessions_.remove(uuid.toString());

    emit infoMessage(tr("Session with UUID=%1 deleted").arg(uuid.toString()));
}

//-----------------------------------------------------------------------------

Drivers::DriverImpl*
Server::getDriver(const QUuid& uuid)
{
    Drivers::DriverImpl* driverImpl = NULL;
    foreach(Drivers::DriverInterface* item, equipment_)
    {
        if (item->getUuid() == uuid)
        {
            bool ok = true;
            driverImpl = item->createDriverImpl(&ok);

            if (!ok)
                driverImpl = NULL;
            else
            {
#ifdef UDAV_COMBO
                connect(driverImpl, SIGNAL(criticalMessage(QString)), serverLogger, SLOT(writeCriticalMessage(QString)));
                connect(driverImpl, SIGNAL(debugMessage(QString)), serverLogger, SLOT(writeDebugMessage(QString)));
                connect(driverImpl, SIGNAL(infoMessage(QString)), serverLogger, SLOT(writeInfoMessage(QString)));
#else
                connect(driverImpl, SIGNAL(criticalMessage(QString)), serverApp->logger(), SLOT(writeCriticalMessage(QString)));
                connect(driverImpl, SIGNAL(debugMessage(QString)), serverApp->logger(), SLOT(writeDebugMessage(QString)));
                connect(driverImpl, SIGNAL(infoMessage(QString)), serverApp->logger(), SLOT(writeInfoMessage(QString)));
#endif
            }
            break;
        }
    }
    return driverImpl;
}

//-----------------------------------------------------------------------------

QUuid
Server::getSessionUuid(ServerSideSocket* socket)
{
    QUuid uuid = sessions_.key(socket);
    return uuid;
}

//-----------------------------------------------------------------------------

void
Server::incomingConnection(qintptr socketId)
{
    emit infoMessage(tr("New incoming connection requested"));

    // Сокет уничтожается автоматически после того, как соединения будет закрыто
    ServerSideSocket *socket = new ServerSideSocket(this);

    socket->setSocketDescriptor(socketId);

    connect(socket, SIGNAL(disconnected(ServerSideSocket *)), this, SLOT(connectionClosed_(ServerSideSocket *)));

#ifdef UDAV_COMBO
    connect(socket, SIGNAL(criticalMessage(QString)), serverLogger, SLOT(writeCriticalMessage(QString)));
    connect(socket, SIGNAL(debugMessage(QString)), serverLogger, SLOT(writeDebugMessage(QString)));
    connect(socket, SIGNAL(infoMessage(QString)), serverLogger, SLOT(writeInfoMessage(QString)));
#else
    connect(socket, SIGNAL(criticalMessage(QString)), serverApp->logger(), SLOT(writeCriticalMessage(QString)));
    connect(socket, SIGNAL(debugMessage(QString)), serverApp->logger(), SLOT(writeDebugMessage(QString)));
    connect(socket, SIGNAL(infoMessage(QString)), serverApp->logger(), SLOT(writeInfoMessage(QString)));
#endif

    addSession_(socket);
}

//-----------------------------------------------------------------------------

void
Server::loadDrivers_()
{
    emit infoMessage(tr("Loading equipment drivers..."));

    QDir driversDir = directoryOf("drivers");
    emit infoMessage(tr("Using %1 as path to drivers").arg(driversDir.absolutePath()));

    int i = 0;

    equipment_.clear();
    foreach (QString fileName, driversDir.entryList(QDir::Files))
    {
        QString fullPathName = driversDir.absoluteFilePath(fileName);
        emit infoMessage(tr("Loading %1").arg(fullPathName));
        QPluginLoader loader(fullPathName);

        if (Drivers::DriverInterface* driver = qobject_cast<Drivers::DriverInterface*>(loader.instance()))
        {
            equipment_.append(driver);
            connect(driver, SIGNAL(infoMessage(QString)), this, SIGNAL(infoMessage(QString)));
            connect(driver, SIGNAL(debugMessage(QString)), this, SIGNAL(debugMessage(QString)));
            connect(driver, SIGNAL(criticalMessage(QString)), this, SIGNAL(criticalMessage(QString)));

            emit infoMessage(tr("%1 : %2 : %3 : loaded successfully").
                             arg(driver->getName()).
                             arg(driver->getDescription()).
                             arg(driver->getUuid().toString()));

            i++;
        }
        else
        {
            emit criticalMessage(loader.errorString());
        }
    }
    emit infoMessage(tr("Loaded %1 driver(s)").arg(i));
}


}


