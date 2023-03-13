#include "push_button_ui_cell.h"

#include <QMessageBox>
#include <QPainter>

namespace udav_gui
{

PushButtonUiCell::PushButtonUiCell(QWidget *parent) :
    QPushButton(parent), AbstractUiCell(),
    onQuestion_(),
    onQuestionOn_(false),
    offQuestion_(),
    offQuestionOn_(false)
{
    widget_ = this;

    setCheckable(true);

    connect(this, SIGNAL(toggled(bool)), this, SLOT(buttonToggled_(bool)));
}

//-----------------------------------------------------------------------------

void
PushButtonUiCell::setSignalId(udav_drivers::SignalID signalId)
{
    AbstractUiCell::setSignalId(signalId);

    if (!signalId)
        setEnabled(false);
    else
        setEnabled(true);
}

//-----------------------------------------------------------------------------

void
PushButtonUiCell::draw_(QPainter* painter)
{
    //----------------------------------------------
    // Рисуем ободок кнопки (BOF)

    QColor niceBlue(150, 150, 200);

    QConicalGradient coneGradient(0, -12, 90.0);
    coneGradient.setColorAt(0.0, Qt::darkGray);
    coneGradient.setColorAt(0.2, niceBlue);
    coneGradient.setColorAt(0.5, Qt::white);
    coneGradient.setColorAt(1.0, Qt::darkGray);

    painter->setBrush(coneGradient);
    painter->drawEllipse(-35, -45, 70, 70);

    // Рисуем ободок кнопки (EOF)
    //----------------------------------------------

    //----------------------------------------------
    // Рисуем кнопку (BOF)

    QRadialGradient radialGradient(-32, -45, 64);

    if (isChecked())
    {
        radialGradient.setColorAt(0.0, Qt::green);
        radialGradient.setColorAt(1.0, Qt::darkGreen);
    }
    else
    {
        radialGradient.setColorAt(0.0, Qt::gray);
        radialGradient.setColorAt(1.0, Qt::darkGray);
    }

    painter->setBrush(radialGradient);
    painter->drawEllipse(-32, -42, 64, 64);

    // Рисуем кнопку (EOF)
    //----------------------------------------------

    //----------------------------------------------
    // Рисуем подпись под кнопкой (BOF)

    QPen thinPen(palette().foreground(), 0.5);

    painter->setBrush(palette().background());
    painter->setPen(thinPen);
    painter->drawRoundedRect(-45, 30, 90, 15, 3, 3);

    QFont font;
    font.setFamily("Drouid Sans Mono");
    font.setStyleHint(QFont::Monospace);
    painter->setFont(font);
    painter->drawText(-45, 30, 90, 15, Qt::AlignHCenter | Qt::AlignVCenter, text());

    // Рисуем подпись под кнопкой (EOF)
    //----------------------------------------------
}

//-----------------------------------------------------------------------------

void
PushButtonUiCell::nextCheckState()
{
    if (isChecked())
    {
        if (offQuestionOn())
        {
            int res = QMessageBox::warning(this, tr("Question"), offQuestion(), QMessageBox::Yes | QMessageBox::No);
            if (res == QMessageBox::Yes) setChecked(false);
        }
        else
            setChecked(!isChecked());
    }
    else
    {
        if (onQuestionOn())
        {
            int res = QMessageBox::warning(this, tr("Question"), onQuestion(), QMessageBox::Yes | QMessageBox::No);
            if (res == QMessageBox::Yes) setChecked(true);
        }
        else
            setChecked(!isChecked());
    }
}

//-----------------------------------------------------------------------------

void
PushButtonUiCell::paintEvent(QPaintEvent* /*e*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int side = qMin(width(), height());
    painter.setViewport((width() - side) / 2, (height() - side) / 2, side, side);
    painter.setWindow(-50, -50, 100, 100);

    draw_(&painter);
}


}
