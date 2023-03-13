#ifndef VACCUM_SENSOR_BUTTON_UI_CELL_H
#define VACCUM_SENSOR_BUTTON_UI_CELL_H

#include "circuit_push_button_ui_cell.h"

namespace udav_gui
{

namespace CircuitButtonClassNames
{
    const QString Sensor = "sensor"; //!< Датчик
}

/**
 * @brief Класс для вывода на поле мнемосхемы изображения датчика
 */
class VacuumSensorButtonUiCell :  public CircuitPushButtonUiCell
{
    Q_OBJECT
    public:
        /**
         * @brief Конструктор по умолчанию
         * @param parent - объект-владелец
         */
        explicit VacuumSensorButtonUiCell(QWidget *parent = 0);

    protected:
        /**
         * @brief Отрисовывает изображение датчика на мнемосхеме
         * @param e - событие рисования
         */
        virtual void paintEvent(QPaintEvent *e);

    signals:

    public slots:
};

}

#endif // VACCUM_SENSOR_BUTTON_UI_CELL_H
