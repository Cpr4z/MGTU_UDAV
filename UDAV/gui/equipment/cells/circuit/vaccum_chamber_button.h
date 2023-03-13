#ifndef VACCUM_CHAMBER_BUTTON_H
#define VACCUM_CHAMBER_BUTTON_H

#include "circuit_push_button_ui_cell.h"

namespace udav_gui
{

namespace CircuitButtonClassNames
{
    const QString VacuumChamber = "vacuumChamber"; //!< Vacuum chamber
}

class VacuumChamberButton : public CircuitPushButtonUiCell
{
    Q_OBJECT
public:
    explicit VacuumChamberButton(QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent* e);

signals:

public slots:

};

}

#endif // VACCUM_CHAMBER_BUTTON_H
