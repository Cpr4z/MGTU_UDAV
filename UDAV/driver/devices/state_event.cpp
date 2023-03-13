#include "state_event.h"

const QEvent::Type &StateEvent::event_type = static_cast<QEvent::Type>(QEvent::registerEventType());

StateEvent::StateEvent(QState *state, Result result)
    : QEvent(event_type), state_(state), result_(result)
{

}

