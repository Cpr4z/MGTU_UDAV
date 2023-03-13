#ifndef SAVE_PROTOCOL_DIALOG_H
#define SAVE_PROTOCOL_DIALOG_H

#include <QDialog>
#include "ui_save_protocol_dialog.h"

namespace Core
{
    class UdavSettings;
}

namespace udav_gui
{

class SaveProtocolDialog : public QDialog, private Ui::SaveProtocolDialog
{
        Q_OBJECT

    public:
        explicit SaveProtocolDialog(Core::UdavSettings* settings, QWidget *parent = 0);
        ~SaveProtocolDialog();

        QStringList selectedFiles() const;
        QString target() const;

    private slots:
        void on_browseTargetButton_clicked();

    private:
        Core::UdavSettings* settings_;
};

}

#endif // SAVE_PROTOCOL_DIALOG_H
