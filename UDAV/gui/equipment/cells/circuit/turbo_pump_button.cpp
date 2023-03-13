#include "turbo_pump_button.h"

#include <QPainter>

namespace udav_gui
{

TurboPumpButton::TurboPumpButton(QWidget *parent) :
        CircuitPushButtonUiCell(parent)
{
}

void TurboPumpButton::paintEvent(QPaintEvent* /*e*/)
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

    painter.drawLine(width() / 2.0, height() / 2.8, width() / 2.0, height() / 1.6);
    painter.drawLine(width() / 2.7, height() / 2.8, width() / 1.6, height() / 2.8);
    painter.drawLine(width() / 2.7, height() / 2.23, width() / 1.6, height() / 2.23);
    painter.drawLine(width() / 2.7, height() / 1.86, width() / 1.6, height() / 1.86);
    painter.drawLine(width() / 2.7, height() / 1.6, width() / 1.6, height() / 1.6);
}

}
