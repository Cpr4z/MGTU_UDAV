#include "core/udav_settings.h"

#include "equipment_control_page.h"

#include "cells/abstract_ui_cell.h"

#include <QPainter>

namespace udav_gui
{

void
EquipmentControlPage::paintEvent(QPaintEvent *e)
{
    if (settings_->showGrid())
    {
        if (gridStep_)
        {
            QPainter painter(this);
            painter.setRenderHint(QPainter::TextAntialiasing, true);

            painter.setPen(QPen(palette().foreground().color(), 1, Qt::DotLine, Qt::FlatCap));

            // Рисуем вертикальные линии сетки
            for (int i = 0; i <= xSize_; ++i)
                painter.drawLine(i * gridStep_, 0, i * gridStep_, ySize_ * gridStep_);

            // Рисуем горизонтальные линии сетки
            for (int i = 0; i <= ySize_; ++i)
                painter.drawLine(0, i * gridStep_, xSize_ * gridStep_, i * gridStep_);
        }
    }

    QWidget::paintEvent(e);
}

//-----------------------------------------------------------------------------

void
EquipmentControlPage::resizeEvent(QResizeEvent* /*event*/)
{
    if (xSize_ != 0 && ySize_ != 0)
    {
        int hStep = width() / xSize_;
        int vStep = height() / ySize_;

        if (hStep < vStep)
            gridStep_ = hStep;
        else
            gridStep_ = vStep;
    }

    // Изменяем геометрию каждой кнопки, размещенной на мнемосхеме
    AbstractUiCell* item;
    foreach(item, uiItems_)
    {
        item->setGridStep(gridStep_);
    }
}

} // namespace Gui
