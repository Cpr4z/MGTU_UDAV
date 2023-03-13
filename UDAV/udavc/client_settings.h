#ifndef CLIENT_SETTINGS_H
#define CLIENT_SETTINGS_H

#include <QtGlobal>
#include <QHostAddress>

#include "elvactools/settings.h"

// В отличие от других заголовочных файлов методы и переменные класса объявлены не в алфавитном порядке,
// а сгруппированы в соответствие с порядком их хранения в файле настроек

namespace Core
{

//! Класс для работы с настройками приложения-клиента.
/**
  Обеспечивает централизованный доступ к настройкам приложения-клиента, в том числе их сохранение на диске.

  Место хранения настроект определяется операционной системой, на которой используется приложение.
  При использовании сервера на платформе Windows настройки сохраняются в реестре. При использовании системы
  семейства UNIX настройки сохраняются в текстовых файлах.
*/
class ClientSettings : public Settings
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

        /**
         * @brief IP адрес сервера.
         *
         * Значение по умолчанию - 127.0.0.1
         */
        QHostAddress serverAddress_;

        /**
         * @brief Порт для установления соединения с сервером.
         *
         * Значение по умолчанию - 6178.
         */
        quint16 serverPort_;

        /**
         * @brief Использовать кэширование файлов, получаемых с сервера.
         *
         * Значение по умолчанию - true.
         */
        bool cacheFiles_;

        /**
         * @brief Автоматически подключаться к серверу при запуске и загружать драйвер, указанный в настройках.
         *
         * Значение по умолчанию - false.
         *
         * @sa defaultDriverUuid_, defaultDriverName_
         */
        bool autoConnect_;

        /**
         * @brief Имя драйвера оборудования, загружаемого при автоматичском подклчючении к серверу.
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
        quint16 protocolTimer_;

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
        explicit ClientSettings(const QString& organization, const QString& application = QString(), QObject * parent = 0);

        //=====================================================
        // Методы получения/установки значений параметров (BOF)

        //----------------------------------
        // Раздел "General"/"Общие настройки"

        inline const QString& logPath() const;

        inline void setLogPath(const QString& logPath);

        //----------------------------------
        // Раздел "Connection"/"Подключение"

        /**
         * @brief Возвращает значение настройки IP адреса сервера.
         * @return IP адрес сервера.
         */
        inline const QHostAddress& serverAddress() const;

        /**
         * @brief Устанавливает значение настройки IP адреса сервера.
         * @param serverAddress - новое значение IP адреса сервера.
         *
         * Смена IP адреса сервера не приводит к изменению параметров открытого соединения.
         */
        inline void setServerAddress(const QHostAddress& serverAddress);

        //!
        /**
         * @brief Возвращает значение настройки номера порта для установки связи с сервером.
         * @return Номер порта для установки связи с сервером.
         */
        inline quint16 serverPort() const;

        /**
         * @brief Устанавливает значение настройки номера порта для связи с сервером.
         * @param newPort - новое значение номера порта сервера.
         *
         * Смена порта сервера не приводит к изменению параметров открытого соединения.
         */
        inline void setPortNumber(quint16 serverPort);

        /**
         * @brief Возвращает режим кэширования файлов.
         * @return true - режим кэширования файлов включен, false - выключен.
         */
        inline bool cacheFiles() const;

        /**
         * @brief Задает режим кэширования файлов.
         * @param cacheFiles: true - включить режим кэширования файлов, false - выключить
         */
        inline void setCacheFiles(bool cacheFiles);

        /**
         * @brief autoConnect
         * @return
         */
        inline bool autoConnect() const;

        /**
         * @brief setAutoConnect
         * @param autoConnect
         */
        inline void setAutoConnect(bool autoConnect);

        /**
         * @brief defaultDriverName
         * @return
         */
        inline QString defaultDriverName() const;

        /**
         * @brief defaultDriverUuid
         * @return
         */
        inline QString defaultDriverUuid() const;

        /**
         * @brief setDefaultDriver
         * @param driverName
         * @param driverUuid
         */
        inline void setDefaultDriver(const QString& driverName, const QString& driverUuid);

        //----------------------------------
        // Раздел "Protocol"/"Протокол работы"

        /**
         * @brief Получить настройку включения протокола работы
         * @return Значение настройки ведения протокола работы
         */
        inline bool writeProtocol() const;

        /**
         * @brief Получить настройку включения протокола работы
         * @param writeProtocol - новое значение настройки включения протокола работы
         */
        inline void setWriteProtocol(bool writeProtocol);

        /**
         * @brief protocolPath
         * @return
         */
        inline QString protocolPath() const;

        /**
         * @brief setProtocolPath
         * @param protocolPath
         */
        inline void setProtocolPath(const QString& protocolPath);

        /**
         * @brief protocolTimer
         * @return
         */
        inline int protocolTimer() const;

        /**
         * @brief setProtocolTimer
         * @param protocolTimer
         */
        inline void setProtocolTimer(int protocolTimer);

        /**
         * @brief deleteProtocolFilesAfterDays
         * @return
         */
        inline int deleteProtocolFilesAfterDays() const;

        /**
         * @brief setDeleteProtocolFilesAfterDays
         * @param deleteProtocolFilesAfterDays
         */
        inline void setDeleteProtocolFilesAfterDays(int deleteProtocolFilesAfterDays);

        /**
         * @brief protocolSaveToPath
         * @return
         */
        inline QString protocolSaveToPath() const;

        /**
         * @brief setProtocolSaveToPath
         */
        inline void setProtocolSaveToPath(const QString& protocolSaveToPath);

        //----------------------------------
        // Раздел "Interface"/"Внешний вид"

        /**
         * @brief startInFullScreenMode
         * @return
         */
        inline bool startInFullScreenMode() const;

        /**
         * @brief setStartInFullScreenMode
         * @param startInFullScreenMode
         */
        inline void setStartInFullScreenMode(bool startInFullScreenMode);

        inline bool saveWindowsPos() const;

        inline void setSaveWindowsPos(bool saveWindowsPos);

        inline bool restoreWindowsPos() const;

        inline void setRestoreWindowsPos(bool restoreWindowsPos);

        inline bool showGrid() const;

        inline void setShowGrid(bool showGrid);

        inline int toolBarButtonSize() const;

        inline void setToolBarButtonSize(int toolBarButtonSize);

        inline bool showToolBarText() const;

        inline void setShowToolBarText(bool showToolBarText);

        inline bool showDebugPage() const;

        inline void setShowDebugPage(bool showDebugPage);

        inline bool showLogMessagesPage() const;

        inline void setShowLogMessagesPage(bool showLogMessagesPage);

        //------------------------------------
        // Раздел "Logging"/"Лог-файлы"

        inline bool clearLogAtStart() const;

        inline void setClearLogAtStart(bool clearLogAtStart);

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

//-----------------------------------------------------------------------------

const QString&
ClientSettings::logPath() const
{
    return logPath_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setLogPath(const QString& logPath)
{
    logPath_ = logPath;
}

//-----------------------------------------------------------------------------

const QHostAddress&
ClientSettings::serverAddress() const
{
    return serverAddress_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setServerAddress(const QHostAddress& serverAddress)
{
    serverAddress_ = serverAddress;
}

//-----------------------------------------------------------------------------

quint16
ClientSettings::serverPort() const
{
    return serverPort_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setPortNumber(quint16 serverPort)
{
    serverPort_ = serverPort;
}

//-----------------------------------------------------------------------------

bool
ClientSettings::cacheFiles() const
{
    return cacheFiles_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setCacheFiles(bool cacheFiles)
{
    cacheFiles_ = cacheFiles;
}

//-----------------------------------------------------------------------------

bool
ClientSettings::autoConnect() const
{
    return autoConnect_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setAutoConnect(bool autoConnect)
{
    autoConnect_ = autoConnect;
}

//-----------------------------------------------------------------------------

QString
ClientSettings::defaultDriverName() const
{
    return defaultDriverName_;
}

//-----------------------------------------------------------------------------

QString
ClientSettings::defaultDriverUuid() const
{
    return defaultDriverUuid_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setDefaultDriver(const QString& driverName, const QString& driverUuid)
{
    defaultDriverName_ = driverName;
    defaultDriverUuid_ = driverUuid;
}

//-----------------------------------------------------------------------------

bool ClientSettings::writeProtocol() const
{
    return writeProtocol_;
}

//-----------------------------------------------------------------------------

void ClientSettings::setWriteProtocol(bool writeProtocol)
{
    writeProtocol_ = writeProtocol;
}

//-----------------------------------------------------------------------------

QString
ClientSettings::protocolPath() const
{
    return protocolPath_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setProtocolPath(const QString& protocolPath)
{
    protocolPath_ = protocolPath;
}

//-----------------------------------------------------------------------------

int
ClientSettings::protocolTimer() const
{
    return protocolTimer_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setProtocolTimer(int protocolTimer)
{
    protocolTimer_ = protocolTimer;
}

//-----------------------------------------------------------------------------

int
ClientSettings::deleteProtocolFilesAfterDays() const
{
    return deleteProtocolFilesAfterDays_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setDeleteProtocolFilesAfterDays(int deleteProtocolFilesAfterDays)
{
    deleteProtocolFilesAfterDays_ = deleteProtocolFilesAfterDays;
}

//-----------------------------------------------------------------------------

QString
ClientSettings::protocolSaveToPath() const
{
    return protocolSaveToPath_;

}

//-----------------------------------------------------------------------------

void
ClientSettings::setProtocolSaveToPath(const QString& protocolSaveToPath)
{
    protocolSaveToPath_ = protocolSaveToPath;
}

//-----------------------------------------------------------------------------

bool
ClientSettings::startInFullScreenMode() const
{
    return startInFullScreenMode_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setStartInFullScreenMode(bool startInFullScreenMode)
{
    startInFullScreenMode_ = startInFullScreenMode;
}

//-----------------------------------------------------------------------------

bool
ClientSettings::saveWindowsPos() const
{
    return saveWindowsPos_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setSaveWindowsPos(bool saveWindowsPos)
{
    saveWindowsPos_ = saveWindowsPos;
}

//-----------------------------------------------------------------------------

bool
ClientSettings::restoreWindowsPos() const
{
    return restoreWindowsPos_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setRestoreWindowsPos(bool restoreWindowsPos)
{
    restoreWindowsPos_ = restoreWindowsPos;
}

//-----------------------------------------------------------------------------

bool
ClientSettings::showGrid() const
{
    return showGrid_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setShowGrid(bool showGrid)
{
    showGrid_ = showGrid;
}

//-----------------------------------------------------------------------------

int
ClientSettings::toolBarButtonSize() const
{
    return toolBarButtonSize_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setToolBarButtonSize(int toolBarButtonSize)
{
    toolBarButtonSize_ = toolBarButtonSize;
}

//-----------------------------------------------------------------------------

bool
ClientSettings::showToolBarText() const
{
    return showToolBarText_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setShowToolBarText(bool showToolBarText)
{
    showToolBarText_ = showToolBarText;
}

//-----------------------------------------------------------------------------

bool
ClientSettings::showDebugPage() const
{
    return showDebugPage_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setShowDebugPage(bool showDebugPage)
{
    showDebugPage_ = showDebugPage;
}

//-----------------------------------------------------------------------------

bool
ClientSettings::showLogMessagesPage() const
{
    return showLogMessagesPage_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setShowLogMessagesPage(bool showLogMessagesPage)
{
    showLogMessagesPage_ = showLogMessagesPage;
}

//-----------------------------------------------------------------------------

bool
ClientSettings::clearLogAtStart() const
{
    return clearLogAtStart_;
}

//-----------------------------------------------------------------------------

void
ClientSettings::setClearLogAtStart(bool clearLogAtStart)
{
    clearLogAtStart_ = clearLogAtStart;
}


}

#endif // CLIENT_SETTINGS_H
