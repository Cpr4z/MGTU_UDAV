#include "pipe_button_ui_cell.h"

#include <QPainter>

#include <QDebug>

namespace udav_gui
{

//-----------------------------------------------------------------------------

PipeButtonUiCell::PipeButtonUiCell(QWidget *parent) :
        CircuitPushButtonUiCell(parent)
{
    //setEnabled(false);
}

//-----------------------------------------------------------------------------

void
PipeButtonUiCell::paintEvent(QPaintEvent* /*e*/)
{
    //SymbolicCircuitButton::paintEvent(e);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen thickPen(palette().foreground(), 5);
    painter.setPen(thickPen);

    int centerX = width() / 2;
    int centerY = height() / 2;

    if (connection_ & Position::Top) painter.drawLine(centerX, centerY, centerX, 0);
    if (connection_ & Position::Right) painter.drawLine(centerX, centerY, width(), centerY);
    if (connection_ & Position::Bottom) painter.drawLine(centerX, centerY, centerX, height());
    if (connection_ & Position::Left) painter.drawLine(centerX, centerY, 0, centerY);
}


}
