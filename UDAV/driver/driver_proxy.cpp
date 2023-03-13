#include "driver_proxy.h"

namespace udav_drivers
{

//-----------------------------------------------------------------------------

DriverProxy::DriverProxy(elvactools::Logger* logger, QObject* parent) :
    QObject(parent)
{
    ELVACTOOLS_LOGGER_SYSTEM_CONNECT(this, logger)
}

//-----------------------------------------------------------------------------

void
DriverProxy::onControlSignalChanged(SignalID signalID, const QVariant& value)
{
    emit infoMessage(tr("Processing signal from UI! Sending control data to equipment..."));

    MetaSignal::ValueType valueType = metaSignals_[signalID].valueType;
    emit infoMessage(tr("Signal ID: %1, value: %2").arg(signalID).arg(value.toString()));
    emit infoMessage(tr("Signal type: %2").arg(MetaSignal::valueType2String(valueType)));

}

//-----------------------------------------------------------------------------

} // namespace udav_drivers

//-----------------------------------------------------------------------------
// EOF
