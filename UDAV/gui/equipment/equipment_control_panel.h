#ifndef EQUIPMENT_WIDGET_H
#define EQUIPMENT_WIDGET_H

#include <QWidget>
#include <QMap>

#include "elvactools/files/incoming_files_dispatcher.h"
#include "driver/drivers.h"
#include "driver/meta_signal.h"
#include "elvactools/logger.h"

class QPushButton;
class QPlainTextEdit;
class QTextBrowser;

class QListWidget;
class QListWidgetItem;


//class QStackedWidget;
class QTabWidget;

namespace Core
{
    class Client;
    class IncomingFilesDispatcher;
    class UdavSettings;
}

struct UiEnabler
{
    udav_drivers::SignalID signalValue;
    QWidget* ui;

    UiEnabler(udav_drivers::SignalID value = 0, QWidget* ptr = 0)
        : signalValue(value), ui(ptr)
    {

    }
};

namespace udav_drivers
{
    class DriverImpl;
}

namespace udav_gui
{

class EquipmentControlPage;
class EquipmentConfigWidget;
class EquipmentDebugWidget;
class EquipmentHelpWidget;
class GridToolButtonWidget;
class PostionedWidget;

class AbstractUiCell;
class PushButtonUiCell;
class KnobUiItem;
class TextMessageUiItem;

/**
 * @brief The EquipmentControlPanel widget class for equipment gui control panel.
 *
 * At the right side we have buttons switching mode of the control panel:
 * - working with the connected equipment (default working mode);
 * - starting new protocol;
 * - saving the current protocol;
 * - disconnecting from equipment without shutdown it;
 * - shutdown equipment and disconnect;
 * - reload ui from driver;
 * - setup (not implemented);
 * - debug (not implemented);
 * - user help (not implemented).
 *
 * In default working mode the left side of the widget contains tabs of pages
 * described by driver at the moment of connection. Tabs placed at the bottom
 * side.
 *
 * At the bottom of the widget we have area to display text messaged from driver.
 *
 */
class EquipmentControlPanel : public QWidget
{
    Q_OBJECT
    ELVACTOOLS_LOGGER_SYSTEM_DECLARE
    friend class EquipmentUiReader;

    enum Actions
    {
//        CONTROL_PAGES,
        START_NEW_PROTOCOL,
        SAVE_PROTOCOL,
        DISCONNECT,
        SHUTDOWN,
        RELOAD_UI,
//        SETUP_DRIVER,
//        DEBUG_DRIVER,
//        HELP
    };
    Q_ENUM(Actions);

    private:

        /**
         * @brief Клиент, осуществляющего связь с сервером
         */
        Core::Client* client_;

        udav_drivers::DriverImpl* driverImpl_;

        /**
         * @brief Диспетчер файлов, принимаемых с сервера
         *
         * Диспетчер используется для обработки загружамых с сервера файлов изображений
         */
        Core::IncomingFilesDispatcher fileDispatcher_;

        /**
         * @brief Карта соответствия интерфейсных элементов кодам сигналов
         */
        QMap<AbstractUiCell*, udav_drivers::SignalID> ui2signal_;

        /**
         * @brief Карта соответствия кодов сигналов интерфейсным элементам
         */
        QMap<udav_drivers::SignalID, AbstractUiCell*> signal2ui_;

        /**
         * @brief Карта соответствия доступности элементов управления от значений сигналов
         *
         * @todo Задокументировать использование данной возможноти!
         */
        QMap<udav_drivers::SignalID, UiEnabler> uiEnbaler;

        /**
         * @brief Левая колонка кнопок для переключения между страницами панели управления
         */
        GridToolButtonWidget* pagesButtonsWidget_;

        /**
         * @brief Центральный элемент панели управления - панель закладок - Переключение между закладками осуществляется с помощью SideBar, как в Qt Creator
         */

        QTabWidget* controlPages_;

        /**
         * @brief Список созданных страниц с интерфейсом пользователя управления оборудованием
         */
        QVector<EquipmentControlPage*> controlPagesArray_;

        /**
         * @brief Массив страниц с интерфейсом пользователя, открытых в отдельных окнах
         */
        QVector<PostionedWidget*> controlWindowsArray_;

        /**
         * @brief Массив команд на переключение страниц
         *
         * Нужен, чтобы удалить их, когда перезагружаем интефейс пользователя,
         * так как данный класс выступает их владельцем
         */
        QVector<QAction*> pagesActionsArray_;

        /**
         * @brief Страница для настройки драйвера оборудования
         */
        EquipmentConfigWidget* configPage_;

        /**
         * @brief Страница для отладки драйвера оборудования
         */
        EquipmentDebugWidget* debugPage_;

        /**
         * @brief Страница справки по драйверу оборудования
         */
        EquipmentHelpWidget* helpPage_;

        /**
         * @brief Область вывода информационных сообщений, поступающих от оборудования. Расположена в нижней части окна.
         */
        QTextBrowser* messagesBrowser_;

        Core::UdavSettings* settings_;

        elvactools::Logger* logger_;

        EquipmentControlPage* createControlPage();

    public:
        /**
         * @brief Конструктор по умолчанию
         * @param client - указатель на клиента системы
         * @param parent - указатель на интерфейсный-элемент владелец
         */
        explicit EquipmentControlPanel(udav_drivers::DriverImpl* driverImpl,
                                       Core::UdavSettings* settings_,
                                       elvactools::Logger* logger,
                                       QWidget* parent = 0);

        virtual ~EquipmentControlPanel();

        //void setVisibleItems();

    protected:

    public slots:

        void displayCriticalMessage(const QString& message);

        void displayErrorMessageInBrowser(const QString& message);

        void displayErrorMessage(const QString& message);

        void displayInfoMessage(const QString& message);

    //---------------------------------------------------------------------------------
    // From Widget...

    signals:

        /**
         * @brief Сообщает об изменении пользователем значения сигнала
         */
        void controlSignalChanged(udav_drivers::SignalID, QVariant);

        void requestFileFromServer(const QString& fname);

//        /**
//         * @brief Сообщает о необходимости отображения сообщения в области вывода сообщений
//         * @param message
//         *
//         * @todo По-моему теперь сообщать нечего. Область здесь, надо сразу выводить сообщение!
//         */
//        void displayErrorMessageInBrowser(const QString& message);

    public slots:

        /**
         * @brief Восстанавить интерфейс управления оборудования на основе его описания
         * @param xml
         * @return
         *
         * Описание интерфейса передается в виде строки, содержащей текст в формате XML.
         *
         * В процессе восстановления интерфейса будут созданы необходимые страницы (EquipmentControlPage)
         * с требуемыми интерфейсными элементами.
         */
        bool restoreUi(const udav_drivers::MetaSignals& metaSignals, const QString& xml);

        /**
         * @brief Обработать изменение значения сигнала
         * @param id - код сигнала
         * @param value - значение сигнала
         * @param message - текстовое сообщение
         */
        void signalValueChanged(udav_drivers::SignalID id, const QVariant& value, const QString& message);

    private slots:

        /**
         * @brief Обработать получение файла с сервера
         * @param fname
         */
        void fileRecieved(const QString& fname);

        /**
         * @brief Сделать текущей страницу с индексом i
         * @param i - индекс страницы
         */
        void changePage_(int i);

    private:
        /**
         * @brief Добавить в панель управления страницу с интерфейсом пользователя
         * @param page
         */
        void addPage_(EquipmentControlPage* page);

        /**
         * @brief Добавить к панели управления страницу с интерфейсом пользователя, открытую в отдельном окне
         * @param page
         */
        void addWindow_(EquipmentControlPage* page);

        /**
         * @brief Удалить из панели управления страницы с интерфейсом пользователя
         */
        void removePages_();

        /**
         * @brief Удалить из панели управления страницы с интерфейсом пользователя, открытые в отдельных окнах
         */
        void removeWindows_();
};

} // namespace Gui

#endif // EQUIPMENT_WIDGET_H
