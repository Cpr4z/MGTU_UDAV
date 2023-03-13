#include "server_internal_data.h"

namespace modbus4qt {

ServerInternalData::ServerInternalData(QObject *parent)
    : QObject{parent}
{

}

//-----------------------------------------------------------------------------

void
ServerInternalData::addCoil(quint16 index)
{
    if (coils_.contains(index))
    {
        return;
    }

    coils_[index] = false;
    emit coilAdded(index);
}

//-----------------------------------------------------------------------------

void
ServerInternalData::addCoils(quint16 startIndex, quint16 endIndex)
{
    for (quint16 i = startIndex; i <= endIndex; i++)
    {
        addCoil(i);
    }
}

//-----------------------------------------------------------------------------

void
ServerInternalData::addDescreteInput(quint16 index)
{
    if (descreteInputs_.contains(index))
    {
        return;
    }

    descreteInputs_[index] = false;
}

//-----------------------------------------------------------------------------

void
ServerInternalData::addDescreteInputs(quint16 startIndex, quint16 endIndex)
{
    for (quint16 i = startIndex; i <= endIndex; i++)
    {
        addDescreteInput(i);
    }
}

//-----------------------------------------------------------------------------

void
ServerInternalData::addHoldingRegister(quint16 index)
{
    if (holdingRegisters_.contains(index))
    {
        return;
    }

    holdingRegisters_[index] = 0;
}

//-----------------------------------------------------------------------------

void
ServerInternalData::addHoldingRegisters(quint16 startIndex, quint16 endIndex)
{
    for (quint16 i = startIndex; i <= endIndex; i++)
    {
        addHoldingRegister(i);
    }
}

//-----------------------------------------------------------------------------

void
ServerInternalData::addInputRegister(quint16 index)
{
    if (inputRegisters_.contains(index))
    {
        return;
    }

    inputRegisters_[index] = 0;
}

//-----------------------------------------------------------------------------

void
ServerInternalData::addInputRegisters(quint16 startIndex, quint16 endIndex)
{
    for (quint16 i = startIndex; i <= endIndex; i++)
    {
        addInputRegister(i);
    }

}

//-----------------------------------------------------------------------------

QMap<quint16, bool>*
ServerInternalData::coils()
{
    return &coils_;
}

//-----------------------------------------------------------------------------

} // namespace modbus4qt

//-----------------------------------------------------------------------------
// EOF

