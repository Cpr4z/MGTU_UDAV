#include "screw_nut_drive_transition.h"

bool
ScrewNutDriveTransition::eventTest(QEvent *event)
{
    if (event->type() == ScrewNutDriveCommand::event_type)
    {
        ScrewNutDriveCommand *lh_event = static_cast<ScrewNutDriveCommand*>(event);

        return (lh_event->state() == sourceState() && lh_event->command() == command_);
    }

    return false;
}
