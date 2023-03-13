#include "knob_ui_item.h"

#include <QResizeEvent>
#include <QPen>

#include <qwt_round_scale_draw.h>

#include <QVBoxLayout>

namespace udav_gui
{

const double KnobUiItem::ScaleFactor_ = 0.4;


KnobUiItem::KnobUiItem(const QString& title, QWidget *parent) :
    QWidget(parent), AbstractUiCell()
{
    widget_ = this;

    knob_ = new QwtKnob(this);
//	knob->setRange(min, max, 0,1);
//	knob->setScaleMaxMajor(10);

//	knob->setKnobWidth(50);

    QFont font;
    font.setBold(true);
    font.setFamily("Arial");
    font.setPointSize(12);

    label_ = new QLabel(title, this);
    label_->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    label_->setFont(font);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(knob_);
    mainLayout->addWidget(label_);

    setLayout(mainLayout);

    //setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    connect(knob_, SIGNAL(valueChanged(double)), this, SLOT(valueChanged(double)));
}

//-----------------------------------------------------------------------------

void
KnobUiItem::valueChanged(double value)
{
    emit valueChanged(this, value);
}

}
