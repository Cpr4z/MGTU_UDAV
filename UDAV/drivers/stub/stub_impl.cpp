#include "stub_impl.h"
#include "stub.h"

const QString DriverVersion = "0.0";

// Step 1. Select name for a new driver
//
// Rename classes and files, replace Stub with selected name.

namespace udav_drivers {

//-----------------------------------------------------------------------------

StubImpl::StubImpl(const MetaSignals& metaSignals,
        const DriverSettings& settings,
        DriverInterface* driverInterface,
        bool simulate,
        elvactools::Logger* logger) :
    DriverImpl(metaSignals, settings, driverInterface, simulate, logger)
{
    emit infoMessage_(tr("Creating driver implementation..."));

    //
    // Step 6. Create connections with real equipment.
    //

//    tic_ = new Tic(driverSettings_[Lab311Conf::TIC_port].toString(), this);
//    connect(tic_, SIGNAL(infoMessage(QString)), this, SLOT(driverInfoMessage(QString)));
//    connect(tic_, SIGNAL(debugMessage(QString)), this, SLOT(driverDebugMessage(QString)));
//    connect(tic_, SIGNAL(criticalMessage(QString)), this, SLOT(driverCriticalMessage(QString)));

//    tic_->setPressureUnit(Tic::mbar);
//    tic_->setQueueTimerInterval(settings_[Lab311Conf::TIC_queueTimerInterval].toInt());
//    tic_->setRequestTimerInterval(settings_[Lab311Conf::TIC_requestTimerInterval].toInt());
//    tic_->setResponseTimeOut(settings_[Lab311Conf::TIC_responseTimeOut].toInt());

//    tic_.addBackingPump2System(true, true);
//    tic_.addTurboPump2System(true, true);
//    tic_.setTurboPumpVentOption(Tic::ControlledVent);

//    tic_->openConnection();

    // Start reading values from gauges
    //
    //start();
}

//-----------------------------------------------------------------------------

//
// Step 7. Switch all devaices in safe state
//

//! @todo It'll be a error if something goes wrong. We should have opportunity to make it safe in this case.

StubImpl::~StubImpl()
{
//    closeTurboValve();
//    closeRutsValve();
    //    closeAtmInletValve();
}

//-----------------------------------------------------------------------------

//
// Step 8. Init settings for real devices with values readed into driverSettings_ struct.
//

bool
StubImpl::initSettings_(QUuid messageWindow)
{
    bool result = true;
    bool ok = true;

    QString errorMessage;

    emit infoMessage_(tr("Reading driver settings..."));

//    settings_.ticBusPort = driverSettings_[Lab311Conf::TicBus_port].toString();

//    int iBaudRate = driverSettings_[Lab311Conf::TicBus_baud_rate].toInt(&ok);
//    if (ok) settings_.ticBusBaudRate = SerialPort::int2BaudRate(iBaudRate, &ok);
//    if (!ok)
//    {
//        errorMessage = tr("Error in baud rate setring for TERLA serial line! (%1)").arg(Lab311Conf::TicBus_baud_rate);

//        emit criticalMessage_(errorMessage);
//        emit sendTextMessage2Window(messageWindow, Core::MessageType::Critical, errorMessage);
//    }
//    result &= ok;

    return result;
}

//-----------------------------------------------------------------------------

//
// Step 9. Read and return description of user interface
//

QString
StubImpl::userInterface() const
{
    QFile f("drivers/stub-ui.xml");
    if (f.open(QFile::ReadOnly))
    {
        QTextStream s(&f);
        s.setCodec("UTF-8");

        QString retVal = s.readAll();
        return retVal;
    }
    else
    {
        emit criticalMessage(tr("Cannot read ui file from resource!"));
        return QString();
    }
}

//-----------------------------------------------------------------------------

//
// Step 10. Setup equipment and put it in intial state.
//

bool
StubImpl::initEquipment_()
{
    emit infoMessage_(tr("Initializing %1").arg(DriverVersion));

    bool ok = true;
    bool result = true;

    QUuid msgWinId(QUuid::createUuid());

    //--------------------------------------------------------
    // Read settings. If something goes wrong nothing to do :-(

    ok = initSettings_(msgWinId);
    if (!ok)
    {
        return false;
    }
}

//-----------------------------------------------------------------------------

//bool
//StubImpl::isOff() const
//{
//    return false;
//}

//-----------------------------------------------------------------------------

//
// Step 11. Read values for control signals from equipment.
//

void
StubImpl::readControlValues_()
{

}

//-----------------------------------------------------------------------------

//
// Step 11. Read values for view signals from equipment.
//

void
StubImpl::readViewValues_()
{
    bool result;

//    result = tic_->readGauge1(signalValues_.p1_ );
//    viewValues_[Lab311::VS_VacuumGauge1] = signalValues_.p1_;

//    result = tic_->readGauge2(signalValues_.p2_);
//    viewValues_[Lab311::VS_VacuumGauge2] = signalValues_.p2_;

//    result = tic_->readGauge3(signalValues_.p3_);
//    viewValues_[Lab311::VS_VacuumGauge3] = signalValues_.p3_;

    emit viewSignalsReaded();
}

//-----------------------------------------------------------------------------

//
// Step 11. Send control signal value to equipment.
//

int
StubImpl::setControlSignal(SignalID signalID, bool value)
{
//    switch (signalID)
//    {
//        case Lab311::CS_OpenAtmInletValve :
//        {
//            if (value)
//                openAtmInletValve();
//            else
//                closeAtmInletValve();
//        }
//        break;
//        case Lab311::CS_OpenRutsValve :
//        {
//            if (value)
//                openRutsValve();
//            else
//                closeRutsValve();
//        }
//        break;
//        case Lab311::CS_OpenTurboValve :
//        {
//            if (value)
//                openTurboValve();
//            else
//                closeTurboValve();
//        }
//        break;
//    }
//    return CommandOk;
//    return CommandError;
}

//-----------------------------------------------------------------------------

}
