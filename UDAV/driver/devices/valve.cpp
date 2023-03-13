#include "valve.h"

#include <QTimer>
#include <QDebug>

#include "state_event.h"
#include "state_transition.h"

//-----------------------------------------------------------------------------

Valve::Valve(QObject *parent) :
    Device(parent),
    close_func_(NULL),
    open_func_(NULL),
    is_closed_func_(NULL),
    is_opened_func_(NULL)
{
    //===========================
    // Таймеры

    closingTimer_ = new QTimer(this);
    closingTimer_->setInterval(500);
    connect(closingTimer_, SIGNAL(timeout()), this, SLOT(checkClosing_()));

    openingTimer_ = new QTimer(this);
    openingTimer_->setInterval(500);
    connect(openingTimer_, SIGNAL(timeout()), this, SLOT(checkOpening_()));

    //===========================
    // Состояния

    //---------------------------
    // Состояние: Открытие клапана/затвора

    opening_ = new QState(machine_);
    connect(opening_, SIGNAL(entered()), this, SLOT(open_()));

    //---------------------------
    // Состояние: Закрытие клапана/затвора

    closing_ = new QState(machine_);
    connect(closing_, SIGNAL(entered()), this, SLOT(close_()));
    connect(closing_, SIGNAL(entered()), this, SLOT(checkClosing_()));

    //---------------------------
    // Состояние: Клапан/затвор открыт

    opened_ = new QState(machine_);
    connect(opened_, SIGNAL(entered()), this, SIGNAL(opened()));

    //---------------------------
    // Состояние: Клапан/затвор закрыт

    closed_ = new QState(machine_);
    connect(closed_, SIGNAL(entered()), this, SIGNAL(closed()));

    //============================
    // Переходы

    //----------------------------
    // Состояние: Открыт
    {

    }

    //----------------------------
    // Состояние: Закрыт
    {

    }

    //----------------------------
    // Состояние: Открытие клапана/затвора
    {
    StateTransition *openingSuccess = new StateTransition();
    openingSuccess->setEventResult(StateEvent::SUCCEED);
    openingSuccess->setTargetState(opened_);

    StateTransition *openingFail = new StateTransition();
    openingFail->setEventResult(StateEvent::FAILED);
    openingFail->setTargetState(failed_);

    opening_->addTransition(openingSuccess);
    opening_->addTransition(openingFail);
    }
    //----------------------------
    // Состояние: Закрытие клапана/затвора
    {
    StateTransition *closingSuccess = new StateTransition();
    closingSuccess->setEventResult(StateEvent::SUCCEED);
    closingSuccess->setTargetState(closed_);

    StateTransition *closingFail = new StateTransition();
    closingFail->setEventResult(StateEvent::FAILED);
    closingFail->setTargetState(failed_);

    closing_->addTransition(closingSuccess);
    closing_->addTransition(closingFail);
    }

    machine_->setInitialState(closed_);
}

//-----------------------------------------------------------------------------

Valve::~Valve()
{
}

//-----------------------------------------------------------------------------

void
Valve::close_()
{
    if (!close_func_) return;

    emit infoMessage_(tr("Valve will be close!"));
    close_func_();

    closingTimer_->start();
}

//-----------------------------------------------------------------------------

void
Valve::open_()
{
    if (!open_func_) return;

    emit infoMessage_(tr("Valve will be open!"));
    open_func_();

    openingTimer_->start();
}

//-----------------------------------------------------------------------------

void
Valve::checkClosing_()
{
    Q_ASSERT(!is_closed_func_);

    bool result = false;
    bool isClosed;

    result = is_closed_func_(isClosed);
    if (!result)
    {
        machine_->postEvent(new StateEvent(closing_, StateEvent::FAILED));
        closingTimer_->stop();

        return;
    }

    if (isClosed)
    {
        machine_->postEvent(new StateEvent(closing_, StateEvent::SUCCEED));
        closingTimer_->stop();

        emit infoMessage_(tr("Valve closed!"));
        emit closed();

        return;
    }
}

//-----------------------------------------------------------------------------

void
Valve::checkOpening_()
{
    Q_ASSERT(!is_opened_func_);

    bool result = false;
    bool isOpened;

    result = is_opened_func_(isOpened);
    if (!result)
    {
        machine_->postEvent(new StateEvent(opening_, StateEvent::FAILED));
        openingTimer_->stop();

        return;
    }

    if (isOpened)
    {
        machine_->postEvent(new StateEvent(opening_, StateEvent::SUCCEED));
        openingTimer_->stop();

        emit infoMessage_(tr("Valve opened!"));
        emit opened();

        return;
    }
}

//-----------------------------------------------------------------------------

void
Valve::installCloseFunc(QObject *sender, const char *signal, bool (*close_func)())
{
    close_func_ = close_func;
    opened_->addTransition(sender, signal, closing_);
}

//-----------------------------------------------------------------------------

void
Valve::installIsClosedFunc(bool (*is_closed_func)(bool&))
{
    is_closed_func_ = is_closed_func;
}

//-----------------------------------------------------------------------------

void
Valve::installOpenFunc(QObject *sender, const char *signal, bool (*open_func)())
{
    open_func_ = open_func;
    closed_->addTransition(sender, signal, opening_);
}

//-----------------------------------------------------------------------------

void
Valve::installIsOpenedFunc(bool (*is_opend_func)(bool&))
{
    is_opened_func_ = is_opend_func;
}

//-----------------------------------------------------------------------------

void
Valve::close()
{

}

//-----------------------------------------------------------------------------
void
Valve::open()
{

}

//-----------------------------------------------------------------------------

//bool
//Valve::isOpened() const
//{
//    if (!is_opened_func_)
//        return false;

//    bool result;
//    bool ok = is_opened_func_(result);

//    if (ok)
//        return result;
//    else
//        return false;
//}

////-----------------------------------------------------------------------------

//bool
//Valve::isClosed() const
//{
//    if (!is_closed_func_)
//        return false;

//    bool result;
//    bool ok = is_closed_func_(result);

//    if (ok)
//        return result;
//    else
//        return false;
//}

//-----------------------------------------------------------------------------

