#ifndef STUB_H
#define STUB_H
//      ^^^^^^
//
// Step 1. Select name for a new driver
//
// Do not foget rename define for real driver name!
// If not renamed it can provide very strange results!
//
// Rename classes and files, replace Stub with selected name.

#include "driver_interface.h"

namespace udav_drivers {

/**
 * @brief The StubConf class - list of names for driver options in config file
 */
class StubConf
{
    public:

        // Step 2. Add string constants for driver options names

        //static const QString TIC_port;
        //static const QString TIC_queueTimerInterval;
        //static const QString TIC_requestTimerInterval;
        //static const QString TIC_responseTimeOut";
};

class Stub : public DriverInterface
{
    Q_OBJECT
    Q_INTERFACES(udav_drivers::DriverInterface)
    Q_PLUGIN_METADATA(IID "ru.elvactech.udav.DriversInterface/1.0")

    public :

        /**
         * @brief Default constructor.
         *
         * We cannot make it private due limitation of Qt plugin arch.
         */
        Stub();

    protected :

        // Step 3. Add constants for signal codes

        // Signal with code 0 is reserved. User interface elements with signal code 0 is not connected to any signal.

        static const SignalID CS_SWITCH_LED = CS_Begin + 1;             //!< Turn On/Off LED

        static const SignalID VS_TIME           = VS_Begin + 0;			//!< Time from start
        static const SignalID VS_LED_STATUS     = VS_TIME  + 1;			//!< LED State

        // Steps 4 and later see in cpp file

        /**
         * @brief Create the driver implementation.
         * @return Pointer to dirver implementation instance or nullptr in case of error.
         */
        virtual  DriverImpl* createDriverImpl() override;

    private:

        /**
         * @brief Init array with description of control signals provided by driver.
         */
        virtual void initControlMetaSignals_() override;

        /**
         * @brief Init default driver settings.
         */
        virtual void initDefaultSettings_() override;

        /**
         * @brief Init array with description of view signals provided by driver.
         */
        virtual void initViewMetaSignals_() override;
};

}

#endif // STUB_H
