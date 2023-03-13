#ifndef UDAV_H
#define UDAV_H

#include "core.h"
#include "udav_settings.h"

#include "elvactools/logger.h"

namespace Core
{
    class UdavSettings;
}

class QDesktopWidget;

extern elvactools::Logger* logger;

extern Core::UdavSettings* settings;

extern const QString CompanyName;

extern const QString PROGRAM_NAME;

extern const QString PROGRAM_USER_FRIENDLY_NAME;

extern const QString LOG_FILE_NAME;

#endif // UDAV_H
