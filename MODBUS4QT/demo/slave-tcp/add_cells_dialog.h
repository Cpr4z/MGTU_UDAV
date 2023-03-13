#ifndef ADDCELLSDIALOG_H
#define ADDCELLSDIALOG_H

#include <QDialog>

class QSpinBox;

class AddCellsDialog : public QDialog
{
    Q_OBJECT

    public:

        explicit AddCellsDialog(QWidget* parent = nullptr);

        int startAddress() const;

        int cellsQuantity() const;

    signals:

    protected:

        QSpinBox* startAddressEdit_;

        QSpinBox* cellsQuanityEdit_;

    protected slots:
        void setMaximumCellsQuantity(int startAddress);
};

#endif // ADDCELLSDIALOG_H
