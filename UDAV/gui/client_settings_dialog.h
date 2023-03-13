#ifndef CLIENT_SETTINGS_WINDOW_H
#define CLIENT_SETTINGS_WINDOW_H

#include "ui_client_settings_dialog.h"
#include "../core/core.h"

namespace Core
{
    class UdavSettings;
}

namespace udav_gui
{

//! Окно настройки параметров программы тонкого клиента.

class ClientSettingsDialog : public QDialog, private Ui::ClientSettingsDialog
{
    Q_OBJECT

    private:
//        Core::Client* client_;

        Core::UdavSettings* settings_;

    public:
        explicit ClientSettingsDialog(Core::UdavSettings* settings, QWidget *parent = 0);

    private:
        void applySettings();

        /**
         * @brief Установить доступность элементов диалога в зависимости от выбранных настроек
         */
        void enableUiItems_();

        void readSettings();
        void writeSettings();


    private slots:
        void on_autoConnectBox_stateChanged(int state);
        void on_buttonBox_clicked(QAbstractButton * button);
        void on_buttonBox_accepted();
        void on_clearCacheButton_clicked();
        void on_loadDriversButton_clicked();
        void on_protocolPathSelectButton_clicked();
        void on_protocolSaveToBrowseButton_clicked();

        /**
         * @brief Обработать изменение состояния флага включения ведения протокола работы
         * @param state - новое состояние флага
         */
        void on_writeProtocolCheckBox_stateChanged(int state);

    public slots:
        void fillDriversListComboBox(const QList<Core::UniversalNamedMap>& driversList);

    signals: // Система записи лог-файлов

        /**
         * @brief Отправляет сообщение о критической ошибке
         * @param message - текст сообщения
         */
        void criticalMessage(const QString& message) const;

        /**
         * @brief Отправляет отладочное сообщение
         * @param message - текст сообщения
         */
        void debugMessage(const QString& message) const;

        /**
         * @brief Отправляет информационное сообщение
         * @param message - текст сообщения
         */
        void infoMessage(const QString& message) const;
};

}

#endif // CLIENT_SETTINGS_WINDOW_H
