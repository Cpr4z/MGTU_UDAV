#ifndef LEAK_BUTTON_UI_CELL_H
#define LEAK_BUTTON_UI_CELL_H

#include <QString>

#include "vent_button_ui_cell.h"

namespace udav_gui
{

//-----------------------------------------------------------------------------

namespace CircuitButtonClassNames
{
    const QString Leak = "leak"; //!< Натекатель
}

//-----------------------------------------------------------------------------

/**
 * @brief Класс кнопки мнемосхемы для управления натекателем
 */
class LeakButtonUiCell :  public VentButtonUiCell
{
    Q_OBJECT
    public:
        /**
         * @brief Конструктор по умолчанию
         * @param parent - объект-владелец
         */
        explicit LeakButtonUiCell(QWidget *parent = 0);

    protected:

        /**
         * @brief Отрисовывает изображение датчика на мнемосхеме
         * @param e - событие рисования
         */
        void paintEvent(QPaintEvent* e);

    signals:

    public slots:
};

}
#endif // LEAK_BUTTON_UI_CELL_H
