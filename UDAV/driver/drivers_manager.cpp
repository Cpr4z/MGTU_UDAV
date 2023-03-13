#include <QDir>
#include <QPluginLoader>

#include "elvactools/utils.h"

#include "driver_interface.h"
#include "driver_impl.h"

#include "drivers_manager.h"

//-----------------------------------------------------------------------------

namespace udav_drivers
{

//-----------------------------------------------------------------------------

DriversManager::DriversManager(elvactools::Logger* logger, QObject *parent)
    : QObject(parent),
      logger_(logger)
{
    ELVACTOOLS_LOGGER_SYSTEM_CONNECT(this, logger)

    emit infoMessage(tr("Loading equipment drivers..."));

    QDir driversDir = Core::directoryOf("drivers");
    emit infoMessage(tr("Using %1 as path to drivers").arg(driversDir.absolutePath()));

    int i = 0;

    drivers_.clear();

    const auto entryList = driversDir.entryList(QDir::Files);

    for (const QString& fileName : entryList)
    {
        QString fullPathName = driversDir.absoluteFilePath(fileName);

        if (!QLibrary::isLibrary(fullPathName))
        {
            continue;
        }

        emit infoMessage(tr("Loading %1").arg(fullPathName));
        QPluginLoader loader(fullPathName);

        if (DriverInterface* driverInterface = qobject_cast<DriverInterface*>(loader.instance()))
        {
            emit infoMessage(tr("[%1] %2 : %3 : loaded successfully").
                             arg(driverInterface->name(),
                                 driverInterface->description(),
                                 driverInterface->uuid().toString()));

            driverInterface->setLogger_(logger);

            driverInterface->initDefaultSettings_();
            driverInterface->initControlMetaSignals_();
            driverInterface->initViewMetaSignals_();

            drivers_[driverInterface->uuid()] = driverInterface;

            i++;
        }
        else
        {
            emit criticalMessage(loader.errorString());
        }
    }

    emit infoMessage(tr("Loaded %1 driver(s)").arg(i));
}

//-----------------------------------------------------------------------------

DriversManager::~DriversManager()
{
    emit infoMessage(tr("Unloading drivers manager..."));
}

//-----------------------------------------------------------------------------

QList<Core::UniversalNamedMap>
DriversManager::availableDriversList() const
{
    QList<Core::UniversalNamedMap> result;

    for(auto item : drivers_)
    {
        Core::UniversalNamedMap driverInfo;
        driverInfo["uuid"] = item->uuid().toString();
        driverInfo["name"] = item->name();
        driverInfo["desc"] = item->description();

        result.append(driverInfo);
    }

    return result;
}

//-----------------------------------------------------------------------------

DriverImpl*
DriversManager::createDriverImpl(const QUuid& uuid)
{
    DriverImpl* result = nullptr;

    auto i = drivers_.find(uuid);

    if (i != drivers_.end())
    {
        auto driverInterface = i.value();

        emit infoMessage(tr("Creating %1 driver implementation...").arg(driverInterface->name()));

        emit infoMessage(tr("Driver has %1 metasignal(s):").arg(driverInterface->metaSignals().count()));
        emit infoMessage(tr("\t SignalID : Name : Desc : Type : MinValue : MaxValue : DefaultValue"));

        MetaSignalsJavaStyleIterator i(driverInterface->metaSignals());
        while (i.hasNext())
        {
            i.next();
            udav_drivers::MetaSignal item = i.value();
            emit infoMessage(tr("\t %1 : %2 : %3 : %4 : %5 : %6 : %7").
                             arg(i.key()).
                             arg(item.fieldName).
                             arg(item.fieldDesc).
                             arg(item.valueType2String(item.valueType)).
                             arg(item.minValue).
                             arg(item.maxValue).
                             arg(item.defaultValue));
        }

        result = driverInterface->createDriverImpl();

        if (result)
        {
            result->setParent(this);
        }
    }

    if (!result)
    {
        emit criticalMessage(tr("Error creating implementation for driver with uuid: %1!").
                             arg(uuid.toString()));
    }

    return result;
}

//-----------------------------------------------------------------------------

} // udav_drivers

