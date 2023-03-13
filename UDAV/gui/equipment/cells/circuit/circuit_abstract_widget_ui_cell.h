#ifndef CIRCUIT_ABSTRACT_WIDGET_UI_CELL_H
#define CIRCUIT_ABSTRACT_WIDGET_UI_CELL_H

#include <QWidget>

#include "circuit_abstract_ui_item.h"
#include "../abstract_ui_cell.h"

namespace udav_gui
{

/**
 * @brief Абстрактный элемент ячейки интерфейса интерактивной мнемосхемы
 */
class CircuitAbstractWidgetUiCell : public CircuitAbstractUiItem, public AbstractUiCell, public QWidget
{
    public:
        CircuitAbstractWidgetUiCell(QWidget* parent = 0);
};

}

#endif // CIRCUIT_ABSTRACT_WIDGET_UI_CELL_H
