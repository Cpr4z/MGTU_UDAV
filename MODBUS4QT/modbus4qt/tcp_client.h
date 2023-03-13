/*****************************************************************************
 * modbus4qt Library
 * Author: Leonid Kolesnik, l.kolesnik@m-i.ru
 * Copyright (C) 2012-2015
 * http://mt11.net.ru
 *****************************************************************************/

/*****************************************************************************
* This file is part of modbus4qt.
*
* modbus4qt is free software: you can redistribute it and/or modify it under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option) any
* later version.
*
* modbus4qt is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
* A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with
* modbus4qt.
*
* If not, see <https://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef MODBUS_TCP_CLIENT_H
#define MODBUS_TCP_CLIENT_H


#include "client.h"
#include "tcp_device.h"

#include <QHostAddress>
#include <QTcpSocket>
#include <QVector>

namespace modbus4qt
{

//! Класс MODBUS/Tcp клиента
class MODBUS4QT_EXPORT TcpClient : public Client, public TCPDevice
{
    Q_OBJECT

    private: // Закрытые поля класса

        //! Использовать или нет режим автоматического подключения к серверу
        /**
            @ru Значение по умолчанию: истина
        */
        bool autoConnect_;

        //! Максимальное время ожидания подключения к серверу в милисекундах
        /**
          Значение по умолчанию CONNECTION_TIMEOUT 3000 ms (3 сек).
        */
        int connectTimeOut_;


        //! Номер TCP порта для связи с сервером
        /**
            Значение по умолчанию: 502
            Исходное объявление: property Port default MB_PORT;
        */
        int port_;

        //! IP адрес сервера
        /**
            Значение по умполчанию 127.0.0.1
        */
        QHostAddress serverAddress_;

        //! Внутренняя переменная для хранения указателя на ioDevice.
        /**
          Используется для обеспечения возможности вызова методов TcpSocket,
          экземпляром которого является ioDevice в контексте данного класса.
        */
        QTcpSocket* tcpSocket_;

        //! Номер последней транзакции
        /**
            Исходное объявление: FLastTransactionID: Word;
        */
        quint16 lastTransactionID_;

    private:
        //! Возвращает номер следующей транзакции
        /**
            Увеличивает номер последней транзакции на единицу и возщвращает полученное значение.
            Если номер транзакции рhttp://ru.wikipedia.org/wiki/%C1%EE%E4авен 0xFFFF, то возвращает 0.

            Готовность: полностью готова <br>
            Исходное объявление: function GetNewTransactionID: Word;
        */
        quint16 getNewTransactionID_();

        virtual bool sendRequest_(const ProtocolDataUnit& requestPDU,  int requestPDUSize, ProtocolDataUnit* responsePDU) override;

    public:

        //! Конструктор по умолчанию
        /**
            Инициализирует внутренние переменные экземпляра класса.

            Готовность: почти готов <br>
            Исходное объявление: {$IFNDEF DMB_INDY10} constructor Create(AOwner: TComponent); override; {$ENDIF}

            @todo Присвоить значения указателям обработчиков ошибок
        */
        explicit TcpClient(QObject *parent = 0);

        //! Выполняет подключение к серверу
        virtual bool connectToServer(int timeout = DEFAULT_TIMEOUT);

        //! Отключается от сервера
        virtual void disconnectFromServer();

        //! Возвращает значение режима автоматического подключения к серверу
        bool isAutoConnect() const;

        //! Проверяет наличие подключения к серверу
        /**
            Возвращает true, если клиент подключен к серверу. false - в противном случае.
        */
        virtual bool isConnected() const;

        //! Возвращает текущее значение адреса сервера
        QHostAddress getServerAddress() const;

        //! Устанавливает режим автоматического подключения к серверу
        void setAutoConnect(bool autoConnect = true);

        //! Устанавливает адрес и порт сервера для подключения
        /**
            Если ранее было установлено соединение с другим сервером, то соединение будет закрыто.
        */
        void setServerAddress(const QHostAddress& getServerAddress);

        void setServerPort(const int& port);

        // Client interface
    protected:

        virtual QByteArray prepareADU_(const ProtocolDataUnit& pdu, int pduSize) override;

        virtual ProtocolDataUnit processADU_(const QByteArray& buf) override;

        virtual QByteArray readResponse_() override;
};

} // namespace modbus

#endif // MODBUS_TCP_CLIENT_H
