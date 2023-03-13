#include "vaccum_chamber_button.h"

#include <QPainter>

namespace udav_gui
{

VacuumChamberButton::VacuumChamberButton(QWidget *parent) :
        CircuitPushButtonUiCell(parent)
{
}


void VacuumChamberButton::paintEvent(QPaintEvent* e)
{
    //CircuitPushButtonUiCell::paintEvent(e);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen thickPen(palette().foreground(), 2.5);

    painter.setPen(thickPen);

    // Углы
    int leftX = 0;
    int topY = 0;
    int rightX = width();
    int bottomY = height();

    // Середина
    int midY = (bottomY + topY) * 1 / 2;
    int midX = height() * 1 / 2;
    int midLeftX = width() * 1 / 8;
    int midRightX = width() * 7 / 8;

    QPoint points[7];
    points[0] = QPoint(leftX, topY);
    points[1] = QPoint(rightX, topY);
    points[2] = QPoint(midRightX, midY);
    points[3] = QPoint(rightX, bottomY);
    points[4] = QPoint(leftX, bottomY);
    points[5] = QPoint(midLeftX, midY);
    points[6] = QPoint(leftX, topY);

    painter.drawPolyline(points, 7);

    if (connection_ & Position::Top) painter.drawLine(midX, topY, midX, 0);
    if (connection_ & Position::Right) painter.drawLine(midRightX, midY, width(), midY);
    if (connection_ & Position::Bottom) painter.drawLine(midX, bottomY, midX, height());
    if (connection_ & Position::Left) painter.drawLine(midLeftX, midY, 0, midY);
}

}
