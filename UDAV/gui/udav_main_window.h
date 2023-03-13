#ifndef UDAV_MAIN_WINDOW_H
#define UDAV_MAIN_WINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QUuid>
#include <QVector>

#include "elvactools/logger.h"
#include "driver/drivers.h"

class QPlainTextEdit;

namespace Core
{
    class UdavSettings;
}

namespace udav_drivers
{
    class DriversManager;
    class DriverImpl;
}

namespace udav_gui
{

class EquipmentControlPanel;
class MessageWindow;
class GridToolButtonWidget;

/**
 * @brief Класс главного окна программы-клиента
 *
 * TO BE CHECKED AND REWRITED
 *
 * В верхней части окна содержится панель кнопок для управления основными функциями программы.
 *
 * Главное окно программы клиента содержит центральную область, в которой имеются две закладки:
 *  - Панель управления оборудованием
 *  - Область просмотра лог-файла
 *
 * Сообщения, выводимые программой в лог-файл дублируются в соответствующую область просмотра.
 *
 * В момент запуска программы панель управлния оборудованием создается пустой. Реальное наполнение
 * панели управления оборудованием происходит после подключения к серверу и получения описания
 * интерфейса панели управления от выбранного драйвера управлением оборудованием.
 */
class UdavMainWindow : public QMainWindow
{
    Q_OBJECT
    ELVACTOOLS_LOGGER_SYSTEM_DECLARE

    /**
     * We use UdavMainWindowCreator to get the instance of UdavMainWindow.
     *
     * It let us divide the initiation and implementation logic
     * and make code clear.
     */
    friend class UdavMainWindowCreator;

    signals :
        void connectedToEquipment(udav_drivers::DriverImpl*);

    public slots:

        /**
         * @brief Закрыть всплывающее окно вывода сообщений
         * @param uuid
         * @param delay
         * @param autoClose
         */
        void closeMessageWindow(const QUuid& uuid, quint16 delay, bool autoClose);

        /**
         * @brief Открыть новое всплывающее окно вывода сообщений
         * @param uuid
         * @param header
         */
        void openMessageWindow(const QUuid& uuid, const QString& header);

        /**
         * @brief Переключить полноэкранный режим вывода главного окна программы
         *
         * Если текущее состояние вывода окна обычное окно, то вывод будет переключен
         * в полноэкранный режим.
         *
         * Если текущее состояние вывода окна полноэкранный режим, то вывод будет переключен
         * в режим обычного окна.
         */
        void switchFullScreen();

        /**
         * @brief Вывести сообщение во всплывающее окно вывода сообщений
         * @param uuid
         * @param type
         * @param message
         */
        void writeTextMessage2Window(const QUuid& uuid, quint16 type, const QString& message);

    protected:

        /**
         * @brief Default constructor should be called from UdavMainWindowCreator class
         * @param settings - application settings
         * @param logger - application logger
         */
        explicit UdavMainWindow(Core::UdavSettings* settings, elvactools::Logger* logger);

        /**
         * @brief Обработать событие закрытия окна
         * @param event
         */
        virtual void closeEvent(QCloseEvent *event);

    private:

        /**
         * @brief Прочитать настройки программы
         */
        void readGeometry_();

        /**
         * @brief Записать настройки программы
         */
        void writeGeometry_();

    private slots:

        /**
         * @brief Подключиться к серверу
         */
        void connectToServer_();

        /**
         * @brief Обработать сигнал подключения к серверу
         * @param address
         *
         * В заголовок программы выводится состояние подключения и адрес сервера
         */
        void onConnectedToEquipment_(udav_drivers::DriverImpl* driverImpl);

        /**
         * @brief Отключиться от сервера
         *
         * Функция проверяет наличие подключения к серверу. Если подключение установлено,
         * то пользователю будет задан вопрос на подтверждение действия.
         *
         * @return true - отключение с сервером разорвано, false - пользователь не подтвердил разрыв соединения
         */
        bool disconnectFromServer_();

        /**
         * @brief Обработать сигнал отключения от сервера
         * @param address
         *
         * В заголовок программы выводится состояние подключения
         */
        void disconnectedFromServer_();

        /**
         * @brief Редактировать настройки программы
         */
        void editSettings_();

        /**
         * @brief Сохранить протокол текущей сессии на диск
         */
        void saveProtocolToDisk_();

        /**
         * @brief Удалить всплывающее окно сообщений из массива открытых окон
         * @param window - указатель на окно, которое должно быть удалено из массива
         */
        void deleteMessageWindow_(QWidget* window);

        void swicth2ProgramControlPanel_();

    private :

        Core::UdavSettings* settings_;

        elvactools::Logger* logger_;

        //--------------------------------------------
        // Внутренние данные класса

        /**
         * @brief Запускать программу в полноэкранном или обычном режиме
         */
        bool fullScreen_;

        /**
         * @brief ? Массив открытых всплывающих окон вывода сообщений пользователю
         */
        QMap<QUuid, MessageWindow*> messageWindows_;

        //--------------------------------------------
        // Интерфейсные элементы

        /**
         * @brief Центральная область окна - с закладками
         */
        QTabWidget* centralTabWidget_;

        /**
         * @brief Панель для управления оборудованием
         */
        //EquipmentControlPanel* equipmentControlPanel_;

        /**
         * @brief Manager of hardware drivers available to the user
         */
        udav_drivers::DriversManager* driversManager_;

        /**
         * @brief Driver implementation connected to control panel
         */
        QVector<udav_drivers::DriverImpl*> connectedEquipment_;
};

}

#endif // UDAV_MAIN_WINDOW_H
