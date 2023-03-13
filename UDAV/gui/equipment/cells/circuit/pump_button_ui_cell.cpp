#include "pump_button_ui_cell.h"

#include <QPainter>

namespace udav_gui
{

//-----------------------------------------------------------------------------

PumpButtonUiCell::PumpButtonUiCell(QWidget *parent) :
    CircuitPushButtonUiCell(parent)
{
    setCheckable(true);

    connect(this, SIGNAL(toggled(bool)), this, SLOT(buttonToggled_(bool)));
}

//-----------------------------------------------------------------------------

void
PumpButtonUiCell::paintEvent(QPaintEvent* /*e*/)
{
    //SymbolicCircuitButton::paintEvent(e);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen thickPen(palette().foreground(), 1.5);
    painter.setPen(thickPen);

    QBrush brush = painter.brush();
    brush.setStyle(Qt::SolidPattern);
    if (isChecked())
    {
        brush.setColor(Qt::green);
    }
    else
    {
        brush.setColor(Qt::red);
    }
    painter.setBrush(brush);

    int leftX = 0;
    int topY = 0;
    int rightX = width();
    int bottomY = height();

    int midX = width() / 2;
    int midY = height() / 2;

    int rectWidth = rightX - leftX;
    int rectHeight = bottomY - topY;

    painter.drawRect(leftX, topY, rectWidth, rectHeight);

    painter.drawEllipse(width() * 1 / 4, height()* 1 / 4,  width() * 1 / 2, height() * 1 / 2);

    painter.drawLine(width() / 6, height() / 1.2, width() / 1.2, height() / 6);

    /*
    if (connection_ & Position::Top) painter.drawLine(midX, topY, midX, 0);
    if (connection_ & Position::Right) painter.drawLine(rightX, midY, width(), midY);
    if (connection_ & Position::Bottom) painter.drawLine(midX, bottomY, midX, height());
    if (connection_ & Position::Left) painter.drawLine(leftX, midY, 0, midY);
    */

}

//-----------------------------------------------------------------------------
}
