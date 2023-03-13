#include "group_ui_cell.h"

#include <QHBoxLayout>

#include "../equipment_control_page.h"


const int MarginLeft    = 9;
const int MarginTop     = 0;
const int MarginRight   = 9;
const int MarginBottom  = 3;


namespace udav_gui
{

void GroupUiCell::setLayout_(EquipmentControlPage *layout)
{
    if (!layout_) delete layout_;

    layout_ = new QHBoxLayout;
    layout_->setContentsMargins(MarginLeft, MarginTop, MarginRight, MarginRight); // LTRB
    layout_->addWidget(layout);

    setLayout(layout_);
}

GroupUiCell::GroupUiCell(QWidget *parent) :
    QGroupBox(parent), AbstractUiCell()
{
    widget_ = this;
    layout_ = NULL;
}

}
