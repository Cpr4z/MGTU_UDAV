#ifndef STATE_TRANSITION
#define STATE_TRANSITION

#include <QAbstractTransition>

#include "state_event.h"

class StateTransition : public QAbstractTransition
{
    Q_OBJECT
private:
    StateEvent::Result result_;

    public:
        StateTransition(QState *source_state = 0)
            : QAbstractTransition(source_state)
            , result_(StateEvent::UNDEFINED) {}

        StateEvent::Result eventResult() const
        {
            return result_;
        }

        void setEventResult(StateEvent::Result result)
        {
            result_ = result;
        }

    protected:
        virtual bool eventTest(QEvent *event);

        virtual void onTransition(QEvent *event)
        {
            Q_UNUSED(event);
        }
};

#endif // STATE_TRANSITION

