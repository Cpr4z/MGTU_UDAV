#include "vent_button_ui_cell.h"

#include <QPainter>

namespace udav_gui
{

//-----------------------------------------------------------------------------

VentButtonUiCell::VentButtonUiCell(QWidget *parent) :
    CircuitPushButtonUiCell(parent)
{
    setCheckable(true);
}

//-----------------------------------------------------------------------------

void
VentButtonUiCell::paintEvent(QPaintEvent* /*e*/)
{
    //SymbolicCircuitButton::paintEvent(e);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen thinPen(palette().foreground(), 1);
    QPen thickPen(palette().foreground(), 1.5);

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

    // Координаты граничных точек изображения клапана
    qreal leftX = 0.0, rightX = 0.0, topY = 0.0, bottomY = 0.0;
    qreal centerX = 0.0, centerY = 0.0;
    qreal delta = 0.1;

    // Координаты прямоугольника для вывода обозначения
    QRectF textRect;

    switch (textPos_)
    {
        case Position::Top :
            // Используем верхнюю половину для вывода текстового обозначения
            textRect.setLeft(0);
            textRect.setRight(width());
            textRect.setTop(height() / 3);
            textRect.setBottom(height() / 2);

            // Используем нижнюю половину кнопки для вывода графического обозначения
            centerX = width() / 2;
            centerY = height() * 3 / 4;

            leftX = width() / 5.0;
            topY = (height() / 2.0) + (height() / 2.0) / 5.0;
            rightX = width() * 4.0 / 5.0;
            bottomY = (height() / 2.0) + (height() / 2.0) * (4.0 / 5.0);

            delta *= (rightX - leftX);
        break;
        case Position::Bottom :
            // Используем нижнюю половину для вывода текстового обозначения
            textRect.setLeft(0);
            textRect.setRight(width());
            textRect.setTop(height() / 8);
            textRect.setBottom(height());

            // Используем верхнюю половину кнопки для вывода графического обозначения
            centerX = width() / 2.0;
            centerY = height() / 4.0;

            leftX = width() / 5.0;
            topY = (height() / 2.0) * (1.0 / 5.0);
            rightX = width() * 4.0 / 5.0;
            bottomY = (height() / 2.0) * (4.0 / 5.0);

            delta *= (rightX - leftX);
        break;
        case Position::Left :
            // Используем левую половину для вывода текстового обозначения
            textRect.setLeft(width() / 5);
            textRect.setRight(width() / 1.8);
            textRect.setTop(0.0);
            textRect.setBottom(height());

            // Используем правую половину кнопки для вывода графического обозначения
            centerX = width() - width() / 4;
            centerY = height() / 2;

            leftX = width() - (width() / 2.0) * (4.0 / 5.0);
            topY = height() / 5.0;
            rightX = width() - (width() / 2.0) * (1.0 / 5.0);
            bottomY = height() * 4.0 / 5.0;

            delta *= (bottomY - topY);
        break;
        case Position::Right :
            // Используем правую половину для вывода текстового обозначения
            textRect.setLeft(width() / 8);
            textRect.setRight(width());
            textRect.setTop(0.0);
            textRect.setBottom(height());

            // Используем левую половину кнопки для вывода графического обозначения
            centerX = width() / 4.0;
            centerY = height() / 2.0;

            leftX = (width() / 2.0) * (1.0 / 5.0);
            topY = height() / 5.0;
            rightX = (width() / 2.0) * (4.0 / 5.0);
            bottomY = height() * 4.0 / 5.0;

            delta *= (bottomY - topY);
        break;
        default :
            // Позиция текста не установлена, обозначение выводится не будет
            // Используем все поле кнопки для вывода элемента
            leftX = width() * 1 / 5;
            topY = height() * 1 / 5;
            rightX = width() * 4 / 5;
            bottomY = height() * 4 / 5;

            centerX = width() / 2;
            centerY = height() / 2;

            delta *= (rightX - leftX);
    }

    // Рисуем верхний треугольник клапана
    if (connection_ & Position::Top)
    {
        QPointF points[3];
        points[0] = QPointF(centerX, centerY);
        points[1] = QPointF(leftX + delta, 0);
        points[2] = QPointF(rightX - delta, 0);

        painter.setPen(thinPen);
        painter.drawConvexPolygon(points, 3);

        painter.setPen(thickPen);
    }

    if (connection_ & Position::Right)
    {
        QPointF points[3];
        points[0] = QPointF(centerX, centerY);
        points[1] = QPointF(width(), topY + delta);
        points[2] = QPointF(width(), bottomY - delta);

        painter.setPen(thinPen);
        painter.drawConvexPolygon(points, 3);

        painter.setPen(thickPen);
    }

    if (connection_ & Position::Bottom)
    {
        QPointF points[3];
        points[0] = QPointF(centerX, centerY);
        points[1] = QPointF(rightX - delta, height());
        points[2] = QPointF(leftX + delta, height());

        painter.setPen(thinPen);
        painter.drawConvexPolygon(points, 3);

        painter.setPen(thickPen);
    }

    if (connection_ & Position::Left)
    {
        QPointF points[3];
        points[0] = QPointF(centerX, centerY);
        points[1] = QPointF(0, bottomY - delta);
        points[2] = QPointF(0, topY + delta);

        painter.setPen(thinPen);
        painter.drawConvexPolygon(points, 3);

        painter.setPen(thickPen);
    }

    if (textPos_)
    {
        // Вывести обозначение в свободную часть кнопки
        QFont font;
        font.setFamily("Drouid Sans Mono");
        font.setStyleHint(QFont::Monospace);
        painter.setFont(font);
        painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter, text());
    }
}

}
