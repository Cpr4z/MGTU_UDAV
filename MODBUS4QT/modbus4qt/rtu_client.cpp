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

#include <QDebug>
#include <QMutex>
#include <QWaitCondition>

#include "rtu_client.h"
#include "memory_utils.h"

namespace modbus4qt
{

RtuClient::RtuClient(const QString& portName,
                   QSerialPort::BaudRate baudRate,
                   QSerialPort::DataBits dataBits,
                   QSerialPort::StopBits stopBits,
                   QSerialPort::Parity parity,
                   QObject *parent)
    : Client(parent),
      portName_(portName),
      baudRate_(baudRate),
      dataBits_(dataBits),
      stopBits_(stopBits),
      parity_(parity),
      silenceTimer_(this),
      inSilenceState_(false),
      silenceTime_(0)
{
    ioDevice_ = new QSerialPort(this);
    serialPort_ = dynamic_cast<QSerialPort*>(ioDevice_);

    connect(this, SIGNAL(dataReaded()), this, SLOT(startSilence_()));

    serialPort_->setPortName(portName_);

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

RtuClient::RtuClient(const QString& portName,
                   QSerialPort::BaudRate baudRate,
                   QSerialPort::DataBits dataBits,
                   QSerialPort::StopBits stopBits,
                   QObject *parent)
    : Client(parent),
      portName_(portName),
      baudRate_(baudRate),
      dataBits_(dataBits),
      stopBits_(stopBits),
      parity_(QSerialPort::EvenParity),
      silenceTimer_(this),
      inSilenceState_(false),
      silenceTime_(0)
{
    ioDevice_ = new QSerialPort(this);
    serialPort_ = dynamic_cast<QSerialPort*>(ioDevice_);

    connect(this, SIGNAL(dataReaded()), this, SLOT(startSilence_()));

    serialPort_->setPortName(portName_);

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

RtuClient::RtuClient(const QString& portName,
                   QSerialPort::BaudRate baudRate,
                   QSerialPort::DataBits dataBits,
                   QObject *parent)
    : Client(parent),
      portName_(portName),
      baudRate_(baudRate),
      dataBits_(dataBits),
      stopBits_(QSerialPort::OneStop),
      parity_(QSerialPort::EvenParity),
      silenceTimer_(this),
      inSilenceState_(false),
      silenceTime_(0)
{
    ioDevice_ = new QSerialPort(this);
    serialPort_ = dynamic_cast<QSerialPort*>(ioDevice_);

    connect(this, SIGNAL(dataReaded()), this, SLOT(startSilence_()));

    serialPort_->setPortName(portName_);

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

RtuClient::RtuClient(const QString& portName,
                   QSerialPort::BaudRate baudRate,
                   QObject *parent)
    : Client(parent),
      portName_(portName),
      baudRate_(baudRate),
      dataBits_(QSerialPort::Data8),
      stopBits_(QSerialPort::OneStop),
      parity_(QSerialPort::EvenParity),
      silenceTimer_(this),
      inSilenceState_(false),
      silenceTime_(0)
{
    ioDevice_ = new QSerialPort(this);
    serialPort_ = dynamic_cast<QSerialPort*>(ioDevice_);

    connect(this, SIGNAL(dataReaded()), this, SLOT(startSilence_()));

    serialPort_->setPortName(portName_);

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

RtuClient::RtuClient(const QString& portName,
                   QObject *parent)
    : Client(parent),
      portName_(portName),
      baudRate_(QSerialPort::Baud9600),
      dataBits_(QSerialPort::Data8),
      stopBits_(QSerialPort::OneStop),
      parity_(QSerialPort::EvenParity),
      silenceTimer_(this),
      inSilenceState_(false),
      silenceTime_(0)
{
    ioDevice_ = new QSerialPort(this);
    serialPort_ = dynamic_cast<QSerialPort*>(ioDevice_);

    connect(this, SIGNAL(dataReaded()), this, SLOT(startSilence_()));

    serialPort_->setPortName(portName_);

    setSilenceTime_();
}

//-----------------------------------------------------------------------------

RtuClient::~RtuClient()
{
    serialPort_->close();
}

//-----------------------------------------------------------------------------

bool
RtuClient::configurePort_()
{
    if (!serialPort_->isOpen())
    {
        emit errorMessage(tr("Cannot configure port %1. The port is not open!").arg(portName_));
        return false;
    }

    if (!serialPort_->setBaudRate(baudRate_))
    {
        emit errorMessage(serialPort_->errorString());
        return false;
    }
    setSilenceTime_();

    if (!serialPort_->setDataBits(dataBits_))
    {
        emit errorMessage(serialPort_->errorString());
        return false;
    }

    if (!serialPort_->setStopBits(stopBits_))
    {
        emit errorMessage(serialPort_->errorString());
        return false;
    }

    if (!serialPort_->setParity(parity_))
    {
        emit errorMessage(serialPort_->errorString());
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------

bool
RtuClient::openPort()
{
    if (serialPort_->isOpen()) serialPort_->close();

    bool result = serialPort_->open(QIODevice::ReadWrite);

    if (!result)
    {
        emit errorMessage(serialPort_->errorString());
        serialPort_->close();
        return false;
    }
    else
    {
        qDebug() << "Port opened!";
        emit infoMessage(tr("Port opened!"));

        result = configurePort_();

        if (!result)
            serialPort_->close();
        else
        {
            qDebug() << "Port configured!";
            emit infoMessage(tr("Port configured!"));
        }
    }

    if (!result)
    {
        qDebug() << "Error while opening and configuring port!";
        emit errorMessage(tr("Error while opening and configuring port!"));
    }

    return result;
}

//-----------------------------------------------------------------------------

/*
 * <------------------------ MODBUS SERIAL LINE PDU (1) ------------------->
 *              <----------- MODBUS PDU (1') ---------------->
 *  +-----------+---------------+----------------------------+-------------+
 *  | Address   | Function Code | Data                       | CRC/LRC     |
 *  +-----------+---------------+----------------------------+-------------+
 *  |           |               |                                   |
 * (2)        (3/2')           (3')                                (4)
 *
 * (1)  ... MB_SER_PDU_SIZE_MAX = 256
 * (2)  ... MB_SER_PDU_ADDR_OFF = 0
 * (3)  ... MB_SER_PDU_PDU_OFF  = 1
 * (4)  ... MB_SER_PDU_SIZE_CRC = 2
 *
 * (1') ... MB_PDU_SIZE_MAX     = 253
 * (2') ... MB_PDU_FUNC_OFF     = 0
 * (3') ... MB_PDU_DATA_OFF     = 1
 */

QByteArray
RtuClient::prepareADU_(const ProtocolDataUnit &pdu, int pduSize)
{
    QByteArray result;

    result[0] = unitID_;

    result.insert(1, (char*)&pdu, pduSize);

    quint16 crc = host2net(crc16(result));
    result.append((char*)&crc, 2);

    qDebug() << "ADU: " << result.toHex();
    qDebug() << "ADU size: " << result.size();

    return result;
}

//-----------------------------------------------------------------------------

AbstractDevice::ProtocolDataUnit
RtuClient::processADU_(const QByteArray &buf)
{
    ProtocolDataUnit pdu;

    ErrorCodes errorCode;

    if (!preparePDU(buf, pdu, errorCode))
    {
        switch (errorCode)
        {
            case TOO_SHORT_ADU :
                emit errorMessage(unitID_, tr("Application data unit recieved less then 5 bytes!"));
            break;

            case CRC_MISMATCH :
                emit errorMessage(unitID_, tr("CRC mismatch!"));
            break;
            default: ; // default is empty
        }
    }

    return pdu;
}

//-----------------------------------------------------------------------------

QByteArray
RtuClient::readResponse_()
{
    QByteArray inArray;
    inArray.append(ioDevice_->readAll());
    while (ioDevice_->bytesAvailable() || ioDevice_->waitForReadyRead(silenceTime_ * 2))
    {
        inArray.append(ioDevice_->readAll());
    }
    return inArray;
}

//-----------------------------------------------------------------------------

bool
RtuClient::sendRequest_(const ProtocolDataUnit &requestPDU, int requestPDUSize, ProtocolDataUnit *responsePDU)
{
    if (inSilenceState_)
    {
#ifdef DEBUG
        qDebug() << "In silence state! Waiting...";
        emit debugMessage("In silence state! Waiting...");
#endif
        wait(silenceTime_);

#ifdef DEBUG
        qDebug() << "Waiting's been finised";
        emit debugMessage("Waiting's been finised");
#endif
    }

    if (!serialPort_->isOpen() && !openPort())
    {
        return false;
    }

    bool result = Client::sendRequest_(requestPDU, requestPDUSize, responsePDU);

    startSilence_();

    return  result;
}

//-----------------------------------------------------------------------------

void
RtuClient::setBaudRate(QSerialPort::BaudRate baudRate)
{
    if (baudRate_ != baudRate)
    {
        baudRate_ = baudRate;
        setSilenceTime_();
        if (serialPort_->isOpen()) serialPort_->setBaudRate(baudRate_);
    }
}

//-----------------------------------------------------------------------------

void
RtuClient::setDataBits(QSerialPort::DataBits dataBits)
{
    if (dataBits_ != dataBits)
    {
        dataBits_ = dataBits;
        if (serialPort_->isOpen()) serialPort_->setDataBits(dataBits_);
    }
}

//-----------------------------------------------------------------------------

void
RtuClient::setParity(QSerialPort::Parity parity)
{
    if (parity_ != parity)
    {
        parity_ = parity;
        if (serialPort_->isOpen()) serialPort_->setParity(parity_);

    }
}

//-----------------------------------------------------------------------------

void
RtuClient::setPortName(const QString& portName)
{
    if (portName != portName_)
    {
        if (serialPort_->isOpen()) serialPort_->close();
        portName_ = portName;
        serialPort_->setPortName(portName_);
    }
}

//-----------------------------------------------------------------------------

QString
RtuClient::portName() const
{
    return portName_;
}

//-----------------------------------------------------------------------------

void RtuClient::setSilenceTime_()
{
    if (baudRate_ > QSerialPort::Baud19200)
        silenceTime_ = 2; // 2 ms > 1750 microseconds
    else
        // For each byte we should transmit 11 bits
        // We have to calculate time in seconds,
        // power it by 1000 and add 1 for any cases
        //
        // Для передачи каждого байта требуется 11 бит
        // Высчитываем время в секундах
        // Умножаем на 1000 и на всякий случай прибавляем единицу
        silenceTime_ = ((11.0 / baudRate_ ) * 3.5) * 1000 + 1;

    qDebug() << QString("Silence time: %1 ms").arg(silenceTime_);
    emit infoMessage(tr("Silence time: %1 ms").arg(silenceTime_));
}

//-----------------------------------------------------------------------------

void
RtuClient::startSilence_()
{
    inSilenceState_ = true;
    silenceTimer_.singleShot(silenceTime_, this, SLOT(stopSilence_()));
}

//-----------------------------------------------------------------------------

void
RtuClient::stopSilence_()
{
    inSilenceState_ = false;
}

//-----------------------------------------------------------------------------

void
RtuClient::setStopBits(QSerialPort::StopBits stopBits)
{
    if (stopBits_ != stopBits)
    {
        stopBits_ = stopBits;
        if (serialPort_->isOpen()) serialPort_->setStopBits(stopBits_);
    }
}

} // namespace modbus4qt
