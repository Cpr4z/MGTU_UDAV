#include "select_driver_dialog.h"

//------------------------------------------------------------------------------

namespace udav_gui
{

SelectDriverDialog::SelectDriverDialog(const QList<Core::UniversalNamedMap>& drvLst, QWidget *parent) :
    QDialog(parent), items(drvLst)
{
    setupUi(this);

    for(const auto& item : items)
    {
        listWidget->addItem(item["desc"].toString());
    }

    listWidget->setCurrentItem(listWidget->itemAt(0, 0));
}

//------------------------------------------------------------------------------

QUuid
SelectDriverDialog::getSelection()
{
    int selectedItem = listWidget->currentRow();

    return QUuid(items[selectedItem]["uuid"].toString());
}

} // namespace udav_gui

//------------------------------------------------------------------------------
// EOF
