#ifndef STATE_EVENT
#define STATE_EVENT

#include <QEvent>

class QState;

class StateEvent : public QEvent
{
public:
    enum Result
    {
        UNDEFINED,
        SUCCEED,
        STOPPED,
        FAILED
    };

private:
    QState *state_;
    Result result_;

public:

    static const QEvent::Type &event_type;

    StateEvent(QState * state, Result result);

    QState *state() const
    {
        return state_;
    }
    StateEvent::Result result() const
    {
        return result_;
    }
};

#endif // STATE_EVENT

