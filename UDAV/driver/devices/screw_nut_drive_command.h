#ifndef SCREWNUTDRIVECOMMAND_H
#define SCREWNUTDRIVECOMMAND_H

#include <QEvent>
#include <QState>
/**
 * @brief Класс для описания внешних команд конечного автомата привода винт-гайка
 */
class ScrewNutDriveCommand : public QEvent
{
    public:
        enum Command
        {
            UNDEFINED,  //!< Команда не определена
            MOVE,       //!< Запустить двиежение
            STOP        //!< Остановить движение
        };

    private:
        QState *state_;
        Command command_;

    public:

        static const QEvent::Type &event_type;

        ScrewNutDriveCommand(QState *state, Command command)
            : QEvent(event_type),
              state_(state),
              command_(command) {}

        QState *state() const
        {
            return state_;
        }

        ScrewNutDriveCommand::Command command() const
        {
            return command_;
        }
};

#endif // SCREWNUTDRIVECOMMAND_H
