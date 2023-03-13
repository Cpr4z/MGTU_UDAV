#include "add_cells_dialog.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QSpinBox>

#include <QFormLayout>
#include <QVBoxLayout>

const int MAX_CELLS_ADDRESS = 65535;

//------------------------------------------------------------------------------

AddCellsDialog::AddCellsDialog(QWidget *parent)
    : QDialog(parent)
{
    startAddressEdit_ = new QSpinBox();
    startAddressEdit_->setMinimum(0);
    startAddressEdit_->setMaximum(MAX_CELLS_ADDRESS);

    cellsQuanityEdit_ = new QSpinBox();
    cellsQuanityEdit_->setMinimum(1);
    cellsQuanityEdit_->setMaximum(MAX_CELLS_ADDRESS);

    connect(startAddressEdit_, SIGNAL(valueChanged(int)), this, SLOT(setMaximumCellsQuantity(int)));

    QFormLayout* inputLayout = new QFormLayout();
    inputLayout->addRow(tr("Start address:"), startAddressEdit_);
    inputLayout->addRow(tr("Cells quantity:"), cellsQuanityEdit_);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

//------------------------------------------------------------------------------

int
AddCellsDialog::cellsQuantity() const
{
    return cellsQuanityEdit_->value();
}

//------------------------------------------------------------------------------

void
AddCellsDialog::setMaximumCellsQuantity(int startAddress)
{
    cellsQuanityEdit_->setMaximum(MAX_CELLS_ADDRESS - startAddress);
}

//------------------------------------------------------------------------------

int
AddCellsDialog::startAddress() const
{
    return startAddressEdit_->value();
}

//------------------------------------------------------------------------------
// EOF
