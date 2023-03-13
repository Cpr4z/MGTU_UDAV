#ifndef SELECT_DRIVER_DIALOG_H
#define SELECT_DRIVER_DIALOG_H

#include <QUuid>

#include "ui_select_driver_dialog.h"
#include "../core/core.h"

namespace udav_gui
{

class SelectDriverDialog : public QDialog, private Ui::ClientSelectDriverDialog
{
    Q_OBJECT

    public:

        explicit SelectDriverDialog(const QList<Core::UniversalNamedMap>& drvLst, QWidget *parent = 0);
        QUuid getSelection();

    private:
        const QList<Core::UniversalNamedMap>& items;
};

}

#endif // SELECT_DRIVER_DIALOG_H
