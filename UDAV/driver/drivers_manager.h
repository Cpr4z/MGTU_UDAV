#ifndef DRIVERS_MANAGER_H
#define DRIVERS_MANAGER_H

#include <QObject>
#include <QList>

//#include "elvactools/class_factory.h"
#include "elvactools/logger.h"
#include "core/core.h"

#include "drivers.h"

namespace udav_drivers
{

class DriverImpl;

/**
 * @brief The DriversManager class designed to manage available drivers
 *
 * At creation driver loads drivers uinterfaces plugins from disk
 * and create driver implementation when we need it.
 */
class DriversManager : public QObject
{
    Q_OBJECT
    ELVACTOOLS_LOGGER_SYSTEM_DECLARE

    public:

        /**
         * @brief DriversManager - constructor.
         * @param logger - logger for messaging.
         * @param parent - owner object.
         */
        explicit DriversManager(elvactools::Logger* logger, QObject *parent = nullptr);

        /**
         * @brief Destructor.
         */
        ~DriversManager();

        /**
         * @brief Return list of available driver in universal named map format.
         * @return List of available driver in universal named map format.
         */
        QList<Core::UniversalNamedMap> availableDriversList() const;

        /**
         * @brief Create driver implementation and return pointer to it.
         * @param uuid - uuid of driver implementation to be created.
         * @return Pointer to driver implementation or nullptr in case of an error.
         */
        DriverImpl* createDriverImpl(const QUuid& uuid);

    private:

        /**
         * @brief Available drivers grouped by their uuid.
         */
        QMap<QUuid, DriverInterface*> drivers_;

        /**
         * @brief Logger for messaging.
         */
        elvactools::Logger* logger_;
};

} // namespace udav_drivers

#endif // DRIVERS_MANAGER_H
