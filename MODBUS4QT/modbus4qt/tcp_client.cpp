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

#include <QDateTime>
#include <QDebug>

#include "tcp_client.h"
#include "memory_utils.h"

namespace modbus4qt
{

//-----------------------------------------------------------------------------

TcpClient::TcpClient(QObject *parent) :
    Client(parent)
    , connectTimeOut_(DEFAULT_TIMEOUT)
    , lastTransactionID_(0)
{
    autoConnect_		= true;
    port_				= DEFAULT_TCP_PORT;
    serverAddress_		= QHostAddress("127.0.0.1");
    unitID_				= IGNORE_UNIT_ID;

    ioDevice_ = new QTcpSocket(this);
    tcpSocket_ = dynamic_cast<QTcpSocket*>(ioDevice_);

//    onResponseError = nullptr;
//    onResponseMismatch = nullptr;
}

//-----------------------------------------------------------------------------

bool
TcpClient::connectToServer(int timeout)
{
    tcpSocket_->connectToHost(serverAddress_, port_);

    if (!tcpSocket_->waitForConnected(timeout))
    {
        emit errorMessage(tcpSocket_->errorString());
        return false;
    }

    lastTransactionID_ = 0;

    return true;
}

//-----------------------------------------------------------------------------

void
TcpClient::disconnectFromServer()
{
    tcpSocket_->disconnectFromHost();
}

////-----------------------------------------------------------------------------

//void
//TcpClient::doResponseError_(quint8 functionCode, quint8 errorCode, const ResponseBuffer& responseBuffer)
//{
//}

//-----------------------------------------------------------------------------

//void
//TcpClient::doResponseMismatch_(quint8 requestFunctionCode, quint8 responseFunctionCode, const ResponseBuffer& responseBuffer)
//{
//}

//-----------------------------------------------------------------------------

quint16
TcpClient::getNewTransactionID_()
{
    if (lastTransactionID_ == 0xFFFF)
    {
        lastTransactionID_ = 0;
    }
    else
    {
        ++lastTransactionID_;
    }

    return lastTransactionID_;
}

//-----------------------------------------------------------------------------

QHostAddress
TcpClient::getServerAddress() const
{
    return serverAddress_;
}

//-----------------------------------------------------------------------------

bool
TcpClient::isAutoConnect() const
{
    return autoConnect_;
}

//-----------------------------------------------------------------------------

bool
TcpClient::isConnected() const
{
    return (tcpSocket_->state() == QAbstractSocket::ConnectedState);
}

//-----------------------------------------------------------------------------

QByteArray
TcpClient::prepareADU_(const ProtocolDataUnit& pdu, int pduSize)
{
    QByteArray result;

    getNewTransactionID_();

    result.append((char*)&lastTransactionID_, 2);

    result.append('\x0');
    result.append('\x0');

    result[4] = ((pduSize+1) >> 8) & 0xFF;
    result[5] = (pduSize+1) & 0xFF;

    result.append(unitID_);

    result.insert(7, (char*)&pdu, pduSize);

    qDebug() << "ADU: " << result.toHex();
    qDebug() << "ADU size: " << result.size();

    return result;
}

//-----------------------------------------------------------------------------

AbstractDevice::ProtocolDataUnit
TcpClient::processADU_(const QByteArray& buf)
{
    qDebug() << "ADU: " << buf.toHex();
    qDebug() << "ADU size: " << buf.size();

    QByteArray tempBuf(buf);

    // For any case we should check size of recieved data to protect memory
    //
    // На всякий случай проверяем размер полученных данных, чтобы не ничего не испортить
    //
    if (tempBuf.size() > int(sizeof(TcpDataHeader)+sizeof(TCPApplicationDataUnit)))
        tempBuf.resize(sizeof(TcpDataHeader)+sizeof(TCPApplicationDataUnit));

    int tempBufSize = tempBuf.size();

    // Minimum ADU size can be 9 bytes: 2 byte for Transaction ID, 2 bytes for Protocol ID,
    // 2 byte for Length, 1 byte for address, 2 byte for minimum PDU
    //
    // Минимальный размер ADU может быть 9 байт: 2 байт транзакция ИД, 2 байт Протокол ИД,
    // 2 байт длина, 1 байт адрес и 2 байта PDU
    //
    if (tempBufSize < 9)
    {
        emit errorMessage(unitID_, tr("Wrong application data unit recieved!"));
        tempBuf.resize(9);
        tempBufSize = 9;
    }

    TcpDataHeader header;

    WordRec headerTID;
    headerTID.bytes[0] = tempBuf[0];
    headerTID.bytes[1] = tempBuf[1];

    header.transactionId = headerTID.word;

    WordRec headerPID;
    headerPID.bytes[0] = tempBuf[2];
    headerPID.bytes[1] = tempBuf[3];

    header.protocolId = headerPID.word;

    WordRec headerLength;
    headerLength.bytes[0] = tempBuf[4];
    headerLength.bytes[1] = tempBuf[5];

    header.recLength = headerLength.word;

    header.unitId = tempBuf[6];

    TCPApplicationDataUnit adu;

    adu.unitId = tempBuf[6];
    adu.pdu.functionCode = tempBuf[7];

    for (int i = 8; i < tempBufSize; ++i)
        adu.pdu.data[i - 8] = tempBuf[i];

#ifndef QT_NO_DEBUG
    // If we want to print PDU into log file we should remove first 7 byte
    // After that we have pdu in tempBuf
    //
    tempBuf.remove(0, 7);
    qDebug() << "PDU: " << tempBuf.toHex();
    qDebug() << "PDU size: " << tempBuf.size();
#endif

    return adu.pdu;
}

//-----------------------------------------------------------------------------

QByteArray
TcpClient::readResponse_()
{
    QByteArray inArray;
    inArray.append(ioDevice_->readAll());

    while (ioDevice_->bytesAvailable() || ioDevice_->waitForReadyRead(connectTimeOut_))
    {
        inArray.append(ioDevice_->readAll());
    }

    return inArray;
}

//-----------------------------------------------------------------------------

bool
TcpClient::sendRequest_(const ProtocolDataUnit& requestPDU, int requestPDUSize, ProtocolDataUnit* responsePDU)
{
    if (autoConnect_ && !isConnected())
    {
        if (!connectToServer())
        {
            emit infoMessage(tr("Cannot connect to server!"));

            return false;
        }
    }

    bool result = Client::sendRequest_(requestPDU, requestPDUSize, responsePDU);
    return  result;
}

//-----------------------------------------------------------------------------

void
TcpClient::setAutoConnect(bool autoConnect)
{
    autoConnect_ = autoConnect;
}

//-----------------------------------------------------------------------------

void
TcpClient::setServerAddress(const QHostAddress& serverAddress)
{
    if (serverAddress != serverAddress_)
    {
        if (isConnected())
        {
            disconnectFromServer();
        }

        serverAddress_ = serverAddress;
    }
}

//-----------------------------------------------------------------------------

void
TcpClient::setServerPort(const int& port)
{
    if (port != port_)
    {
        if (isConnected())
        {
            disconnectFromServer();
        }

        port_ = port;
    }
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt

// EOF


