#include "device.h"

#include <QStateMachine>

Device::Device(QObject *parent) :
    QObject(parent),
    inited_(false)
{
    machine_ = new QStateMachine(this);

    failed_ = new QState(machine_);
    connect(failed_, SIGNAL(entered()), this, SLOT(failedEntered_()));

    connect(this, SIGNAL(criticalMessage_(QString)), this, SLOT(processCriticalMessage_(QString)));
    connect(this, SIGNAL(debugMessage_(QString)), this, SLOT(processDebugMessage_(QString)));
    connect(this, SIGNAL(infoMessage_(QString)), this, SLOT(processInfoMessage_(QString)));
}

//-----------------------------------------------------------------------------

Device::~Device()
{
    delete machine_;
}

//-----------------------------------------------------------------------------

void
Device::failedEntered_()
{
    emit errorMessage();
}

//-----------------------------------------------------------------------------

void Device::run()
{
    machine_->start();
}


