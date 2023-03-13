#ifndef PIPE_BUTTON_UI_CELL_H
#define PIPE_BUTTON_UI_CELL_H

#include "circuit_push_button_ui_cell.h"

namespace udav_gui
{

namespace CircuitButtonClassNames
{
    const QString Pipe = "pipe"; //!< Трубопровод
}

class PipeButtonUiCell :  public CircuitPushButtonUiCell
{
    Q_OBJECT
public:
    explicit PipeButtonUiCell(QWidget *parent = 0);
protected:
    virtual void paintEvent(QPaintEvent* e);

signals:

public slots:

};

}


#endif // PIPE_BUTTON_UI_CELL_H
