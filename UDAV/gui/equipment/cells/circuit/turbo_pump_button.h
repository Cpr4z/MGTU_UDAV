#ifndef TURBO_PUMP_BUTTON_H
#define TURBO_PUMP_BUTTON_H

#include "circuit_push_button_ui_cell.h"

namespace udav_gui
{

namespace CircuitButtonClassNames
{
    const QString TurboPump = "turboPump"; //!< Vacuum pump (turbo)
}

class TurboPumpButton :  public CircuitPushButtonUiCell
{
    Q_OBJECT
public:
    explicit TurboPumpButton(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *e);

signals:

public slots:

};

}

#endif // TURBO_PUMP_BUTTON_H
