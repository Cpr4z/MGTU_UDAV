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

#ifndef MODBUS4QT_ABSTRACT_DEVICE_H
#define MODBUS4QT_ABSTRACT_DEVICE_H

#include <QObject>

#include "global.h"

namespace modbus4qt
{

//------------------------------------------------------------------------------

//!
//! \brief Default read/write timeout, 3000 ms
//!
const int DEFAULT_TIMEOUT = 3000;

//!
//! \brief Max size of protocol data unit, 253 bytes
//!
const int PDU_MAX_SIZE = 253;

//!
//! \brief Max size of data in protocol data unit, 252 bytes
//!
const int PDU_DATA_MAX_SIZE = PDU_MAX_SIZE - 1;

//!
//! \brief Default port for MODBUS/TCP, 502
//!
const int DEFAULT_TCP_PORT = 502;

//!
//! \brief Unit ID which will be ignored (for MODBUS/TCP only) - 255
//!
const uint8_t IGNORE_UNIT_ID = 255;

//!
//! \brief Unit ID for broadcast message (for MODBUS/TCP only) - 0
//!
const uint8_t BROADCAST_UNIT_ID = 0;

//!
//! \brief Maximum coils quantity for reading - 2000
//!
//! See: Modbus Protocol Specification v1.1b3, p. 11
//!
const int MAX_COILS_FOR_READ = 2000;

//!
//! \brief Maximum coils quantity for writing - 1968
//!
//! See: Modbus Protocol Specification v1.1b3, p. 29
//!
const int MAX_COILS_FOR_WRITE = 1968;

//!
//! \brief Maximum registers quantity for reading - 125
//!
//! See: Modbus Protocol Specification v1.1b3, p. 15
//!
const int MAX_REGISTERS_FOR_READ = 125;

//!
//! \brief Maximum registers quantity for writing - 23
//!
//! See: Modbus Protocol Specification v1.1b3, p. 30
//!
const int MAX_REGISTERS_FOR_WRITE = 23;

//------------------------------------------------------------------------------

//!
//! \brief The Functions struct - place to define constants for modbus function codes.
//!
//! We cannot use enum class as some devices use user defined codes.
//!
//! Commented codes is not implemented in current version
//!
struct Functions
{
    static const uint8_t READ_COILS                          = 0x01; //! Read data block from coils table
    static const uint8_t READ_DESCRETE_INPUTS                = 0x02; //! Read data block from discrete inputs table
    static const uint8_t READ_HOLDING_REGISTERS              = 0x03; //! Read data block from holding registers table
    static const uint8_t READ_INPUT_REGISTERS                = 0x04; //! Read data block from input registers table
    static const uint8_t WRITE_SINGLE_COIL                   = 0x05; //! Write single value to coils table
    static const uint8_t WRITE_SINGLE_REGISTER               = 0x06; //! Write single value to holding registers table
    //ReadExceptionStatus               = 0x07,
    //Diagnostics                       = 0x08,
    //GetCommEventCounter               = 0x0,
    //GetCommEventLog                   = 0x0C,
    static const uint8_t WRITE_MULTIPLE_COILS                = 0x0F; //! Write data block to coils table
    static const uint8_t WRITE_MULTIPLE_REGISTERS            = 0x10; //! Write data block from holding registers table
    //ReportServerID                    = 0x11,
    //ReadFileRecord                    = 0x14,
    //WriteFileRecord                   = 0x15,
    //MaskWriteRegister                 = 0x16
    //ReadWritMultipleeRegisters        = 0x17
    //ReadFIFOQueue                     = 0x18
    //EncapsulatedInterfaceTransport    = 0x2B

};

//------------------------------------------------------------------------------

//!
//! \brief The AbstractDevice class is a base class for any device supporting modbus protocol.
//!
//! In our case abstract device is not an external device.
//! It is a PC or PLC where application runs.
//! It could be client or server.
//!
//! If you connecting to external device supporting modbus protocol you should
//! use client to make connection.
//!
//! If we want represent PC or PLC as modbus device we should use server.
//!
class AbstractDevice : public QObject
{
    Q_OBJECT
    public:

        //!
        //! \brief The ErrorCodes enum represents error codes for modbus protocol.
        //!
        enum ErrorCodes
        {
            NO_ERROR        = 0,
            TOO_SHORT_ADU   = 1,
            CRC_MISMATCH    = 2

        };
        Q_ENUM(ErrorCodes)

        //!
        //! \brief The Exception enum - modbus error codes
        //!
        //! We can use enum class as all exception codes defined in protocol.
        //!
        enum class Exceptions:uint8_t
        {
            OK                                          = 0x00, //! No errors
            ILLEGAL_FUNCTION                            = 0x01, //! Illegal function
            ILLEGAL_DATA_ADDRESS                        = 0x02, //! Illegal data address
            ILLEGAL_DATA_VALUE                          = 0x03, //! Illegal data value
            SERVER_DEVICE_FAILURE                       = 0x04, //! Server device falilure
            ACKNOWLEDGE                                 = 0x05, //! Acknowledge
            SERVER_DEVICE_BUSY                          = 0x06, //! Server device busy
            MEMORY_PARITY_ERROR                         = 0x08, //! Memory parity error
            GATEWAY_PATH_NOT_AVAILABLE                  = 0x0A, //! Gateway path not available
            GATEWAY_TARGET_DEVICE_FAILED_TO_RESPONSE    = 0x0B, //! Gateway target device failed to response
        };
        Q_ENUM(Exceptions)

        /**
         * Constants which defines the format of a modbus frame. The example is
         * shown for a Modbus RTU/ASCII frame. Note that the Modbus PDU is not
         * dependent on the underlying transport.
         *
         * <code>
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
         * </code>
         */

        /*
         *
         * <------------------------ MODBUS TCP/IP ADU(1) ------------------------->
         *              <----------- MODBUS PDU (1') ---------------->
         *  +-----------+---------------+------------------------------------------+
         *  | TID | PID | Length | UID  |Code | Data                               |
         *  +-----------+---------------+------------------------------------------+
         *  |     |     |        |      |
         * (2)   (3)   (4)      (5)    (6)
         *
         * (2)  ... MB_TCP_TID          = 0 (Transaction Identifier - 2 Byte)
         * (3)  ... MB_TCP_PID          = 2 (Protocol Identifier - 2 Byte)
         * (4)  ... MB_TCP_LEN          = 4 (Number of bytes - 2 Byte)
         * (5)  ... MB_TCP_UID          = 6 (Unit Identifier - 1 Byte)
         * (6)  ... MB_TCP_FUNC         = 7 (Modbus Function Code)
         *
         * (1)  ... Modbus TCP/IP Application Data Unit
         * (1') ... Modbus Protocol Data Unit
         */

        #pragma pack(push, 1)

        //!
        //! \brief Protocol Data Unit is used in all types of communication lines.
        //!
        //! See: Modbus Protocol Specification v1.1b3, page 5
        //!
        struct ProtocolDataUnit
        {
            //!
            //! \brief modbus function code
            //!
            uint8_t functionCode;

            //!
            //! \brief Data to send
            //!
            uint8_t data[PDU_DATA_MAX_SIZE];

            //!
            //! \brief Default constructor
            //!
            //! Fill data with zeros.
            //!
            ProtocolDataUnit() : functionCode(0)
            {
                std::fill(data, data + PDU_DATA_MAX_SIZE, 0);
            }

            //!
            //! \brief Copying constructor
            //! \param rhv - right hand value
            //!
            //! Copy data array from rhv.
            //!
            ProtocolDataUnit(const ProtocolDataUnit& rhv)
            {
                functionCode = rhv.functionCode;
                std::copy(rhv.data, rhv.data + PDU_DATA_MAX_SIZE, data);
            }

            //!
            //! \brief Copy data array from rhv
            //! \param rhv - right hand value
            //! \return Copied entity
            //!
            ProtocolDataUnit& operator=(const ProtocolDataUnit& rhv)
            {
                functionCode = rhv.functionCode;
                std::copy(rhv.data, rhv.data + PDU_DATA_MAX_SIZE, data);

                return *this;
            }
        };



//        //!
//        //! \brief The RequestResponseBuffer struct
//        //!
//        struct RequestResponseBuffer
//        {
//            //!
//            //! \brief Request/response header
//            //!
//            TcpDataHeader header;

//            //!
//            //! \brief modbus function code
//            //!
//            uint8_t functionCode;

//            //!
//            //! \brief modbus data
//            //!
//            uint8_t mbpData[261];
//        };

//        //! \brief The ExceptionBuffer struct
//        //!
//        struct ExceptionBuffer
//        {
//            //!
//            //! \brief header
//            //!
//            TcpDataHeader header;

//            //!
//            //! \brief modbus function code
//            //!
//            uint8_t function;

//            //!
//            //! \brief Exception code
//            //!
//            Exceptions exceptionCode;
//        };

        #pragma pack(pop)

        //!
        //! \brief AbstractDevice - default constructor.
        //! \param parent - Qt parent object
        //!
        explicit AbstractDevice(QObject* parent = nullptr);

    signals:

        //!
        //! \brief Signal for debugging informing
        //! \param msg - Debug message
        //!
        void debugMessage(const QString& msg);

        //!
        //! \brief Signal for informing about error occured
        //! \param msg - Message with error description
        //!
        void errorMessage(const QString& msg);

        //!
        //! \brief Signal for general purposes informing
        //! \param msg - message
        //!
        void infoMessage(const QString& msg);

    protected:

        /**
         * @brief
         * @en Process coils data readed from server and returns values of coils as array
         * @ru Разбирает полученный от буфер с данными и записывает значения дискретных входов/выходов в массив
         *
         * @param
         * @en buffer - data recieved from server
         * @ru buffer - полученный от сервера буфер с данными
         *
         * @param
         * @en regQty - quintity of coils expected
         * @ru regQty - количество ожидаемых флагов
         *
         * @return
         * @en Array of coils
         * @ru Массив флагов
         */
        QVector<bool> getCoilsFromBuffer(const QByteArray& buffer, quint16 regQty);

        /**
         * @brief
         * @en Process register data readed from server and returns values as array
         * @ru Разбирает полученный от буфер с данными и записывает значения регистров в массив
         *
         * @param
         * @en buffer - data recieved from server
         * @ru buffer - полученный от сервера буфер с данными
         *
         * @param
         * @en regQty - quintity of coils expected
         * @ru regQty - количество ожидаемых флагов
         *
         * @return
         * @en Array of values
         * @ru Массив значений регистров
         */
        QVector<quint16> getRegistersFromBuffer(const QByteArray& buffer, quint16 regQty);

        /**
         * @brief
         * @en Puts coils data into protocol data buffer
         * @ru Записывает значения флагов в буфер данных протокола
         *
         * @param
         * @en buffer - data will be writed to protocol data unit
         * @ru buffer - буфер с данными для блока данных протокола (PDU)
         *
         * @param
         * @en data - coils values
         * @ru data - массив значений флагов
         */

        void putCoilsIntoBuffer(quint8* buffer, const QVector<bool>& data);
        /**
         * @brief
         * @en Puts registers data into protocol data buffer
         * @ru Записывает значения регистров в буфер данных протокола
         *
         * @param
         * @en buffer - data will be writed to protocol data unit
         * @ru buffer - буфер с данными для блока данных протокола (PDU)
         *
         * @param
         * @en data - registers values
         * @ru data - массив значений регистров
         */
        void putRegistersIntoBuffer(quint8* buffer, const QVector<quint16>& data);

        /**
          * @brief
          * @en Wait time milliseconds
          * @ru Сделать паузу в выполнении
          *
          * @param
          * @en time - pause time, milliseconds
          * @ru time - время задержки в миллисекундах
          */
        void wait(int time);

        /**
         * @brief
         * Forms protocol data unit
         *
         * Function should be realized in descendent class since format of application
         * data unit depends from physical commincation line used.
         *
         * @param
         * pdu - Protocol Data Unit
         *
         * @param
         * pduSize - size of protocol data unit (in bytes)
         *
         * @return
         * Application Data Unit formed
         *
         * @sa ProtocolDataUnit, RTUApplicationDataUnit
         *
         */
        virtual QByteArray prepareADU_(const ProtocolDataUnit& pdu, int pduSize) = 0;

        /**
         * @brief
         * Process application data unit recieved from server and returns protocol data unit
         *
         * Function should be realized in descendent class since format of application
         * data unit depends from physical commincation line used (RTU or TCP).
         *
         * @param
         * buf - data recieved from server
         *
         * @return
         * Protocol data unit
         *
         * @sa ProtocolDataUnit, RTUApplicationDataUnit
         *
         */
        virtual ProtocolDataUnit processADU_(const QByteArray& buf) = 0;

};

} // namespace modbus4qt

#endif // MODBUS4QT_ABSTRACT_DEVICE_H
