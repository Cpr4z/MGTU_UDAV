#include "multi_axe_screw_nut_drive.h"

#include <QTimer>

#include "core/core.h"

#include "elvactools/math/math.h"

#include "devices/state_event.h"
#include "devices/state_transition.h"
#include "screw_nut_drive_transition.h"

#include "elvacquip/phytron_mcc2.h"

//=============================================================================

MultiAxeScrewNutDrive::MultiAxeScrewNutDrive(PhytronMCC2 *&pytronMCC2, double screwStep, double maxDistance, QObject *parent) :
    Device(parent),
    phytronMCC2_(pytronMCC2),
    timerInt_(500)
{
    driveCurrent_[AXE_X] = 1200;             //!< Ток двигателя, мА
    driveStepRes_[AXE_X] = 1;              //!< Дробление шага
    stepsForCircle_[AXE_X] = 5000;     //!< Шагов на оборот

    fastStepFreq_[AXE_X] = 4000;                 //!< Частота шагов для быстрого движения по оси Z
    slowStepFreq_[AXE_X] = 2000;                 //!< Частота шагов для медленного движения по оси Z

    screwStep_[AXE_X] = 5; //X_SCREW_STEP_MM

    driveCurrent_[AXE_Y] = 1200;             //!< Ток двигателя, мА

    driveStepRes_[AXE_Y] = 8;              //!< Дробление шага
    stepsForCircle_[AXE_Y] = 200;     //!< Шагов на оборот

    fastStepFreq_[AXE_Y] = 4000;                 //!< Частота шагов для быстрого движения по оси Z
    slowStepFreq_[AXE_Y] = 2000;                 //!< Частота шагов для медленного движения по оси Z

    screwStep_[AXE_Y] = 0.5; //Y_SCREW_STEP_MM

    setScrewStep(screwStep_[AXE_X], AXE_X);
    setScrewStep(screwStep_[AXE_Y], AXE_Y);

    timer_ = new QTimer(this);
    timer_->setInterval(timerInt_);
    connect(timer_, SIGNAL(timeout()), this, SLOT(checkMoving_()));

    //===========================
    // >> Состояния

    //---------------------------
    // Инициализация

    initing_ = new QState(machine_);
    connect(initing_, SIGNAL(entered()), this, SLOT(initStarted_()));
    connect(initing_, SIGNAL(exited()), this, SLOT(initFinished_()));

    //---------------------------
    // Поиск нуля

    zeroSearching_  = new QState(machine_);
    connect(zeroSearching_, SIGNAL(entered()), this, SLOT(zeroSearchStarted_()));

    //---------------------------
    // Остановка

    stopped_ = new QState(machine_);
    connect(stopped_, SIGNAL(entered()), this, SLOT(stoppedEntered_()));

    //---------------------------
    // Движение

    moving_ = new QState(machine_);
    connect(moving_, SIGNAL(entered()), this, SLOT(movingStarted_()));

    // << Состояния
    //===========================

    //===========================
    // >> Переходы между состояниями (ЕOF)
    //---------------------------
    // Состояние: Инициализация
    //
    // В случае успешной инициализации переход в состояние поиска нуля
    // В случае сбоя переход в состояние FAILED
    {
    StateTransition *success = new StateTransition();
    success->setEventResult(StateEvent::SUCCEED);
//    success->setTargetState(zeroSearching_);
    success->setTargetState(stopped_);

    StateTransition *fail = new StateTransition();
    fail->setEventResult(StateEvent::FAILED);
    fail->setTargetState(failed_);

    initing_->addTransition(success);
    initing_->addTransition(fail);
    }

    //----------------------------
    // Состояние: Поиск нуля
    //
    // В случае успешного поиска нуля переход в состояние stopped с нулевой координатой
    // В случае сбоя переход в состояние FAILED
//    {
//    StateTransition *success = new StateTransition();
//    success->setEventResult(StateEvent::SUCCEED);
//    success->setTargetState(stopped_);

//    StateTransition *fail = new StateTransition();
//    fail->setEventResult(StateEvent::FAILED);
//    fail->setTargetState(failed_);

//    zeroSearching_->addTransition(success);
//    zeroSearching_->addTransition(fail);
//    }

    //----------------------------
    // Состояние: Остановка
    //
    // В случае получения команды переход в состояние движения к следующей координате
    {
    ScrewNutDriveTransition *move = new ScrewNutDriveTransition();
    move->setEventResult(ScrewNutDriveCommand::MOVE);
    move->setTargetState(moving_);

    stopped_->addTransition(move);
    }

    //----------------------------
    // Состояние: Движение
    //
    // В случае успеного выполнения переход в состояние остановки
    // В случае сбоя переход в состояние FAILED
    {
    StateTransition *success = new StateTransition();
    success->setEventResult(StateEvent::SUCCEED);
    success->setTargetState(stopped_);

    StateTransition *fail = new StateTransition();
    fail->setEventResult(StateEvent::FAILED);
    fail->setTargetState(failed_);

    ScrewNutDriveTransition *stop = new ScrewNutDriveTransition();
    stop->setEventResult(ScrewNutDriveCommand::STOP);
    stop->setTargetState(stopped_);

    moving_->addTransition(success);
    moving_->addTransition(fail);
    moving_->addTransition(stop);
    }

    // Переходы между состояниями (ЕOF)
    //============================

    machine_->setInitialState(initing_);

}

//-----------------------------------------------------------------------------

void
MultiAxeScrewNutDrive::setScrewStep(double screwStep, Axes axe)
{
    screwStep_[axe] = screwStep;

    if (screwStep_[axe] != 0.0)
    {
        stepsForMm_[axe] = double(stepsForCircle_[axe]) * double(driveStepRes_[axe]) / screwStep_[axe];
        qDebug() << tr("Axe: %1, steps for mm: %2").arg(axe).arg(stepsForMm_[axe]);
    }
    else
        stepsForMm_[axe]  = 0;
}

//-----------------------------------------------------------------------------

void
MultiAxeScrewNutDrive::checkMoving_()
{
    QState* state = NULL;
    if (machine_->configuration().contains(initing_))
        state = initing_;
    else if (machine_->configuration().contains(zeroSearching_))
        state = zeroSearching_;
    else if (machine_->configuration().contains(moving_))
        state = moving_;
    else
        return;

//    if (state == zeroSearching_)
//    {
//        bool atHome;
//        bool ok = isAtHome(atHome);
//        if (!ok)
//        {
//            machine_->postEvent(new StateEvent(state, StateEvent::FAILED));
//        }
//        else
//        {
//            if (atHome)
//            {
//                ok &= phytronMCC2_->setZeroPosition();
//                if (ok)
//                {
//                    emit infoMessage_(tr("Home switch founded"));
//                    pos_ = 0.0;
//                    nextPos_ = 0.0;
//                    machine_->postEvent(new StateEvent(state, StateEvent::SUCCEED));
//                }
//                else
//                    machine_->postEvent(new StateEvent(state, StateEvent::FAILED));
//            }
//        }
//    }
//    else if (state == initing_)
//    {
//        bool atHome;
//        bool ok = isAtHome(atHome);
//        if (!ok)
//        {
//            machine_->postEvent(new StateEvent(state, StateEvent::FAILED));
//        }
//        else
//        {
//            if (!atHome)
//            {
//                emit infoMessage_(tr("Home switch released succefully! Going to search for home..."));
//                ok &= onitexOSM_->stopMoving();
//                if (ok)
//                {
//                    machine_->postEvent(new StateEvent(state, StateEvent::SUCCEED));
//                }
//                else
//                    machine_->postEvent(new StateEvent(state, StateEvent::FAILED));
//                inited_ = true;
//            }
//        }
//    }
//    else
//    {
        qint32 counterX, counterY;
        bool ok;
        ok = phytronMCC2_->curXCoord(&counterX);
        ok &= phytronMCC2_->curYCoord(&counterY);
//        bool ok = onitexOSM_->readStepsCounter(&counter);
        if (!ok)
            machine_->postEvent(new StateEvent(state, StateEvent::FAILED));
        else //if (!counter)
        {
            if ((almostEqual(double(counterX) / double(stepsForMm(AXE_X)), nextPos_[AXE_X], 0.5) &&
                    (almostEqual(double(counterY) / double(stepsForMm(AXE_Y)), nextPos_[AXE_Y], 0.5))))
            {
                machine_->postEvent(new StateEvent(state, StateEvent::SUCCEED));
                pos_[AXE_X] = double(counterX) / double(stepsForMm(AXE_X)); //nextPos_[AXE_X];
                pos_[AXE_Y] = double(counterY) / double(stepsForMm(AXE_Y)); //nextPos_[AXE_Y];
                emit infoMessage_(tr("Moving finished. Real position: [%1 x %2] mm").arg(pos_[AXE_X]).arg(pos_[AXE_Y]));

            }
            qDebug() << "X checked: " << double(counterX) / double(stepsForMm(AXE_X)) << " != " << nextPos_[AXE_X];
            qDebug() << "Y checked: " << double(counterY) / double(stepsForMm(AXE_Y)) << " != " << nextPos_[AXE_Y];
        }
//    }
}

//-----------------------------------------------------------------------------

void
MultiAxeScrewNutDrive::failedEntered_()
{
    timer_->stop();
    Device::failedEntered_();
}

//-----------------------------------------------------------------------------

void
MultiAxeScrewNutDrive::initFinished_()
{
    timer_->stop();
    inited_ = true;
}

//-----------------------------------------------------------------------------

void
MultiAxeScrewNutDrive::initStarted_()
{
    emit infoMessage_(tr("Initializing..."));

    bool ok;

//    bool ok = onitexOSM_->enable();
//    ok &= onitexOSM_->setCurrent(driveCurrent_);
//    ok &= onitexOSM_->setSleepCurrent(driveCurrent_ / 2);
//    ok &= onitexOSM_->setMicroStep(driveStepRes_);
//    ok &= onitexOSM_->setStartSpeed(fastStepFreq_);
//    ok &= onitexOSM_->setSpeed(fastStepFreq_);
//    ok &= onitexOSM_->setInterruptMode(OnitexOSM::Encoder);

    ok = phytronMCC2_->resetX();
    ok &= phytronMCC2_->resetY();

    ok &= phytronMCC2_->setMovementType(PhytronMCC2::X, PhytronMCC2::Linear);
    ok &= phytronMCC2_->setUnits(PhytronMCC2::X, PhytronMCC2::Step);
    ok &= phytronMCC2_->setPositiveLimitSwitchCheck(PhytronMCC2::X, false);
    ok &= phytronMCC2_->setNegativeLimitSwitchCheck(PhytronMCC2::X, false);

    ok &= phytronMCC2_->setMovementType(PhytronMCC2::Y, PhytronMCC2::Linear);
    ok &= phytronMCC2_->setUnits(PhytronMCC2::Y, PhytronMCC2::Step);
    ok &= phytronMCC2_->setPositiveLimitSwitchCheck(PhytronMCC2::Y, false);
    ok &= phytronMCC2_->setNegativeLimitSwitchCheck(PhytronMCC2::Y, false);

    ok &= phytronMCC2_->setStepResolution(PhytronMCC2::X, PhytronMCC2::FullStep); //X_STEP_RES
    ok &= phytronMCC2_->setStepResolution(PhytronMCC2::Y, PhytronMCC2::Step_1_8); //Y_STEP_RES

    ok &= phytronMCC2_->setRunFrequency(PhytronMCC2::X, 12000); //X_FAST_STEP);
    ok &= phytronMCC2_->setRunFrequency(PhytronMCC2::Y, 12000); //Y_FAST_STEP);

    PhytronMCC2::ExtendedAxeState stateX, stateY;
    ok &= phytronMCC2_->readXYState(stateX, stateY);

    if (!ok)
    {
        machine_->postEvent(new StateEvent(initing_, StateEvent::FAILED));
    }
    else
        machine_->postEvent(new StateEvent(initing_, StateEvent::SUCCEED));

    pos_[AXE_X] = 0.0;
    pos_[AXE_Y] = 0.0;
    nextPos_[AXE_X] = 0.0;
    nextPos_[AXE_Y] = 0.0;
    encPos_[AXE_X] = 0;
    encPos_[AXE_Y] = 0;

    ok &= phytronMCC2_->moveToEndX();
    ok &= phytronMCC2_->moveToEndY();

//    bool atHome;
//    ok &= isAtHome(atHome);

//    if (!ok)
//    {
//        machine_->postEvent(new StateEvent(initing_, StateEvent::FAILED));
//    }
//    else
//    {
//        if (!atHome)
//            // Если площадка не в нулевой точке, то просто переходим в состояние поиска нуля
//            machine_->postEvent(new StateEvent(initing_, StateEvent::SUCCEED));
//        else
//        {
//            // Если площалка в нулевой точке, то едем в положительном направлении немного,
//            // а потом переходим в состояние поиска нуля
////            ok &= onitexOSM_->setDirection(POSITIVE);
////            ok &= onitexOSM_->startMovinig(stepsForDistance(maxDistance_ * 0.5));

//            timer_->start();
//        }
//    }
}

//-----------------------------------------------------------------------------

bool
MultiAxeScrewNutDrive::isAtHome(bool &state, Axes axe)
{
    bool result = true;
//    bool result = onitexOSM_->isAtHome(state);
//    if (!result)
//        emit criticalMessage_(tr("Cannot read home state!"));

    state = (encPos_[axe] == 0);

    return result;
}

//-----------------------------------------------------------------------------

void
MultiAxeScrewNutDrive::goHome()
{
    if (!inited_)
    {
        emit criticalMessage_(tr("Device is not inited!"));
        return;
    }

    if (!machine_->configuration().contains(stopped_))
    {
        emit criticalMessage_(tr("Moving commands allowed only at stopped state!"));
        return;
    }

    emit infoMessage_(tr("Will be going home!"));

    nextPos_[AXE_X] = 0.0;
    nextPos_[AXE_Y] = 0.0;
    machine_->postEvent(new ScrewNutDriveCommand(stopped_, ScrewNutDriveCommand::MOVE));
}

//-----------------------------------------------------------------------------

void
MultiAxeScrewNutDrive::moveNegative(double distance, MultiAxeScrewNutDrive::Axes axe)
{
    if (!inited_)
    {
        emit criticalMessage_(tr("Device is not inited!"));
        return;
    }

    if (!machine_->configuration().contains(stopped_))
    {
        emit criticalMessage_(tr("Moving commands allowed only at stopped state!"));
        return;
    }

    if ((pos_[axe] - distance) < 0.0)
    {
        emit criticalMessage_(tr("Max distance exceeded! Allowed: %1; requested: %2").arg(0.0).arg(pos_[axe] - distance));
        return;
    }

    nextPos_[axe] = pos_[axe] - distance;
    machine_->postEvent(new ScrewNutDriveCommand(stopped_, ScrewNutDriveCommand::MOVE));
}

//-----------------------------------------------------------------------------

void
MultiAxeScrewNutDrive::movePositive(double distance, MultiAxeScrewNutDrive::Axes axe)
{
    if (!inited_)
    {
        emit criticalMessage_(tr("Device is not inited!"));
        return;
    }

    if (!machine_->configuration().contains(stopped_))
    {
        emit criticalMessage_(tr("Moving commands allowed only at stopped state!"));
        return;
    }

    if ((pos_[axe] + distance) > maxDistance_[axe])
    {
        emit criticalMessage_(tr("Max distance exceeded! Allowed: %1; requested: %2").arg(maxDistance_[axe]).arg(pos_[axe] + distance));
        return;
    }

    nextPos_[axe] = pos_[axe] + distance;
    machine_->postEvent(new ScrewNutDriveCommand(stopped_, ScrewNutDriveCommand::MOVE));
}

//-----------------------------------------------------------------------------

void
MultiAxeScrewNutDrive::moveTo(double x, double y)
{
    if (!machine_->configuration().contains(stopped_))
    {
        emit criticalMessage_(tr("Moving commands allowed only at stopped state!"));
        return;
    }

    nextPos_[AXE_X] = x;
    nextPos_[AXE_Y] = y;

    machine_->postEvent(new ScrewNutDriveCommand(stopped_, ScrewNutDriveCommand::MOVE));
}

//-----------------------------------------------------------------------------

void
MultiAxeScrewNutDrive::moveTo(double coord, MultiAxeScrewNutDrive::Axes axe)
{
    if (!machine_->configuration().contains(stopped_))
    {
        emit criticalMessage_(tr("Moving commands allowed only at stopped state!"));
        return;
    }

    nextPos_[axe] = coord;
    machine_->postEvent(new ScrewNutDriveCommand(stopped_, ScrewNutDriveCommand::MOVE));
}

//-----------------------------------------------------------------------------

void
MultiAxeScrewNutDrive::movingStarted_()
{
    bool ok;
    qint32 counterX, counterY;
    ok = phytronMCC2_->curXCoord(&counterX);
    ok &= phytronMCC2_->curYCoord(&counterY);
//        bool ok = onitexOSM_->readStepsCounter(&counter);
    if (!ok)
        machine_->postEvent(new StateEvent(stopped_, StateEvent::FAILED));
    else //if (!counter)
    {
        pos_[AXE_X] = double(counterX) / double(stepsForMm(AXE_X));
        pos_[AXE_Y] = double(counterY) / double(stepsForMm(AXE_Y));
    }

    double dist[MAX_AXES];
    dist[AXE_X] = nextPos_[AXE_X] - pos_[AXE_X];
    dist[AXE_Y] = nextPos_[AXE_Y] - pos_[AXE_Y];

    emit debugMessage_(tr("Moving started! Distance: [%1 x %2] mm (value below zero is right direction)").
                       arg(dist[AXE_X]).
                       arg(dist[AXE_Y]));
    emit debugMessage_(tr("Target posistion: [%1 x %2] mm").
                       arg(nextPos_[AXE_X]).
                       arg(nextPos_[AXE_Y]));

//    quint8 dirX, dirY;
//    if (dist[AXE_X] > 0.0)
//        dirX = POSITIVE;
//    else
//        dirX = NEGATIVE;

//    if (dist[AXE_Y] > 0.0)
//        dirY = POSITIVE;
//    else
//        dirY = NEGATIVE;


    ok &= phytronMCC2_->setRunFrequency(PhytronMCC2::X, 12000); //X_FAST_STEP);
    ok = phytronMCC2_->moveToAbsolutePositionX(stepsForDistance(nextPos_[AXE_X], AXE_X));


    ok &= phytronMCC2_->setRunFrequency(PhytronMCC2::Y, 12000); //Y_FAST_STEP);
    ok &= phytronMCC2_->moveToAbsolutePositionY(stepsForDistance(nextPos_[AXE_Y], AXE_Y));

//    ok = onitexOSM_->setDirection(dir);
//    ok &= onitexOSM_->startMovinig(stepsForDistance(dist));

    if (ok)
        timer_->start();
    else
        machine_->postEvent(new StateEvent(moving_, StateEvent::FAILED));
}

//-----------------------------------------------------------------------------

void
MultiAxeScrewNutDrive::stop()
{
    if (!inited_)
    {
        emit criticalMessage_(tr("Device is not inited!"));
        return;
    }

    if (!machine_->configuration().contains(moving_))
    {
        emit criticalMessage_(tr("Stop command allowed only at moving state!"));
        return;
    }

    bool ok;
    ok = phytronMCC2_->stopX();
    ok &= phytronMCC2_->stopY();

    if (ok)
        machine_->postEvent(new ScrewNutDriveCommand(moving_, ScrewNutDriveCommand::STOP));
    else
        machine_->postEvent(new StateEvent(moving_, StateEvent::FAILED));
//    onitexOSM_->stopMoving();
//    machine_->postEvent(new ScrewNutDriveCommand(moving_, ScrewNutDriveCommand::STOP));
}

//-----------------------------------------------------------------------------

void
MultiAxeScrewNutDrive::stoppedEntered_()
{
    timer_->stop();
//    bool ok = onitexOSM_->readPosition(&encPos_);

//    if (!ok)
//        machine_->postEvent(new StateEvent(stopped_, StateEvent::FAILED));
//    else
//    {
//        pos_ = double(encPos_) / double(stepsForMm_);
//        nextPos_ = pos_;

    qint32 counterX, counterY;
    bool ok;
    ok = phytronMCC2_->curXCoord(&counterX);
    ok &= phytronMCC2_->curYCoord(&counterY);
//        bool ok = onitexOSM_->readStepsCounter(&counter);
    if (!ok)
        machine_->postEvent(new StateEvent(stopped_, StateEvent::FAILED));
    else //if (!counter)
    {
        pos_[AXE_X] = double(counterX) / double(stepsForMm(AXE_X));
        pos_[AXE_Y] = double(counterY) / double(stepsForMm(AXE_Y));
    }
    emit debugMessage_(tr("Stopped at pos [%1 x %2] mm and waiting for commands").arg(pos_[AXE_X]).arg(pos_[AXE_Y]));
    emit movingDone();

//    }
}

//-----------------------------------------------------------------------------

void
MultiAxeScrewNutDrive::zeroSearchStarted_()
{
    emit infoMessage_(tr("Searching for home switch..."));

    // Если площадка не в нулевой точке, то просто ищем ноль
    bool ok = true;
//    bool ok = onitexOSM_->setDirection(NEGATIVE);

//    ok &= onitexOSM_->findHome();
    if (!ok)
        machine_->postEvent(new StateEvent(zeroSearching_, StateEvent::FAILED));
    else
        timer_->start();
}





