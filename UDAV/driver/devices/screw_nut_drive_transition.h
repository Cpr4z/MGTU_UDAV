#ifndef SCREWNUTDRIVETRANSITION
#define SCREWNUTDRIVETRANSITION

#include <QAbstractTransition>

#include "screw_nut_drive_command.h"

/**
 * @brief Класс для описания специфических переходов конечного автомата привода винт-гайка
 */
class ScrewNutDriveTransition : public QAbstractTransition
{
    Q_OBJECT
    private:
        ScrewNutDriveCommand::Command command_;

    public:
        explicit ScrewNutDriveTransition(QState *source_state = 0)
            : QAbstractTransition(source_state),
              command_(ScrewNutDriveCommand::UNDEFINED)
        {

        }

        ScrewNutDriveCommand::Command eventResult() const
        {
            return command_;
        }

        void setEventResult(ScrewNutDriveCommand::Command result)
        {
            command_ = result;
        }

    protected:
        virtual bool eventTest(QEvent *event);

        virtual void onTransition(QEvent *event)
        {
            Q_UNUSED(event);
        }
};

#endif // SCREWNUTDRIVETRANSITION
