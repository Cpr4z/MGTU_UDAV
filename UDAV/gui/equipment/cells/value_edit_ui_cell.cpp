#include "value_edit_ui_cell.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QEvent>

#include "../../numeric_pad.h"

const int ButtonMaxWidth = 50;

const int MarginLeft    = 9;
const int MarginTop     = 0;
const int MarginRight   = 9;
const int MarginBottom  = 3;

namespace udav_gui
{

ValueEditUiCell::ValueEditUiCell(QWidget *parent, const QString &title, const QString unit) :
    QWidget(parent), AbstractUiCell(),
    title_(title),
    unit_(unit),
    value_(0)
{
    widget_ = this;

    showTitle_ = true;

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(MarginLeft, MarginTop, MarginRight, MarginBottom);

    QHBoxLayout* boxLayout = new QHBoxLayout;
    boxLayout->setContentsMargins(MarginLeft, MarginTop, MarginRight, MarginBottom);

    spinBox_ = new QDoubleSpinBox;
    spinBox_->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinBox_->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred));
    spinBox_->setSuffix(QString(" %1").arg(unit_));
    spinBox_->installEventFilter(this);

    acceptButton = new QPushButton(tr("Accept"));
    acceptButton->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred));
    acceptButton->setMaximumWidth(ButtonMaxWidth);
    connect(acceptButton, SIGNAL(clicked(bool)), this, SLOT(acceptValue_()));

    revertButton = new QPushButton(tr("Revert"));
    revertButton->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred));
    revertButton->setMaximumWidth(ButtonMaxWidth);
    connect(revertButton, SIGNAL(clicked(bool)), this, SLOT(revertValue_()));

    boxLayout->addWidget(spinBox_);
    boxLayout->addWidget(acceptButton);
    boxLayout->addWidget(revertButton);

    groupBox_ = new QGroupBox;
    groupBox_->setTitle(title);
    groupBox_->setFlat(false);

    if (showTitle_)
    {
        groupBox_->setLayout(boxLayout);
        mainLayout->addWidget(groupBox_);
        setLayout(mainLayout);
    }
    else
    {
        setLayout(boxLayout);
    }
}

//-----------------------------------------------------------------------------

void
ValueEditUiCell::setRange(double min, double max)
{
    spinBox_->setRange(min, max);
}

void
ValueEditUiCell::setValue(double value)
{
    spinBox_->setValue(value);
}

//-----------------------------------------------------------------------------

bool
ValueEditUiCell::eventFilter(QObject* object, QEvent* event)
{
    if (object == spinBox_)
    {
        if (event->type() == QEvent::FocusIn)
        {
            NumericPad* numpad = new NumericPad();
            numpad->setPrecision(spinBox_->decimals());
            numpad->setValue(spinBox_->value());

            if (numpad->exec() == QDialog::Accepted)
                spinBox_->setValue(numpad->getValue());

            delete numpad;

            acceptButton->setFocus();

            return true;
        }
    }
    return QWidget::eventFilter(object, event);
}

//-----------------------------------------------------------------------------

void
ValueEditUiCell::acceptValue_()
{
    value_ = spinBox_->value();
    emit controlSignalChanged(signalId_, QVariant(value_));
}

//-----------------------------------------------------------------------------

void
ValueEditUiCell::revertValue_()
{
    spinBox_->setValue(value_);
}

}
