#ifndef DRIVER_INTERFACE_H
#define DRIVER_INTERFACE_H

// reviewed by @kleomash 2022-12-23

#include <QObject>
#include <QString>
#include <QUuid>
#include <QTranslator>

#include "drivers_global.h"
#include "drivers.h"

#include "core/core.h"
#include "elvactools/block_stream.h"
#include "elvactools/logger.h"
#include "driver_settings.h"
#include "meta_signal.h"

namespace udav_drivers
{

class DriverImpl;
class DriversManager;

/**
 * @brief The DriverInterface class is a interface wrapper for the actual
 * hardware implementation class.
 *
 * An instance of DriverInterface is automatically created and loaded into
 * memory as plugin when the application starts.
 *
 * Further, as requests are received from the user to connect to the equipment,
 * the instance of DriverInterface creates an instance of DriverImpl that
 * performs the actual interaction between the application and the equipment.
 *
 * When writing a driver for real equipment, it is necessary to inherit a new
 * class from the DriverInterface class, to ensure that the plugin describing
 * the hardware is loaded, and from the DriverImpl class to implement
 * the logic of the equipment.
 *
 */
class DriverInterface : public QObject
{
    Q_OBJECT
    ELVACTOOLS_LOGGER_SYSTEM_DECLARE

    friend class DriversManager;

    friend QObject* qt_plugin_instance();

    public:

        /**
         * @brief Return the user-friendly description of the driver.
         * @return Description of the driver.
         */
        virtual QString description() const;

        /**
         * @brief Return reference to metasignals describing signals provided
         * by equipment.
         *
         * @return Reference to metasignals
         */
        virtual const MetaSignals& metaSignals() const;

        /**
         * @brief Return driver name.
         * @return Driver name.
         */
        virtual QString name() const;

        /**
         * @brief Print driver description into BlockStream.
         * @param os - a stream
         * @return Reference to the sream
         */
        Core::BlockStream& printToBlockStream(Core::BlockStream& os);

        /**
         * @brief Return uuid for dirver implementation.
         * @return uuid for dirver implementation
         */
        virtual QUuid uuid() const;

    protected :

        /**
         * @brief DriverInterface - default constructor.
         * @param uuid - driver uuid
         * @param name - driver name
         * @param description - driver description
         *
         * Values for parameters should be set in descendant constructor.
         */
        explicit DriverInterface(const QString& uuid, const QString& name, const QString& description);

        /**
         * @brief Destructor
         **/
        virtual ~DriverInterface();

        /**
         * @brief Check if driver implementation created correctly
         * @param drvImpl - pointer to driver implementations
         * @return true if implementation inited correctly, false overwise
         *
         * Here we can check if all devices inited and working, for example.
         */
        bool checkDriverImpl(DriverImpl* drvImpl) const;

        /**
         * @brief Create the driver implementation.
         * @return Pointer to dirver implementation instance or nullptr in case of error.
         *
         * The function must be implemented in a descendant class.
         * If it is impossible to create an implementation, for example,
         * the equipment is busy, then the function should return nullptr
         * and set ok parameter to false value.
         */
        virtual DriverImpl* createDriverImpl() = 0;

        /**
         * @brief Initialize an array of descriptions of driver control signals.
         *
         * The function must be implemented in a descendant class.
         */
        virtual void initControlMetaSignals_() = 0;

        /**
         * @brief Init driver settings.
         *
         * The function must be implemented in a descendant class.
         */
        virtual void initDefaultSettings_() = 0;

        /**
         * @brief Initializes an array of driver supervision signal descriptions.
         *
         * The function must be implemented in a descendant class.
         */
        virtual void initViewMetaSignals_() = 0;

        /**
         * @brief Set logger to print messages
         * @param logger - logger
         */
        void setLogger_(elvactools::Logger* logger);

    protected slots: // Logging subsystem

        /**
         * @brief Process a critical message, appending the driver name to it,
         * and pass it on to the logger.
         *
         * @param message - message to log.
         *
         * @sa criticalMessage_();
         */
        void processCriticalMessage_(const QString message);

        /**
         * @brief Process a debug message, appending the driver name to it,
         * and pass it on to the logger.
         *
         * @param message - message to log.
         *
         * @sa debugMessage_();
         */
        void processDebugMessage_(const QString message);

        /**
         * @brief Process an info message, appending the driver name to it,
         * and pass it on to the logger.
         *
         * @param message - message to log.
         *
         * @sa infoMessage_();
         */
        void processInfoMessage_(const QString message);

    signals: // protected - Logging subsystem

        /**
         * @brief Send a fatal error message to the driver's message processing
         * subsystem
         *
         * @param message - message to log.
         *
         * Message will be processed by processCriticalMessage_()
         *
         * @sa processCriticalMessage_()
         */
        void criticalMessage_(const QString& message) const;

        /**
         * @brief Send a debug message to the driver's message processing
         * subsystem
         *
         * @param message - message to log.
         *
         * Message will be processed by processDebugMessage_()
         *
         * @sa processDebugMessage_();
         */
        void debugMessage_(const QString& message) const;

        /**
         * @brief Send an info message to the driver's message processing
         * subsystem
         *
         * @param message - message to log.
         *
         * Message will be processed by processInfoMessage_()
         *
         * @sa processInfoMessage_();
         */
        void infoMessage_(const QString& message) const;

    protected:

        /**
         * @brief User friendly driver description.
         */
        QString description_;

        /**
         * @brief Driver uuid.
         */
        QUuid uuid_;

        /**
         * @brief Array of metasignals provided by driver.
         */
        MetaSignals metaSignals_;

        /**
         * @brief Driver settings.
         */
        DriverSettings settings_;

        /**
         * @brief Logger to print messages.
         */
        elvactools::Logger* logger_;
};

//-----------------------------------------------------------------------------

}

Q_DECLARE_INTERFACE(udav_drivers::DriverInterface, "ru.elvactech.udav.DriversInterface/1.0")

#endif // DRIVER_INTERFACE_H
