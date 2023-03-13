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
#include <QByteArray>
#include <QMutex>
#include <QWaitCondition>

#include "abstract_device.h"
#include "memory_utils.h"

namespace modbus4qt
{


//-----------------------------------------------------------------------------

AbstractDevice::AbstractDevice(QObject *parent)
    : QObject(parent)
{

}

//-----------------------------------------------------------------------------

QVector<bool>
AbstractDevice::getCoilsFromBuffer(const QByteArray& buffer, quint16 regQty)
{
    QVector<bool> coils(regQty);
    coils.fill(false);

    /*
    The coils in the response message are packed as one coil per bit of the data field. Status is
    indicated as 1=ON and 0=OFF. The LSB of the first data byte contains the output addressed
    in the query. The other coils follow toward the high order end of this byte, and from low order
    to high order in subsequent bytes
    */

    // For every byte from buffer
    for(int i = 0; i < buffer.size(); ++i)
    {
        quint8 bitMask = 1;
        bool breakFlag = false;

        //  For every bit from bytes
        for (int j = 0; j < 8; ++j)
        {
            int coilNum = i * 8 + j;

            if (coilNum == regQty)
            {
                breakFlag = true;
                break;
            }

            if (buffer[i] & bitMask)
            {
                coils[coilNum] = 1;
            }
            else
            {
                coils[coilNum] = 0;
            }

            bitMask = bitMask << 1;
        }

        if (breakFlag)
        {
            break;
        }
    }

    return coils;
}

//-----------------------------------------------------------------------------

QVector<quint16>
AbstractDevice::getRegistersFromBuffer(const QByteArray& buffer, quint16 regQty)
{
    QVector<quint16> regValues(regQty);
    regValues.fill(0);

    const char* charBuffer = buffer.constData();

    const quint16* ptr = (const quint16*)charBuffer;

    for (quint16 i = 0; i < regQty; ++i)
    {
        regValues[i] = net2host(*ptr);
        ++ptr;
    }

    return regValues;
}

//-----------------------------------------------------------------------------

void
AbstractDevice::putCoilsIntoBuffer(quint8* buffer, const QVector<bool>& values)
{
    quint8 bitMask = 1;
    quint8* ptr = buffer;

    int regQty = values.size();

    // Clear the buffer
    //
    for (int i = 0; i < (regQty + 7) / 8; ++i)
    {
        ptr[i] = 0x00;
    }

    // And fill values
    //
    for(int i = 0; i < regQty; ++i)
    {
        if (values[i] != 0)
        {
            *ptr |= bitMask;
        }
        else
        {
            *ptr &= ~bitMask;
        }

        if (bitMask == 0x80)
        {
            bitMask = 1;
            ++ptr;
        }
        else
        {
            bitMask = bitMask << 1;
        }

        if (i > MAX_COILS_FOR_WRITE)
        {
            break;
        }
    }
}

//-----------------------------------------------------------------------------

void
AbstractDevice::putRegistersIntoBuffer(quint8* buffer, const QVector<quint16>& data)
{
    quint16* ptr = (quint16*)buffer;

    for (quint16 i = 0; i < data.size(); ++i)
    {
        *ptr = host2net(data[i]);
        ++ptr;

        if (i > MAX_COILS_FOR_WRITE)
        {
            break;
        }
    }
}


//-----------------------------------------------------------------------------

void
AbstractDevice::wait(int time)
{
    QMutex mutex;
    mutex.lock();

    QWaitCondition pause;
    pause.wait(&mutex, time);
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt

//-----------------------------------------------------------------------------
// EOF
