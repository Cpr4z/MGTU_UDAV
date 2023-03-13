#include "vaccum_sensor_button_ui_cell.h"

#include <QPainter>

namespace udav_gui
{

VacuumSensorButtonUiCell::VacuumSensorButtonUiCell(QWidget *parent) :
    CircuitPushButtonUiCell(parent)
{
}

void VacuumSensorButtonUiCell::paintEvent(QPaintEvent* /*e*/)
{
    //QPushButton::paintEvent(e);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen thinPen(palette().foreground(), 1.5);
    QPen thickPen(palette().foreground(), 2.5);
    painter.setPen(thickPen);

    // Координаты граничных точек изображения датчика
    qreal leftX = 0.0, rightX = 0.0, topY = 0.0, bottomY = 0.0;
    qreal centerX = 0.0, centerY = 0.0;
    qreal rX = 0.0, rY = 0.0;

    // Координаты прямоугольника для вывода обозначения
    QRectF textRect;


    const qreal R = 0.97;        // Размер радиуса окружности от величины поля рисования
    const qreal ARROW = 0.4;    // Размер половины длины стрелки от радиуса датчика

    switch (textPos_)
    {
        case Position::Top :
            // Используем верхнюю половину для вывода текстового обозначения
            textRect.setLeft(0);
            textRect.setRight(width());
            textRect.setTop(0);
            textRect.setBottom(height() / 1.5);

            // Используем нижнюю половину для вывода графического обозначения
            centerX = width() / 2.0;
            centerY = height() * 3.0 / 4.0;

            rX = centerX * R;
            rY = rX;
        break;
        case Position::Bottom :
            // Используем нижнюю половину для вывода текстового обозначения
            textRect.setLeft(0);
            textRect.setRight(width());
            textRect.setTop(height() / 2.0);
            textRect.setBottom(height() / 1.3);

            // Используем верхнюю половину кнопки для вывода графического обозначения
            centerX = width() / 2.0;
            centerY = height() / 4.0;

            rX = centerX * R;
            rY = rX;
        break;
        case Position::Left :
            // Используем левую половину для вывода текстового обозначения
            textRect.setLeft(0);
            textRect.setRight(width() / 1.5);
            textRect.setTop(0.0);
            textRect.setBottom(height());

            // Текст слева
            // Используем правую половину кнопки для вывода графического обозначения
            centerX = width() - width() / 4.0;
            centerY = height() / 2.0;

            rY = centerY * R;
            rX = rY;
        break;
        case Position::Right :
            // Используем левую половину для вывода текстового обозначения
            textRect.setLeft(width() / 2.0);
            textRect.setRight(width() / 1.3);
            textRect.setTop(0.0);
            textRect.setBottom(height());

            // Используем левую половину кнопки для вывода графического обозначения
            centerX = width() / 4.0;
            centerY = height() / 2.0;

            rY = centerY * R;
            rX = rY;
        break;
        default :
            // Позиция текста не установлена, обозначение выводится не будет
            // Используем все поле кнопки для вывода элемента
            centerX = width() / 2;
            centerY = height() / 2;

            rX = centerX * R;
            rY = centerY * R;
    }

    leftX = centerX - rX;
    topY = centerY - rY;
    rightX = centerX + rX;
    bottomY = centerY + rY;

    painter.drawEllipse(QPointF(centerX, centerY), rX, rY);

    // Нарисовать стрелочку датчика
    painter.setPen(thinPen);
    QPointF startArrow(centerX + rX * ARROW, centerY + rY * ARROW);
    QPointF endArrow(centerX - rX * ARROW, centerY - rY * ARROW);

    painter.drawLine(startArrow, endArrow);
    painter.drawLine(endArrow, QPointF(centerX, centerY - 0.25 * rY));
    painter.drawLine(endArrow, QPointF(centerX - 0.25 * rX, centerY));

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
