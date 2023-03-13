#ifndef UDAV_SETTINGS_H
#define UDAV_SETTINGS_H

//#include <QtGlobal>

#include "elvactools/settings.h"

// В отличие от других заголовочных файлов методы и переменные класса объявлены не в алфавитном порядке,
// а сгруппированы в соответствие с порядком их хранения в файле настроек

namespace Core
{

//! Класс для работы с настройками приложения-клиента.
/**
  Обеспечивает централизованный доступ к настройкам приложения, в том числе их сохранение на диске.

  Место хранения настроект определяется операционной системой, на которой используется приложение.
  При использовании сервера на платформе Windows настройки сохраняются в реестре. При использовании системы
  семейства UNIX настройки сохраняются в текстовых файлах.
*/
class UdavSettings : public Settings
{
    Q_OBJECT

    private : // Переменные для хранения настроек
        //----------------------------------
        // Раздел "General"/"Общие настройки"

        /**
         * @brief Путь для хранения лог-файлов
         *
         * Значение по умолчанию - /var/log/udav
         */
        QString logPath_;

        //----------------------------------
        // Раздел "Connection"/"Подключение"

        bool autoConnect_;

        /**
         * @brief Имя драйвера оборудования, загружаемого при start.
         *
         * Значение по умолчанию - пустая строка.
         *
         * @sa autoConnect_
         */
        QString defaultDriverName_;

        /**
         * @brief Уникальный идентификатор драйвера оборудования, загружаемого при автоматичском подклчючении к серверу.
         *
         * Значение по умолчанию - пустая строка.
         *
         * @sa autoConnect_
         */
        QString defaultDriverUuid_;

        //----------------------------------
        // Раздел "Protocol"/"Протокол работы"

        /**
         * @brief Вести или нет протокол работы.
         *
         * Значение по умолчанию - false
         */
        bool writeProtocol_;

        /**
         * @brief Путь для сохранения файлов протокола работы.
         *
         * Значение по умолчанию - текущий каталог.
         */
        QString protocolPath_;

        /**
         * @brief Интервал сохранения режимов работы оборудования в файле протокола работы в секундах.
         *
         * Значение по умолчанию - 0. (Не сохранять файл протокола)
         */
        int protocolTimer_;

        /**
         * @brief Через сколько дней хранения удалять файлы протокола.
         *
         * Удаление производится в момент запуска программы. Значение по умолчанию 10. 0 - не удалять файлы протокола.
         */
        int deleteProtocolFilesAfterDays_;

        /**
         * @brief Путь по умолчанию для сохранения файлов протокола на внешних носителях.
         */
        QString protocolSaveToPath_;

        //----------------------------------
        // Раздел "Interface"/"Внешний вид"

        /**
         * @brief Запускать в полноэкранном режиме.
         *
         * Значение по умолчанию: false.
         */
        bool startInFullScreenMode_;

        /**
         * @brief Сохранять или нет позицию окон при их закрытии
         *
         * Значение по умолчанию: false
         */
        bool saveWindowsPos_;

        /**
         * @brief Восстанавливать или нет позицию окон при их открытии
         *
         * Значение по умолчанию: false
         */
        bool restoreWindowsPos_;

        /**
         * @brief Показывать сетку на окне управления
         *
         * Значение по умолчанию - true.
         */
        bool showGrid_;

        /**
         * @brief Размер кнопок на панели инструментов в пикселях.
         *
         * Значение по умолчанию 100.
         */
        int toolBarButtonSize_;

        /**
         * @brief Показывать или нет подписи к кнопкам панели управления.
         *
         * Значение по умолчанию true.
         */
        bool showToolBarText_;

        /**
         * @brief Показываеть или нет закладку отладки в основном окне программы
         */
        bool showDebugPage_;

        /**
         * @brief Показываеть или нет закладку лог-сообщений в основной окне программы
         */
        bool showLogMessagesPage_;

        //------------------------------------
        // Раздел "Logging"/"Лог-файлы"

        /**
         * @brief Очищать или нет лог-файлы при запуске
         */
        bool clearLogAtStart_;

    public :
        /**
         * @brief Конструктор по умолчанию. Создает экземпляр класса, инициализируя внутренние переменные.
         * @param organization Название организации-разработчика программы
         * @param application Название программы
         * @param parent Объект-родитель
         *
         * Заполненяет параметры значениями по умолчанию, а после читает их из файла настроек с
         * использованием, класса QSettings.
         */
        explicit UdavSettings(const QString& organization, const QString& application = QString(), QObject * parent = 0);

        //=====================================================
        // Методы получения/установки значений параметров (BOF)

        //----------------------------------
        // Раздел "General"/"Общие настройки"

        const QString& logPath() const;

        void setLogPath(const QString& logPath);

        //----------------------------------
        // Раздел "Connection"/"Подключение"

        /**
         * @brief autoConnect
         * @return
         */
        bool autoConnect() const;

        /**
         * @brief setAutoConnect
         * @param autoConnect
         */
        void setAutoConnect(bool autoConnect);

        /**
         * @brief defaultDriverName
         * @return
         */
        QString defaultDriverName() const;

        /**
         * @brief defaultDriverUuid
         * @return
         */
        QString defaultDriverUuid() const;

        /**
         * @brief setDefaultDriver
         * @param driverName
         * @param driverUuid
         */
        void setDefaultDriver(const QString& driverName, const QString& driverUuid);

        //----------------------------------
        // Раздел "Protocol"/"Протокол работы"

        /**
         * @brief Получить настройку включения протокола работы
         * @return Значение настройки ведения протокола работы
         */
        bool writeProtocol() const;

        /**
         * @brief Получить настройку включения протокола работы
         * @param writeProtocol - новое значение настройки включения протокола работы
         */
        void setWriteProtocol(bool writeProtocol);

        /**
         * @brief protocolPath
         * @return
         */
        QString protocolPath() const;

        /**
         * @brief setProtocolPath
         * @param protocolPath
         */
        void setProtocolPath(const QString& protocolPath);

        /**
         * @brief protocolTimer
         * @return
         */
        int protocolTimer() const;

        /**
         * @brief setProtocolTimer
         * @param protocolTimer
         */
        void setProtocolTimer(int protocolTimer);

        /**
         * @brief deleteProtocolFilesAfterDays
         * @return
         */
        int deleteProtocolFilesAfterDays() const;

        /**
         * @brief setDeleteProtocolFilesAfterDays
         * @param deleteProtocolFilesAfterDays
         */
        void setDeleteProtocolFilesAfterDays(int deleteProtocolFilesAfterDays);

        /**
         * @brief protocolSaveToPath
         * @return
         */
        QString protocolSaveToPath() const;

        /**
         * @brief setProtocolSaveToPath
         */
        void setProtocolSaveToPath(const QString& protocolSaveToPath);

        //----------------------------------
        // Раздел "Interface"/"Внешний вид"

        /**
         * @brief startInFullScreenMode
         * @return
         */
        bool startInFullScreenMode() const;

        /**
         * @brief setStartInFullScreenMode
         * @param startInFullScreenMode
         */
        void setStartInFullScreenMode(bool startInFullScreenMode);

        bool saveWindowsPos() const;

        void setSaveWindowsPos(bool saveWindowsPos);

        bool restoreWindowsPos() const;

        void setRestoreWindowsPos(bool restoreWindowsPos);

        bool showGrid() const;

        void setShowGrid(bool showGrid);

        int toolBarButtonSize() const;

        void setToolBarButtonSize(int toolBarButtonSize);

        bool showToolBarText() const;

        void setShowToolBarText(bool showToolBarText);

        bool showDebugPage() const;

        void setShowDebugPage(bool showDebugPage);

        bool showLogMessagesPage() const;

        void setShowLogMessagesPage(bool showLogMessagesPage);

        //------------------------------------
        // Раздел "Logging"/"Лог-файлы"

        bool clearLogAtStart() const;

        void setClearLogAtStart(bool clearLogAtStart);

        // Методы получения/установки значений параметров (EOF)
        //=====================================================

        /**
         * @brief Сохранить настройки программы
         *
         * Сохранение настроек реализовано с помощью класса QSettings.
         * Настройки сохраняются по правилам
         * платформы, для которой скомпилировано приложение.
         */
        virtual void save() const;

        /**
         * @brief Прочитать из файла настроек сохраненые координаты окна
         * @param windowName - имя окна, для которого читаются настройки
         * @return Прочитанные координаты окна
         */
        QRect readWindowGeometry(const QString& windowName);

        /**
         * @brief Сохранить в файле настроек координаты окна
         * @param windowName - имя окна, для которого сохраняются настройки
         * @param geometry - координаты окна
         */
        void writeWindowGeometry(const QString& windowName, const QRect& geometry);
};

}

#endif // UDAV_SETTINGS_H
