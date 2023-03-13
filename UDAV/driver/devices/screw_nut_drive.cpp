#include "screw_nut_drive.h"

#include <QTimer>

#include <cmath>

#include "elvactools/utils.h"

#include "devices/state_event.h"
#include "devices/state_transition.h"
#include "screw_nut_drive_transition.h"

#include <elvacquip/onitex_osm.h>


//=============================================================================

ScrewNutDrive::ScrewNutDrive(OnitexOSM *&onitexOSM, double screwStep, double maxDistance, QObject *parent) :
    Device(parent),
    maxDistance_(maxDistance),
    screwStep_(screwStep),
    onitexOSM_(onitexOSM),
    timerInt_(500)
{
    driveCurrent_ = 1200;                 //!< Ток двигателя, мА
    driveStepRes_ = 16;                   //!< Дробление шага
    stepsForCircle_ = 200;                //!< Шагов на оборот

    fastStepFreq_ = 4000;                 //!< Частота шагов для быстрого движения по оси Z
    slowStepFreq_ = 2000;                 //!< Частота шагов для медленного движения по оси Z

    setScrewStep(screwStep_);

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
    success->setTargetState(zeroSearching_);

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
    {
    StateTransition *success = new StateTransition();
    success->setEventResult(StateEvent::SUCCEED);
    success->setTargetState(stopped_);

    StateTransition *fail = new StateTransition();
    fail->setEventResult(StateEvent::FAILED);
    fail->setTargetState(failed_);

    zeroSearching_->addTransition(success);
    zeroSearching_->addTransition(fail);
    }

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
ScrewNutDrive::checkMoving_()
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

    if (state == zeroSearching_)
    {
        bool atHome;
        bool ok = isAtHome(atHome);
        if (!ok)
        {
            machine_->postEvent(new StateEvent(state, StateEvent::FAILED));
        }
        else
        {
            if (atHome)
            {
                ok &= onitexOSM_->setZeroPosition();
                if (ok)
                {
                    emit infoMessage_(tr("Home switch founded"));
                    pos_ = 0.0;
                    nextPos_ = 0.0;
                    machine_->postEvent(new StateEvent(state, StateEvent::SUCCEED));
                }
                else
                    machine_->postEvent(new StateEvent(state, StateEvent::FAILED));
            }
        }
    }
    else if (state == initing_)
    {
        bool atHome;
        bool ok = isAtHome(atHome);
        if (!ok)
        {
            machine_->postEvent(new StateEvent(state, StateEvent::FAILED));
        }
        else
        {
            if (!atHome)
            {
                emit infoMessage_(tr("Home switch released! Going to search for home..."));
                ok &= onitexOSM_->stopMoving();
                if (ok)
                {
                    machine_->postEvent(new StateEvent(state, StateEvent::SUCCEED));
                }
                else
                    machine_->postEvent(new StateEvent(state, StateEvent::FAILED));
                inited_ = true;
            }
        }
    }
    else
    {
        quint32 counter;
        bool ok = onitexOSM_->readStepsCounter(&counter);

        if (!ok)
            machine_->postEvent(new StateEvent(state, StateEvent::FAILED));
        else if (!counter)
        {
            machine_->postEvent(new StateEvent(state, StateEvent::SUCCEED));
            pos_ = nextPos_;
            emit infoMessage_(tr("Moving finished. Target position: %1 mm").arg(pos_));
        }
    }
}

//-----------------------------------------------------------------------------

void
ScrewNutDrive::failedEntered_()
{
    timer_->stop();
    Device::failedEntered_();
}

//-----------------------------------------------------------------------------

void
ScrewNutDrive::initFinished_()
{
    timer_->stop();
    inited_ = true;
}

//-----------------------------------------------------------------------------

void
ScrewNutDrive::initStarted_()
{
    emit infoMessage_(tr("Initializing..."));

    bool ok = onitexOSM_->enable();
    ok &= onitexOSM_->setCurrent(driveCurrent_);
    ok &= onitexOSM_->setSleepCurrent(driveCurrent_ / 2);
    ok &= onitexOSM_->setMicroStep(driveStepRes_);
    ok &= onitexOSM_->setStartSpeed(fastStepFreq_);
    ok &= onitexOSM_->setSpeed(fastStepFreq_);
    ok &= onitexOSM_->setInterruptMode(OnitexOSM::Encoder);

    bool atHome;
    ok &= isAtHome(atHome);

    if (!ok)
    {
        machine_->postEvent(new StateEvent(initing_, StateEvent::FAILED));
    }
    else
    {
        if (!atHome)
            // Если площадка не в нулевой точке, то просто переходим в состояние поиска нуля
            machine_->postEvent(new StateEvent(initing_, StateEvent::SUCCEED));
        else
        {
            // Если площалка в нулевой точке, то едем в положительном направлении немного,
            // а потом переходим в состояние поиска нуля
            ok &= onitexOSM_->setDirection(POSITIVE);
            ok &= onitexOSM_->startMovinig(stepsForDistance(maxDistance_ * 0.5));

            timer_->start();
        }
    }
}

//-----------------------------------------------------------------------------

bool
ScrewNutDrive::isAtHome(bool &state)
{
    bool result = onitexOSM_->isAtHome(state);
    if (!result)
        emit criticalMessage_(tr("Cannot read home state!"));

    return result;
}

//-----------------------------------------------------------------------------

void
ScrewNutDrive::goHome()
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

    nextPos_ = 0.0;
    machine_->postEvent(new ScrewNutDriveCommand(stopped_, ScrewNutDriveCommand::MOVE));
}

//-----------------------------------------------------------------------------

void
ScrewNutDrive::moveNegative(double distance)
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

    //! @todo Задать макс границу движения ниже нуля!
//    if ((pos_ - distance) < 0.0)
//    {
//        emit criticalMessage_(tr("Max distance exceeded! Allowed: %1; requested: %2").arg(0.0).arg(pos_ - distance));
//        return;
//    }

    emit infoMessage_(tr("Device will be moved for %1 mm").arg(-distance));

    nextPos_ = pos_ - distance;
    machine_->postEvent(new ScrewNutDriveCommand(stopped_, ScrewNutDriveCommand::MOVE));
}

//-----------------------------------------------------------------------------

void
ScrewNutDrive::movePositive(double distance)
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

    if ((pos_ + distance) > maxDistance_)
    {
        emit criticalMessage_(tr("Max distance exceeded! Allowed: %1; requested: %2").arg(maxDistance_).arg(pos_ + distance));
        return;
    }

    emit infoMessage_(tr("Device will be moved for %1 mm").arg(distance));

    nextPos_ = pos_ + distance;
    machine_->postEvent(new ScrewNutDriveCommand(stopped_, ScrewNutDriveCommand::MOVE));
}

//-----------------------------------------------------------------------------

void
ScrewNutDrive::moveTo(double coord)
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

    if (coord > maxDistance_)
    {
        emit criticalMessage_(tr("Maximum position exceeded! Allowed: %1; requested: %2").arg(maxDistance_).arg(coord));
        return;
    }

    emit infoMessage_(tr("Device will be moved from position %1 to position %2 mm").
                      arg(pos_).
                      arg(coord));

    nextPos_ = coord;
    machine_->postEvent(new ScrewNutDriveCommand(stopped_, ScrewNutDriveCommand::MOVE));
}

//-----------------------------------------------------------------------------

void
ScrewNutDrive::movingStarted_()
{
    double dist = nextPos_ - pos_;
    quint8 dir;

    emit debugMessage_(tr("Moving started! Distance: %1 mm").arg(dist));

    if (dist > 0.0)
        dir = POSITIVE;
    else
        dir = NEGATIVE;

    quint32 steps = stepsForDistance(fabs(dist));
    if (steps == 0)
        machine_->postEvent(new StateEvent(moving_, StateEvent::SUCCEED));
    else
    {
        bool ok;
        ok = onitexOSM_->setDirection(dir);
        ok &= onitexOSM_->startMovinig(steps);

        if (ok)
            timer_->start();
        else
            machine_->postEvent(new StateEvent(moving_, StateEvent::FAILED));
    }
}

//-----------------------------------------------------------------------------

void
ScrewNutDrive::stop()
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

    onitexOSM_->stopMoving();
    machine_->postEvent(new ScrewNutDriveCommand(moving_, ScrewNutDriveCommand::STOP));
}

//-----------------------------------------------------------------------------

void
ScrewNutDrive::stoppedEntered_()
{
    timer_->stop();

    quint32 *tmp;
    tmp = (quint32*)&encPos_;

    bool ok = onitexOSM_->readPosition(tmp);

    if (!ok)
        machine_->postEvent(new StateEvent(stopped_, StateEvent::FAILED));
    else
    {
        pos_ = double(encPos_) / double(stepsForMm_);
        nextPos_ = pos_;
        emit infoMessage_(tr("Device is stopped at position %1 mm and waiting for commands").arg(pos_));
        emit movingDone();
    }
}

//-----------------------------------------------------------------------------

void
ScrewNutDrive::zeroSearchStarted_()
{
    emit infoMessage_(tr("Searching for home switch..."));

    // Если площадка не в нулевой точке, то просто ищем ноль
    bool ok = onitexOSM_->setDirection(NEGATIVE);

    ok &= onitexOSM_->findHome();
    if (!ok)
        machine_->postEvent(new StateEvent(zeroSearching_, StateEvent::FAILED));
    else
        timer_->start();
}





