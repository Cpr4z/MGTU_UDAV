#ifndef TCPDEVICE_H
#define TCPDEVICE_H

#include "abstract_device.h"

namespace modbus4qt
{

class TCPDevice
{
    public:

        #pragma pack(push, 1)

        //!
        //! \brief The TCPApplicationDataUnit struct
        //!
        struct TCPApplicationDataUnit
        {
            //!
            //! \brief Address of device
            //!
            //! Should be from 1 to 247. Range 248-255 is reserved by protocol specification.
            //!
            uint8_t unitId;

            //!
            //! \brief Protocol data unit
            //!
            //! \sa ProtocolDataUnit
            //!
            AbstractDevice::ProtocolDataUnit pdu;

            //!
            //! \brief Error checking field is the result of a "Redundancy Checking"
            //!
            //! CRC calculation is performed on the message content.
            //!
            uint16_t crc;
        };

        //!
        //! \brief The The TcpDataHeader struct
        //!
        struct TcpDataHeader
        {
            /**
             * @brief
             * @en Transaction ID
             * @ru Номер транзакции
             */

            //!
            //! \brief Transaction ID
            //!
            uint16_t transactionId;

            //!
            //! \brief Protocol ID
            //!
            uint16_t protocolId;

            //!
            //! \brief Data packet length
            //!
            uint16_t recLength;

            //! Адрес подчинённого устройства, к которому адресован запрос
            /**
                Обычно игнорируется, если соединение установлено с конкретным устройством.
                Может использоваться, если соединение установлено с мостом,
                который выводит нас, например, в сеть RS485.
            */
            uint8_t unitId;
        };

        #pragma pack(pop)
};

}

#endif // TCPDEVICE_H
