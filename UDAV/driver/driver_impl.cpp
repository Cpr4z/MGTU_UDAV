#include "driver_impl.h"

#include <QDebug>

#include "elvacquip/abstract_device.h"
#include "elvacquip/serial_port.h"
#include <modbus4qt/rtu_client.h>
#include "meta_signal.h"
#include "core/core.h"

namespace udav_drivers
{

//-----------------------------------------------------------------------------

DriverImpl::DriverImpl(
        const MetaSignals& metaSignals,
        const DriverSettings& settings,
        DriverInterface* driverInterface,
        bool simulate,
        elvactools::Logger* logger) :
      controlSignalNames_(),
      controlValues_(),
      devicesInited_(false),
      driverInterface_(driverInterface),
      driverSettings_(settings),
      metaSignals_(metaSignals),
      readTimer_(),
      readTimerInterval_(1000),
      simulate_(simulate),
      viewSignalNames_(),
      viewValues_(),
      logger_(logger)
{
    setLogger(logger_);

    initSignalNames_();
    fillDefaultValues_();

    connect(&readTimer_, SIGNAL(timeout()), this, SLOT(readControlValues_()));
    connect(&readTimer_, SIGNAL(timeout()), this, SLOT(readViewValues_()));
}

//-----------------------------------------------------------------------------

DriverImpl::~DriverImpl()
{
   emit infoMessage_(tr("Unloading driver implementation..."));
}

//-----------------------------------------------------------------------------

bool
DriverImpl::connectDevice_(AbstractDevice *device, QUuid msgWinId)
{
    QString successMessage = tr("%1: %2 successfully connected");
    QString errorMessage = tr("Cannot connect with %1");

    connect(device, SIGNAL(infoMessage(QString)), this, SLOT(processInfoMessage_(QString)));
    connect(device, SIGNAL(criticalMessage(QString)), this, SLOT(processCriticalMessage_(QString)));
    connect(device, SIGNAL(debugMessage(QString)), this, SLOT(processDebugMessage_(QString)));

    bool result;

    result = device->init(simulate_);
    result = device->connectDevice();

    QString deviceNameAndVersion;
    if (result)
    {
        result &= device->readDeviceNameAndVersion(deviceNameAndVersion);
    }

    if (result)
    {   emit infoMessage_(successMessage.arg(device->deviceName(), deviceNameAndVersion));
    }
    else
    {
        emit criticalMessage_(errorMessage.arg(device->deviceName()));
    }

    return result;
}

//-----------------------------------------------------------------------------

modbus4qt::RtuClient*
DriverImpl::createModBusConnection_(const QString &name, const QString &port, QSerialPort::BaudRate baudRate, QSerialPort::DataBits dataBits, QSerialPort::StopBits stopBits, QSerialPort::Parity parity, int timeout)
{
    emit infoMessage_(tr("Creating %1 modbus connection at port %2...").arg(name, port));
    emit infoMessage_(tr("Baud rate: %1").arg(baudRate));
    emit infoMessage_(tr("Mode: %1s%2").arg(dataBits).arg(stopBits));
    emit infoMessage_(tr("Read timeout: %1 ms").arg(timeout));

    modbus4qt::RtuClient* client = new modbus4qt::RtuClient(port, baudRate, dataBits, stopBits, parity, this);
    connect(client, SIGNAL(infoMessage(QString)), this, SLOT(processInfoMessage_(QString)));
    connect(client, SIGNAL(debugMessage(QString)), this, SLOT(processDebugMessage_(QString)));
    connect(client, SIGNAL(errorMessage(QString)), this, SLOT(processCriticalMessage_(QString)));

    client->setReadTimeOut(timeout);
    client->setWriteTimeOut(timeout);

    emit infoMessage_(tr("%1 modbus connection at port %2 set up successfully").arg(name, port));

    return client;
}

//-----------------------------------------------------------------------------

SerialPort*
DriverImpl::createSerialPortConnection_(const QString &name,
                                        const QString &port,
                                        QSerialPort::BaudRate baudRate,
                                        QSerialPort::DataBits dataBits,
                                        QSerialPort::StopBits stopBits,
                                        QSerialPort::Parity parity,
                                        QSerialPort::FlowControl flowControl)
{
    emit infoMessage_(tr("Creating %1 serial port connection at port %2...").
                      arg(name, port));

    emit infoMessage_(tr("Baud rate: %1").arg(baudRate));
    emit infoMessage_(tr("Mode: %1s%2").arg(dataBits).arg(stopBits));
    emit infoMessage_(tr("Parity: %1").arg(parity));
    emit infoMessage_(tr("Flow control: %1").arg(flowControl));

    SerialPort* serialPort = new SerialPort(this);
    serialPort->setPortName(port);

    if (!serialPort->open(QIODevice::ReadWrite))
    {
        emit criticalMessage(tr("Cannot open port %1. Reason: %2").
                             arg(name, serialPort->errorString()));
    }

    if (!serialPort->setBaudRate(baudRate))
    {
        emit criticalMessage(tr("Cannot setup baud rate for port %1. Reason: %2").
                             arg(name, serialPort->errorString()));
    }

    if (!serialPort->setDataBits(dataBits))
    {
        emit criticalMessage(tr("Cannot setup data bits for port %1. Reason: %2").
                             arg(name, serialPort->errorString()));
    }

    if (!serialPort->setStopBits(stopBits))
    {
        emit criticalMessage(tr("Cannot setup stop bits for port %1. Reason: %2").
                             arg(name, serialPort->errorString()));
    }

    if (!serialPort->setParity(parity))
    {
        emit criticalMessage(tr("Cannot setup parity for port %1. Reason: %2").
                             arg(name, serialPort->errorString()));
    }

    if (!serialPort->setFlowControl(flowControl))
    {
        emit criticalMessage(tr("Cannot setup flow control for port %1. Reason: %2").
                             arg(name, serialPort->errorString()));
    }

    connect(serialPort, SIGNAL(infoMessage(QString)), this, SLOT(processInfoMessage_(QString)));
    connect(serialPort, SIGNAL(debugMessage(QString)), this, SLOT(processDebugMessage_(QString)));
    connect(serialPort, SIGNAL(criticalMessage(QString)), this, SLOT(processCriticalMessage_(QString)));

    emit infoMessage_(tr("%1 serial port connection at port %2 set up successfully").arg(name, port));

    return serialPort;
}

//-----------------------------------------------------------------------------

void
DriverImpl::fillDefaultValues_()
{
    emit infoMessage_(tr("Filling default signal values..."));

    controlValues_.clear();
    viewValues_.clear();

    MetaSignalsJavaStyleIterator i(metaSignals_);
    while (i.hasNext())
    {
        i.next();

        QVariant value;

        if (i.value().valueType == MetaSignal::Real)
        {
            value = QVariant(i.value().defaultValue);
        }
        else if (i.value().valueType == MetaSignal::Integer)
        {
            value = QVariant(qint16(i.value().defaultValue));
        }
        else if (i.value().valueType == MetaSignal::Boolean)
        {
            value = QVariant(!(i.value().defaultValue == 0.0));
        }

        if (i.value().signalType == MetaSignal::Control)
        {
            controlValues_.insert(i.key(), value);
        }
        else if (i.value().signalType == MetaSignal::Observation)
        {
            viewValues_.insert(i.key(), value);
        }
    }
}

//-----------------------------------------------------------------------------

void
DriverImpl::initDevices_()
{
    devicesInited_ = true;
}

//-----------------------------------------------------------------------------

void
DriverImpl::initSignalNames_()
{
    emit infoMessage_(tr("Initiating signal names..."));

    controlSignalNames_.clear();
    viewSignalNames_.clear();

    MetaSignalsJavaStyleIterator i(metaSignals_);
    while (i.hasNext())
    {
        i.next();

        if (i.value().signalType == MetaSignal::Control)
        {
            controlSignalNames_.insert(i.key(), i.value().fieldName);
        }
        else if (i.value().signalType == MetaSignal::Observation)
        {
            viewSignalNames_.insert(i.key(), i.value().fieldName);
        }
    }
}

//-----------------------------------------------------------------------------

SignalValues
DriverImpl::controlValues() const
{
    return controlValues_;
}

//-----------------------------------------------------------------------------

QString
DriverImpl::description() const
{
    return driverInterface_->description();
}

//-----------------------------------------------------------------------------

QString
DriverImpl::errorString() const
{
    QString retVal = errorString_;
    errorString_ = "";
    return retVal;
}

//-----------------------------------------------------------------------------

const MetaSignals&
DriverImpl::metaSignals() const
{
    return metaSignals_;
}

//-----------------------------------------------------------------------------

QString
DriverImpl::name() const
{
    return driverInterface_->name();
}

//-----------------------------------------------------------------------------

void
DriverImpl::setLogger(elvactools::Logger* logger)
{
    logger_ = logger;

    ELVACTOOLS_LOGGER_SYSTEM_CONNECT(this, logger_)

    connect(this, SIGNAL(criticalMessage_(QString)), this, SLOT(processCriticalMessage_(QString)));
    connect(this, SIGNAL(infoMessage_(QString)), this, SLOT(processInfoMessage_(QString)));
    connect(this, SIGNAL(debugMessage_(QString)), this, SLOT(processDebugMessage_(QString)));

    emit infoMessage_(tr("Driver implementation connected with logger"));

}

//-----------------------------------------------------------------------------

void
DriverImpl::processCriticalMessage_(const QString message)
{
    QString fullMessage = "[" + driverInterface_->name() + "] " + message;
    emit criticalMessage(fullMessage);
}

//-----------------------------------------------------------------------------

void
DriverImpl::processInfoMessage_(const QString message)
{
    QString fullMessage = "[" + driverInterface_->name() + "] " + message;
    emit infoMessage(fullMessage);
}

//-----------------------------------------------------------------------------

void
DriverImpl::processDebugMessage_(const QString message)
{
    QString fullMessage = "[" + driverInterface_->name() + "] " + message;
    emit debugMessage(fullMessage);
}

//-----------------------------------------------------------------------------

void
DriverImpl::reset_()
{
    emit infoMessage_(tr("Equipment resetted!"));
    emit signalValuesResetted();
}

//-----------------------------------------------------------------------------

void
DriverImpl::start_()
{
    QString msg = tr("Starting reading values from gauges every %1 msec...");
    emit infoMessage_(msg.arg(readTimerInterval_));

    readTimer_.setInterval(readTimerInterval_);
    readTimer_.start();

    emit infoMessage_(tr("Reading values from gauges started"));
}

//-----------------------------------------------------------------------------

void
DriverImpl::stop_()
{
    readTimer_.stop();
}

//-----------------------------------------------------------------------------

SignalValues
DriverImpl::viewValues() const
{
    return viewValues_;
}

//-----------------------------------------------------------------------------

} // namespace Drivers
