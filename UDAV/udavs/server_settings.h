#ifndef SERVER_SETTINGS_H
#define SERVER_SETTINGS_H

#include "elvactools/settings.h"

// В отличие от других заголовочных файлов методы и переменные класса объявлены не в алфавитном порядке,
// а сгруппированы в соответствие с порядком их хранения в файле настроек

namespace Core
{

/**
 * @brief Класс для работы с настройками приложения-сервера.
 *
 * Обеспечивает централизованный доступ к настройкам программы-сервера, в том числе их сохранение на диске.
 *
 * Сохранение настроек производится в зависимости от операционной системы, на которой используется программа.
 * При использовании сервера на платформе Windows настройки сохраняются в реестре, при использовании системы
 * семейства UNIX настройки сохраняются в текстовых файлах.
 */
class ServerSettings : public Settings
{
    Q_OBJECT

    private : // Поля для хранения настроек

        /**
         * @brief Путь для хранения лог-файлов
         *
         * Значение по умолчанию - /var/log/udav
         * @todo Платформонезависимые значения! Issue #95
         */
        QString logPath_;


        /**
         * @brief Номер TCP порта, на котором работает сервер.
         *
         * Значение по умолчанию: 6178
         */
        quint16 port_;

        /**
         * @brief Интервал проверки соединения с клиентом в секундах. 0 - не проверять соединение.
         *
         * Значение по умолчанию: 10
         */
        quint16 connectCheckingIterval_;

        /**
         * @brief Число потерянных пакетов проверки соединения, после которых связь с клиентом считается потерянной.
         *
         * Значение по умолчанию: 3
         */
        quint8 connectLostAfterAttempts_;

        /**
         * @brief Интервал отправки данных клиенту в секундах. 0 - не отправлять данные клиенту.
         *
         * Значение по умолчанию: 15
         */
        quint16 sendDataInterval_;

        /**
         * @brief Путь к каталогу с драйверами оборудования.
         *
         * Значение по умолчанию: /usr/local/lib/udav
         */
        QString pathToDrivers_;

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
        explicit ServerSettings(const QString& organization, const QString& application = QString(), QObject * parent = 0);

        //=====================================================
        // Методы получения/установки значений параметров (BOF)

        inline const QString& logPath() const;

        inline void setLogPath(const QString& logPath);

        /**
         * @brief Возвращает номер порта, на котором работает сервер.
         * @return номер порта, на котором работает сервер.
         */
        inline quint16 port() const;

        /**
         * @brief Устанавливает номер порта, на котором работает сервер.
         * @param port - номер порта
         */
        inline void setPort(quint16 port);

        /**
         * @brief Возвращает интервал в секундах, через который производится проверка наличия содеинения с клиентом.
         * @return интервал в секундах, через который производится проверка наличия содеинения с клиентом.
         */
        inline quint16 connectCheckingIterval() const;

        /**
         * @brief Устанавливает интервал в секундах, через который производится проверка наличия содеинения с клиентом.
         * @param connectCheckingIterval - интервал в секундах, через который производится проверка наличия содеинения с клиентом.
         */
        inline void setConnectCheckingIterval(quint16 connectCheckingIterval);

        /**
         * @brief Возвращает число неудачных попыток проверки соединения с клиентом, через которое соединение считается потерянным.
         * @return число неудачных попыток проверки соединения с клиентом, через которое соединение считается потерянным.
         */
        inline quint8 connectLostAfterAttempts() const;

        /**
         * @brief Устанавливает число неудачных попыток проверки соединения с клиентом, через которое соединение считается потерянным.
         * @param newConnectLostAfterAttempts - число неудачных попыток проверки соединения с клиентом, через которое соединение считается потерянным.
         */
        inline void setConnectLostAfterAttempts(quint16 connectLostAfterAttempts);

        /**
         * @brief Возвращает интервал в секундах, через который осуществляется отправка клиенту данных сигналов наблюдения.
         * @return интервал в секундах, через который осуществляется отправка клиенту данных сигналов наблюдения.
         */
        inline quint16 sendDataInterval() const;

        /**
         * @brief Устанавливает интервал в секундах, через который осуществляется отправка клиенту данных сигналов наблюдения.
         * @param sendDataInterval - интервал в секундах, через который осуществляется отправка клиенту данных сигналов наблюдения.
         */
        inline void setSendDataInterval(quint16 sendDataInterval);

        /**
         * @brief Возвращает текущее значение пути папки с драйверами оборудования
         * @return
         */
        inline QString pathToDrivers() const;

        /**
         * @brief Задает текущее значение пути папки с драйверами оборудования
         * @param pathToDrivers
         */
        inline void setPathToDrivers(const QString& pathToDrivers);

        inline bool clearLogAtStart() const;

        inline void setClearLogAtStart(bool clearLogAtStart);

        // Методы получения/установки значений параметров (EOF)
        //=====================================================

        /**
         * @brief Сохраняет настройки программы.
         *
         * Сохранение настроек реализовано с помощью класса QSettings. Настройки сохраняются по правилам
         * платформы, на которой работает программа.
         */
        virtual void save() const;

};


//-----------------------------------------------------------------------------

const QString&
ServerSettings::logPath() const
{
    return logPath_;
}

//-----------------------------------------------------------------------------

void
ServerSettings::setLogPath(const QString& logPath)
{
    logPath_ = logPath;
}


//-----------------------------------------------------------------------------

quint16
ServerSettings::port() const
{
    return port_;
}

//-----------------------------------------------------------------------------

void
ServerSettings::setPort(quint16 port)
{
    port_ = port;
}

//-----------------------------------------------------------------------------

quint16
ServerSettings::connectCheckingIterval() const
{
    return connectCheckingIterval_;
}

//-----------------------------------------------------------------------------

void
ServerSettings::setConnectCheckingIterval(quint16 connectCheckingIterval)
{
    connectCheckingIterval_ = connectCheckingIterval;
}

//-----------------------------------------------------------------------------

quint8
ServerSettings::connectLostAfterAttempts() const
{
    return connectLostAfterAttempts_;
}

//-----------------------------------------------------------------------------

void
ServerSettings::setConnectLostAfterAttempts(quint16 connectLostAfterAttempts)
{
    connectLostAfterAttempts_ = connectLostAfterAttempts;
}

//-----------------------------------------------------------------------------

quint16
ServerSettings::sendDataInterval() const
{
    return sendDataInterval_;
}

//-----------------------------------------------------------------------------

void
ServerSettings::setSendDataInterval(quint16 sendDataInterval)
{
    sendDataInterval_ = sendDataInterval;
}

//-----------------------------------------------------------------------------

QString
ServerSettings::pathToDrivers() const
{
    return pathToDrivers_;
}

//-----------------------------------------------------------------------------

void
ServerSettings::setPathToDrivers(const QString& pathToDrivers)
{
    pathToDrivers_ = pathToDrivers;
}

//-----------------------------------------------------------------------------

bool
ServerSettings::clearLogAtStart() const
{
    return clearLogAtStart_;
}

//-----------------------------------------------------------------------------

void
ServerSettings::setClearLogAtStart(bool clearLogAtStart)
{
    clearLogAtStart_ = clearLogAtStart;
}

//-----------------------------------------------------------------------------

}

#endif // SERVER_SETTINGS_H
