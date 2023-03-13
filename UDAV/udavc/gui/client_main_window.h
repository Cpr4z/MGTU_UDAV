#ifndef CLIENT_MAIN_WINDOW_H
#define CLIENT_MAIN_WINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QUuid>
#include <QHostAddress>

class QPlainTextEdit;

namespace Core
{
    class Client;
}

namespace udav_gui
{

class EquipmentControlPanel;
class MessageWindow;
class GridToolButtonWidget;

/**
 * @brief Класс главного окна программы-клиента
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
class ClientMainWindow : public QMainWindow
{
    Q_OBJECT

    private :

        //--------------------------------------------
        // Внутренние данные класса

        /**
         * @brief Адрес объекта клиента, осуществляющего связь с сервером
         */
        Core::Client* client_;

        /**
         * @brief Запускать программу в полноэкранном или обычном режиме
         */
        bool fullScreen_;

        /**
         * @brief Массив открытых всплывающих окон вывода сообщений пользователю
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
        EquipmentControlPanel* equipmentControlPanel_;

        /**
         * @brief Область для вывода сообщений программы (лог-файла)
         */
        QPlainTextEdit* logMessagesView_;

        /**
         * @brief Панель для размещения кнопок основных команд управления программой (ранее ToolBox)
         */
        GridToolButtonWidget* toolsWidget_;

        //---------------------------------------------
        // Меню

        /**
         * @brief Меню "Файл"
         */
        QMenu* fileMenu_;

        /**
         * @brief Меню "Правка"
         */
        QMenu* editMenu_;

        /**
         * @brief Меню "Просмотр"
         */
        QMenu* viewMenu_;

        /**
         * @brief Меню "Протокол"
         */
        QMenu* protocolMenu_;

        /**
         * @brief Меню "Справка"
         */
        QMenu* helpMenu_;

        //---------------------------------------------
        // Команды

        /**
         * @brief Команда "О программе"
         */
        QAction* aboutAction_;

        /**
         * @brief Команда "О библиотеке Qt"
         */
        QAction* aboutQtAction_;

        /**
         * @brief Команда "Подключиться к серверу"
         */
        QAction* connectToServerAction_;

        /**
         * @brief Команда "Отключиться от сервера"
         */
        QAction* disconnectFromServerAction_;

        /**
         * @brief Команда "Перезугрузить интерфейс пользователя"
         */
        QAction* reloadIfaceAction_;

        /**
         * @brief Команда "Изменить настройки программы"
         */
        QAction* editSettingsAction_;

        /**
         * @brief Команда "Выход"
         */
        QAction* exitAction_;

        /**
         * @brief Команда "Переключение режима полного экрана"
         */
        QAction* fullScreenAction_;

        /**
         * @brief Команда "Начать новый протокол"
         */
        QAction* newProtocolAction_;

        /**
         * @brief Команда "Сохранить протокол"
         */
        QAction* saveProtocolAction_;

        /**
         * @brief Команда "Переключиться на панель управления программой"
         */
        QAction* swicth2ProgramControlPanelAction_;

    public:
        /**
         * @brief Конструктор по умолчанию
         * @param client - указатель на экземпляр класса клиента, осуществляющего связь с сервером
         * @param parent - указатель на интерфейсный элемент родителя/владельца
         */
        explicit ClientMainWindow(Core::Client* client, QWidget *parent = 0);

        /**
         * @brief Вернуть указатель на область вывода сообщений лог-файла
         * @return
         */
        QPlainTextEdit* getLogMessagesView() const
        {
            return logMessagesView_;
        }

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
         * @brief Обработать событие закрытия окна
         * @param event
         */
        virtual void closeEvent(QCloseEvent *event);

    private:

        /**
         * @brief Создать и инициализировать доступные действия
         */
        void createActions_();

        /**
         * @brief Создать и инициализировать меню программы
         */
        void createMenus_();

        /**
         * @brief Создать и инициализировать страницу кнопок и инструментов
         */
        void createToolsWidget_();

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
        void connectedToServer_(const QHostAddress& address);

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

#endif // CLIENT_MAIN_WINDOW_H
