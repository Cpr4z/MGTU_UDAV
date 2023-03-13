#include "process.h"
#include "driver_impl.h"

namespace udav_drivers
{

Process::Process(DriverImpl *driverImpl, SignalID signalID, const QString& name)
    :
      driverImpl_(driverImpl),
      name_(name),
      signalID_(signalID),
      stopped_(true)
{
    connect(this, SIGNAL(criticalMessage_(QString)), this, SLOT(processCriticalMessage(QString)));
    connect(this, SIGNAL(infoMessage_(QString)), this, SLOT(processInfoMessage(QString)));
    connect(this, SIGNAL(debugMessage_(QString)), this, SLOT(processDebugMessage(QString)));

    connect(driverImpl_, SIGNAL(viewSignalsReaded()), this, SLOT(checkState()));

    connect(this, SIGNAL(processStarted(SignalID, QString)), driverImpl_, SLOT(processStarted(SignalID, QString)));
    connect(this, SIGNAL(processStopped(SignalID, QString)), driverImpl_, SLOT(processStopped(SignalID, QString)));

    connect(this, SIGNAL(stopProcess(SignalID)), driverImpl_, SLOT(stopProcess(SignalID)));
    connect(this, SIGNAL(readViewValues()), driverImpl_, SLOT(readViewValues_()));
}

}
