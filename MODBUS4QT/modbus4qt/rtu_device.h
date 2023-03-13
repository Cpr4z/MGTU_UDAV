#ifndef RTUDEVICE_H
#define RTUDEVICE_H

#include "abstract_device.h"

namespace modbus4qt
{

class RTUDevice
{
    public:

        #pragma pack(push, 1)

        //!
        //! \brief Application data unit for MODBUS over serial line
        //!
        struct RTUApplicationDataUnit
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

        #pragma pack(pop)


        //!
        //! \brief Process Modbus-RTU application data unit and return protocol data unit
        //! \param buf - array with data readed, should contain ADU record
        //! \return Protocol data unit
        //!
        bool preparePDU(const QByteArray& buf, AbstractDevice::ProtocolDataUnit& pdu, AbstractDevice::ErrorCodes& errorCode);

        //!
        //! \brief Calculate the checksum of the data buffer for the crc16 algorithm
        //! \param buf - buffer to calculate crc
        //! \return CRC calculated
        //!
        //! The code of this function was taken from <a href="http://www.libmodbus.org">libmodbus</a>
        //!
        uint16_t crc16(const QByteArray& buf);


};

} // namespace modbus4qt

#endif // RTUDEVICE_H
