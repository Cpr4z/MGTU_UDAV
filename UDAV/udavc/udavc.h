#ifndef UDAVC_H
#define UDAVC_H

#include "client.h"
#include "client_settings.h"

#include "core.h"

#include "elvactools/logger.h"

class QDesktopWidget;

/*
 * Глобальные переменные программы-клиента.
 */

//! Вывод сообщений программы в лог-файл и в окно вывода сообщений.
extern elvactools::Logger* clientLogger;

//! Сохранятель настроек.
extern Core::ClientSettings* clientSettings;

extern const QString CompanyName;

extern const QString ClientProgramName;

extern const QString ClientUserFriendlyName;

extern const QString ClientLogFileName;


#ifdef UDAV_COMBO

//
// If we use combo mode declare global vars for server side
//

namespace Core {

class Server;
class ServerSettings;

}

extern Core::Server* server;
extern Core::ServerSettings* serverSettings;
extern elvactools::Logger* serverLogger;

extern const QString ServerProgramName;
extern const QString ServerUserFriendlyName;
extern const QString ServerLogFileName;

#endif


#endif // UDAVC_H
