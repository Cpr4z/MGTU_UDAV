#ifndef VENT_BUTTON_UI_CELL_H
#define VENT_BUTTON_UI_CELL_H

#include "circuit_push_button_ui_cell.h"

//-----------------------------------------------------------------------------

/**
 * @brief Класс кнопки мнемосхемы для управления клапаном
 */

namespace udav_gui
{

namespace CircuitButtonClassNames
{
    const QString Vent = "vent"; //!< Клапан
}

class VentButtonUiCell : public CircuitPushButtonUiCell
{
    Q_OBJECT
    public:
        /**
         * @brief Конструктор по умолчанию
         * @param parent - объект-владелец
         */
        explicit VentButtonUiCell(QWidget *parent = 0);

    protected:
        /**
         * @brief Отрисовывает изображение датчика на мнемосхеме
         * @param e - событие рисования
         */
        virtual void paintEvent(QPaintEvent *);

    signals:

    public slots:

};

}

#endif // VENT_BUTTON_UI_CELL_H
