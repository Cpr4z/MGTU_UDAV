#ifndef FACTORIES_H
#define FACTORIES_H

#include <QObject>

namespace Core
{
    class Server;
    class ServerSideSocket;
}

namespace Factories
{

Core::Server* createServer(QObject *parent = 0);

}


#endif // FACTORIES_H
