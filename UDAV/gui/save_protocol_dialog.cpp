#include "core/udav_settings.h"

#include "save_protocol_dialog.h"
#include "ui_save_protocol_dialog.h"
#include "elvactools/utils.h"

#include <QFileDialog>
#include <QDir>

namespace udav_gui
{

//-----------------------------------------------------------------------------

SaveProtocolDialog::SaveProtocolDialog(Core::UdavSettings* settings, QWidget *parent) :
    QDialog(parent),
    settings_(settings)
{
    setupUi(this);

    targetEdit->setText(settings_->protocolSaveToPath());

    // Заполнить список файлов
    QString path = Core::checkBackSlash(settings_->protocolPath());

    QDir dir(path);

    dir.setFilter(QDir::Files | QDir::NoSymLinks);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        protocolsList->addItem(fileInfo.fileName());
    }
}

//-----------------------------------------------------------------------------

SaveProtocolDialog::~SaveProtocolDialog()
{
}

//-----------------------------------------------------------------------------

QStringList
SaveProtocolDialog::selectedFiles() const
{
    QStringList result;
    QString path = Core::checkBackSlash(settings_->protocolPath());

    QList<QListWidgetItem *> selected = protocolsList->selectedItems();
    foreach(QListWidgetItem* item, selected)
    {
        result.append(item->text());
    }
    return result;
}

//-----------------------------------------------------------------------------

QString
SaveProtocolDialog::target() const
{
    return targetEdit->text();
}

//-----------------------------------------------------------------------------

void
SaveProtocolDialog::on_browseTargetButton_clicked()
{
    QFileDialog dialog(this, tr("Select path for saving protocols"), targetEdit->text());
    dialog.setOption(QFileDialog::ShowDirsOnly, true);

    if (dialog.exec())
    {
        targetEdit->setText(dialog.directory().absolutePath());
    }
}

//-----------------------------------------------------------------------------

}
