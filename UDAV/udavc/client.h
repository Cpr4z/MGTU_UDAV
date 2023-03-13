#ifndef NANOTOP_CLIENT_H
#define NANOTOP_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QVector>
#include <QUuid>
#include <QMultiMap>

#include "core.h"

#include "meta_signal.h"
#include "drivers.h"

namespace Core
{

//! Класс клиента системы НаноТОП

/**
    Класс клиента, реализующий подключение к серверу и взаимодействие с ним,
    в том числе двусторонний обмен данными.
*/

class Client : public QObject
{
    Q_OBJECT

    private :

        //! Массив, ожидаемых от сервера ответов.
        /**
          Используется при разборе данных, поступающих от сервера.
          В первую очередь поступающий ответ проверяется на правильность.
          Процедура отправки данных на сервер должна заполнить массив
          допустимыми вариантами ответа.

          Очистка массива производится в методе readDataFromServer()
          после обработки поступающих данных.
        */
        QVector<quint8> allowedReplies_;

        quint8 connectionMode_;

        /**
         * @brief The Anonymous:1 struct
         *
         * Этот флаг означает не то, что есть физический канал связи, а то, что процесс инициализации соединения
         * в соответствии с протоколом успешно завершен. Прошел обмен рукопожатием, выбор драйвера, и теперь в штатном
         * режиме идет обмен значениями сигналов.
         *
         */
        struct ConnectionState
        {
            bool isConnected;
            QString driverName;
            QString driverUuid;
            QDateTime conTime;
        } connectionState_;

        //! Уникальный идентификатор подключения
        QUuid connectionUuid_;

        //!	Массив значений сигналов управления
        Drivers::SignalValues controlValues_;

        //! Массив параметров настройки драйвера
        QMap<QString, QVariant> driverSettings_;

        //! Код последнего запроса, отправленного на сервер.
        /**
          Используется при анализе ответа, полученного от сервера.
        */
        quint8 lastRequest_;

        //! Массив метасигналов
        Drivers::MetaSignals metaSignals_;

        //! Размер следующего блока данных.
        /**
          Если от сервера получен размер блока 0xFFFFFFFF, то это означает, что сервер закрыл соединение.
        */
        quint32 nextBlockSize_;

        QString protocolFileName_;

        QTimer protocolTimer_;

        //! Сокет, через который осуществляется подключение к серверу.
        QTcpSocket tcpSocket_;

        //! Массив значений сигналов наблюдения
        Drivers::SignalValues viewValues_;

    public:

        explicit Client(QObject *parent = 0);

        //! Закрывает соединение с сервером.
        void closeConnection();

        //! Возвращает true, если клиент подключен к серверу. false - в противном случае.
        bool isConnected() const
        {
            return (tcpSocket_.state() == QAbstractSocket::ConnectedState);
        }

        QMap<QString, QVariant>* getDriverSettings()
        {
            return &driverSettings_;
        }

        inline quint8 connectionMode() const;

        inline void setConnectionMode(const quint8& connectionMode);

        ConnectionState getConnectionState() const
        {
            return connectionState_;
        }

    public slots:

        //! Устанавливает соединение с сервером.
        void connectToServer();

        /**
         * @brief Обработать изменение значения сигнала управления от интерфейса пользователя
         * @param signalID - номер сигнала
         * @param value - значение сигнала
         */
        void controlSignalChanged(Drivers::SignalID signalID, const QVariant& value);

        void createNewProtocol();

        //! Возвращает константную ссылку на описание метасигналов
        const Drivers::MetaSignals& getMetaSignals() const
        {
            return metaSignals_;
        }

        //! Возвращает константную ссылку на массив сигналов управления
        const Drivers::SignalValues& getControlValues() const
        {
            return controlValues_;
        }

        //! Возвращает ссылку на массив сигналов управления
        Drivers::SignalValues& getControlValues()
        {
            return controlValues_;
        }

        //! Возвращает значение сигнала наблюдения с номером signalId
        const QVariant getViewValue(Drivers::SignalID signalId) const
        {
            return viewValues_[signalId];
        }

        //! Возвращает константную ссылку на массив сигналов наблюдения
        const Drivers::SignalValues& getViewValues() const
        {
            return viewValues_;
        }

        //! Возвращает ссылку на массив сигналов наблюдения
        Drivers::SignalValues& getViewValues()
        {
            return viewValues_;
        }

        void requestFileFromServer(const QString& fname);

        /**
         * @brief Отправить на сервер запрос на перезагрузку интерфейса
         */
        void sendInterfaceReloadRequest();

        void sendDriverSettings();

    private slots:

        //! Закрывает соединение с сервером по инициативе сервера.
        void connectionClosed_();

        //! Вызывается в случае возникновения ошибки.
        void error_();

        //! Читает данные, поступающие от сервера.
        void readDataFromServer_();

        //! Отправляет серверу запрос на подключение.
        void requestConnection_();

        //! Отправляет данные на сервер.
        void sendRequest_();

        void writeProtocolString_();

    private:

        //! Проверяет полученный от сервера пакет HANDSHAKE на корректность.
        /**
          Если пакет корректен, то возвращает true, иначе false.
          \param message в случае ошибки проверки в переменную будет записано сообщение о причине.
          \return true, если проверка успешна; false, если проверка неуспешно.
        */
        bool checkHandShake_(const QString& serverName, const QString& serverVersion, QString& message);

        void sendConnectionEstablished_();

        //! Отправляет на сервер пакет connectionOK.
        /**
          Вызывается после проверки пакетов HANDSHAKE как признак установления успешного соединения.
        */
        void sendConnectionOK_();

        void sendDriversListRequest_();

        //! Отправляет на сервер пакет HANDSHAKE
        void sendHandShake_();

        //! Отправляет на сервер пакет refuseConnection.
        /**
          Отправка пакета означает отказ в соединении. Причина отказа указывается в передаваемом сообщении.
          \param reason причина отказа в соединении.
        */
        void sendRefuseConnection_(const QString& reason);

        //! Отправляет на сервер пакет, содержащий идентификатор выбранного пользователем драйвера.
        void sendSelectedDriverUuid_(const QUuid& connectionUuid_);

        //! Отправляет на сервер пакет запроса подключения клиента без предъявления билета.
        /**
          Подключение без билета исмпользуется для работы клиента с сервером в режиме виртуального
          симулятора, обработки ранее проведенных результатов и других задач, для которых не требуется
          использование реального оборудования.
        */
        void sendWithoutTicketConnectionRequest_();


    signals:

        void fileRecieved(const QString& fname);

        void textMessageRecieved(Drivers::SignalID channel, const QString& message);

//        /**
//         * @brief Сообщает о получении с сервера измененного значения сигнала
//         * @param id - номер сигнала
//         * @param value - значение сигнала
//         * @param message - текстовое сообщение
//         */
//        void signalValueChanged(Drivers::SignalID id, const QVariant& value);

        /**
         * @brief Сообщает о получении с сервера измененного значения сигнала
         * @param id - номер сигнала
         * @param value - значение сигнала
         * @param message - текстовое сообщение
         */
        void signalValueChanged(Drivers::SignalID id, const QVariant& value, const QString& message = QString());

        /**
         * @brief Сообщает о получении с сервера описания интерфейса пользователя
         * @param uiDesc - строка, содержащая описание интерфейса
         */
        void uiDescriptionReaded(const QString& uiDesc);

        /**
         * @brief Сообщает о получении с сервера списка достпных драйверов оборудования
         * @param driversList - список доступных драйверов оборудования
         */
        void driversListReaded(const QList<Core::UniversalNamedMap>& driversList);

        /**
         * @brief Сигнал формируется, когда от сервера получены данные для отрисовки мнемосхемы (визуализации)
         * @param signalID - номер сигнала
         * @param plot - алгоритм отрисовки
         */

        void openMessageWindow(const QUuid& uuid, const QString& header);

        void closeMessageWindow(const QUuid& uuid, quint16 delay, bool autoClose);

        void writeTextMessage2Window(const QUuid& uuid, quint16 type, const QString& message);

        void connectionEstablished(const QHostAddress& address);

        void connectionClosed();

        /**
         * @brief Сообщает о получении с сервера сообщения о критической ошибке
         * @param message - строка сообщения
         */
        void criticalMessageFromServer(const QString& message);

        /**
         * @brief Сообщает о получении с сервера сообщения о некритической ошибке
         * @param message - строка сообщения
         */
        void errorMessageFromServer(const QString& message);

        /**
         * @brief Сообщает о получении с сервера информационного сообщения
         * @param message - строка сообщения
         */
        void infoMessageFromServer(const QString& message);

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

//-----------------------------------------------------------------------------

quint8
Client::connectionMode() const
{
    return connectionMode_;
}

//-----------------------------------------------------------------------------

void
Client::setConnectionMode(const quint8& connectionMode)
{
    connectionMode_ = connectionMode;
}

//-----------------------------------------------------------------------------

}

#endif // NANOTOP_CLIENT_H
