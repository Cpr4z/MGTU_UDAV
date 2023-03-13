#include "driver_interface.h"

//-----------------------------------------------------------------------------

namespace udav_drivers
{

//-----------------------------------------------------------------------------

DriverInterface::DriverInterface(const QString& uuid, const QString& name, const QString& description)
    : description_(description),
      uuid_(uuid),
      settings_(Core::CompanyName, Core::ServerExeName, name)
{
    setObjectName(name);

    logger_ = nullptr;

    settings_.read();
}

//-----------------------------------------------------------------------------

DriverInterface::~DriverInterface()
{
}

//-----------------------------------------------------------------------------

bool
DriverInterface::checkDriverImpl(DriverImpl* drvImpl) const
{
    if (drvImpl != nullptr)
    {
        emit infoMessage_(tr("Driver implementation is not nullptr!"));
        return true;
    }
    else
    {
        emit criticalMessage_(tr("Driver implementation is nullptr!"));
        return false;
    }
}

//-----------------------------------------------------------------------------

QString
DriverInterface::description() const
{
    return description_;
}

//-----------------------------------------------------------------------------

const MetaSignals&
DriverInterface::metaSignals() const
{
    return metaSignals_;
}


//-----------------------------------------------------------------------------

QString
DriverInterface::name() const
{
    return objectName();
}

//-----------------------------------------------------------------------------

Core::BlockStream&
DriverInterface::printToBlockStream(Core::BlockStream& os)
{
    os
            << uuid_
            << name()
            << description_;
    return os;
}

//-----------------------------------------------------------------------------

void
DriverInterface::processInfoMessage_(const QString message)
{
    QString fullMessage = "[" + name() + "] " + message;
    emit infoMessage(fullMessage);
}

//-----------------------------------------------------------------------------

void
DriverInterface::processDebugMessage_(const QString message)
{
    QString fullMessage = "[" + name() + "] " + message;
    emit debugMessage(fullMessage);
}

//-----------------------------------------------------------------------------

void
DriverInterface::processCriticalMessage_(const QString message)
{
    QString fullMessage = "[" + name() + "] " + message;
    emit criticalMessage(fullMessage);
}

//-----------------------------------------------------------------------------

void
DriverInterface::setLogger_(elvactools::Logger* logger)
{
    logger_ = logger;

    ELVACTOOLS_LOGGER_SYSTEM_CONNECT(this, logger_)

    connect(this, SIGNAL(criticalMessage_(QString)), this, SLOT(processCriticalMessage_(QString)));
    connect(this, SIGNAL(infoMessage_(QString)), this, SLOT(processInfoMessage_(QString)));
    connect(this, SIGNAL(debugMessage_(QString)), this, SLOT(processDebugMessage_(QString)));

    emit infoMessage_(tr("Driver interface connected with logger"));
}

//-----------------------------------------------------------------------------

QUuid
DriverInterface::uuid() const
{
    return uuid_;
}

//-----------------------------------------------------------------------------

} // namespace udav_drivers

//-----------------------------------------------------------------------------
// EOF
