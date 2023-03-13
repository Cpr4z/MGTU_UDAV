#include "equipment_debug_widget.h"

#include <QLabel>

#include <QHBoxLayout>

namespace udav_gui
{

EquipmentDebugWidget::EquipmentDebugWidget(QWidget *parent) : QWidget(parent)
{
    QLabel* label = new QLabel(tr("Connect to server to start debugging!"), this);

    label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    label->setAlignment(Qt::AlignCenter | Qt::AlignCenter);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(label);

    setLayout(mainLayout);
}

}

