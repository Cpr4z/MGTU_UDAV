#include "state_transition.h"

bool StateTransition::eventTest(QEvent *event)
{
    if (event->type() == StateEvent::event_type)
    {
        StateEvent *state_event = static_cast<StateEvent*>(event);

        return (state_event->state() == sourceState() && state_event->result() == result_);
    }

    return false;
}
