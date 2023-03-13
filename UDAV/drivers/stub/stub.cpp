#include "stub.h"

// Step 1. Select name for a new driver
//
// Rename classes and files, replace Stub with selected name.

#include <QtPlugin>
#include <QStringList>

#include "stub_impl.h"

namespace udav_drivers {

//
// Step 2. Add string constants for driver options names
//
//const QString StubConf::TIC_port = "TIC_port";
//const QString StubConf::TIC_queueTimerInterval = "TIC_queueTimerInterval";
//const QString StubConf::TIC_requestTimerInterval = "TIC_requestTimerInterval";
//const QString StubConf::TIC_responseTimeOut = "TIC_responseTimeOut";

//
// Step 3. Declare constants for signal codes
//
const SignalID Stub::CS_SWITCH_LED;
const SignalID Stub::VS_TIME;
const SignalID Stub::VS_LED_STATUS;

//-----------------------------------------------------------------------------

// Step 4. Replace UUID with id assigned to new created driver.
//
// You can generate it with https://www.uuidgenerator.net/ for example.
//
// After replace driver name and description with desired string.

const char* DRIVER_NAME         = "stub";
const char* DRIVER_DESCRIPTION  = "UDAV stub driver, version 1.0";
const char* DRIVER_UUID         = "b3fe28d6-22fe-42c3-8b21-d95e4c247ebf";

//-----------------------------------------------------------------------------

Stub::Stub() :
    DriverInterface(QUuid(DRIVER_UUID).toString(), QString(DRIVER_NAME), QString(DRIVER_DESCRIPTION))
{
}

//-----------------------------------------------------------------------------

//
// Step 5. Replace impementation type to real river implementation.
//

DriverImpl*
Stub::createDriverImpl()
{
    const bool simulate = false;

    DriverInterface* owner = this;

    DriverImpl* driverImpl = dynamic_cast<DriverImpl*>(new StubImpl(metaSignals_, settings_, owner, simulate, logger_));

    return driverImpl;
}

//-----------------------------------------------------------------------------

//
// Step 6. Init control meta signals according siganls table.
//

void
Stub::initControlMetaSignals_()
{
    MetaSignal switchLED;
    switchLED.fieldName = "CS_SWITCH_LED";
    switchLED.fieldDesc = tr("Switch LED state");
    switchLED.signalType = MetaSignal::Control;
    switchLED.valueType = MetaSignal::Boolean;
    switchLED.minValue = 0.0;
    switchLED.maxValue = 0.0;
    switchLED.defaultValue = 0.0;
    metaSignals_[CS_SWITCH_LED] = switchLED;
}

//-----------------------------------------------------------------------------

//
// Step 7. Fill driver settings with default values.
//

void
Stub::initDefaultSettings_()
{
//    if (!settings_.contains(Lab311Conf::TicBus_port))
//        settings_.addSetting(Lab311Conf::TicBus_port, "ttyS1");

    settings_.save();
}

//-----------------------------------------------------------------------------

//
// Step 8. Init view meta signals according siganls table.
//

void
Stub::initViewMetaSignals_()
{
    MetaSignal time;
    time.fieldName = "VS_TIME";
    time.fieldDesc = tr("Time");
    time.signalType = MetaSignal::Observation;
    time.valueType = MetaSignal::Time;
    time.minValue = 0.0;
    time.maxValue = 0.0;
    time.defaultValue = 0.0;
    metaSignals_[VS_TIME] = time;

    MetaSignal ledStatus;
    ledStatus.fieldName = "VS_LED_STATUS";
    ledStatus.fieldDesc = tr("Led status");
    ledStatus.signalType = MetaSignal::Observation;
    ledStatus.valueType = MetaSignal::Boolean;
    ledStatus.minValue = 0.0;
    ledStatus.maxValue = 0.0;
    ledStatus.defaultValue = 0.0;
    metaSignals_[VS_LED_STATUS] = ledStatus;
}

//-----------------------------------------------------------------------------

// All done. Remove unneeded comments and go!

}
