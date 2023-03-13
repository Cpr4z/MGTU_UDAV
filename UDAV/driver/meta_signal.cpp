#include "meta_signal.h"
#include "elvactools/logger.h"

#include <QDebug>

namespace udav_drivers
{

//-----------------------------------------------------------------------------

QDataStream&
operator<<(QDataStream& os, const MetaSignal::SignalType& st)
{
    os << quint8(st);
    return os;
}

//-----------------------------------------------------------------------------

QDataStream&
operator>>(QDataStream& is, MetaSignal::SignalType& st)
{
    quint8 buf;
    is >> buf;
    st = MetaSignal::SignalType(buf);
    return is;
}

//-----------------------------------------------------------------------------

QDataStream&
operator<<(QDataStream& os, const MetaSignal::ValueType& vt)
{
    os << quint8(vt);
    return os;
}

//-----------------------------------------------------------------------------

QDataStream&
operator>>(QDataStream& is, MetaSignal::ValueType& vt)
{
    quint8 buf;
    is >> buf;
    vt = MetaSignal::ValueType(buf);
    return is;
}

//-----------------------------------------------------------------------------

QDataStream&
operator<<(QDataStream& stream, const MetaSignal& ms)
{
    stream
            << ms.fieldName
            << ms.signalType
            << ms.valueType
            << ms.minValue
            << ms.maxValue
            << ms.defaultValue
            << ms.fieldDesc;

    return stream;

}

//-----------------------------------------------------------------------------

QDataStream&
operator>>(QDataStream& stream, MetaSignal& ms)
{
    stream
            >> ms.fieldName
            >> ms.signalType
            >> ms.valueType
            >> ms.minValue
            >> ms.maxValue
            >> ms.defaultValue
            >> ms.fieldDesc;

    return stream;
}

//-----------------------------------------------------------------------------

QTextStream&
operator<<(QTextStream& stream, const MetaSignal& ms)
{
    stream
            << ms.fieldName << " : "
            << ms.signalType << " : "
            << ms.valueType << " : "
            << ms.minValue << " : "
            << ms.maxValue << " : "
            << ms.fieldDesc;

    return stream;

}

//-----------------------------------------------------------------------------

QString
MetaSignal::valueType2String(MetaSignal::ValueType type)
{
    switch (type)
    {
        case Boolean    : return QString(QObject::tr("Boolean")); break;
        case Integer    : return QString(QObject::tr("Integer")); break;
        case Real       : return QString(QObject::tr("Real")); break;
        case Time       : return QString(QObject::tr("Time")); break;
        default: return QString(QObject::tr("Unknown"));
    }
}

//-----------------------------------------------------------------------------

//Core::Logger&
//operator<<(Core::Logger& s, const MetaSignal& ms)
//{
//    s
//            << ms.fieldName << " : "
//            << ms.signalType << " : "
//            << ms.valueType << " : "
//            << ms.minValue << " : "
//            << ms.maxValue << " : "
//            << ms.fieldDesc;

//    return s;

//}

}


