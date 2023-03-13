#include "led_ui_item.h"
#include <QPainter>

namespace udav_gui
{

LedUiItem::LedUiItem(QWidget *parent) :
    QWidget(parent), AbstractUiCell()
{
    widget_ = this;
    value_ = false;
}

//-----------------------------------------------------------------------------

void LedUiItem::paintEvent(QPaintEvent* /*e*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(palette().foreground().color());

    if (value_)
    {
        painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
    }
    else
    {
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
    }

    painter.drawEllipse(width() * 1 / 3, height()* 1 / 3,  width() * 1 / 3, height() * 1 / 3);
}

}
