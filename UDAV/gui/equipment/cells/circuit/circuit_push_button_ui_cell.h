#ifndef SYMBOLIC_CIRCUIT_BUTTON_H
#define SYMBOLIC_CIRCUIT_BUTTON_H

#include <QPushButton>
#include "circuit_abstract_ui_item.h"

#include "../push_button_ui_cell.h"

namespace udav_gui
{

/**
 * @brief Базовый класс для кнопок интективной мнемосхемы
 */
class CircuitPushButtonUiCell : public PushButtonUiCell, public CircuitAbstractUiItem
{
    Q_OBJECT
    public:
        explicit CircuitPushButtonUiCell(QWidget* parent)
            : PushButtonUiCell(parent), CircuitAbstractUiItem()
        {
        }


};

}

#endif // SYMBOLIC_CIRCUIT_BUTTON_H
