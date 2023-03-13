#ifndef PUMP_BUTTON_UI_CELL_H
#define PUMP_BUTTON_UI_CELL_H

#include "circuit_push_button_ui_cell.h"

//-----------------------------------------------------------------------------

namespace udav_gui
{

namespace CircuitButtonClassNames
{
    const QString Pump = "pump"; //!< Вакуумный насос (без указания типа)
}

//-----------------------------------------------------------------------------

class PumpButtonUiCell : public CircuitPushButtonUiCell
{
    public:
        explicit PumpButtonUiCell(QWidget *parent = 0);

    protected:
        void paintEvent(QPaintEvent* e);

    signals:

    public slots:
};

}

#endif // PUMP_BUTTON_UI_CELL_H
