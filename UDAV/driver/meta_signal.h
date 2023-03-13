#ifndef META_SIGNAL_H
#define META_SIGNAL_H

#include <QtCore>
#include <QVector>
#include <QMap>
#include <QMapIterator>

#include "drivers.h"

namespace elvactools
{
    class Logger;
}

namespace udav_drivers
{

/**
 * @brief Структура для описания сигналов, предоставляемых драйвером оборудования.
 *
 * Каждый сигнал характеризуется следующими параметрами:
 * <ul>
 * <li>типом (наблюдение или управление);
 * <li>типом значения (логическое; целочисленное; вещественное; время);
 * <li>именем сигнала;
 * <li>описанием сигнала;
 * <li>значением по умолчанию;
 * <li>диапазоном значений (только для числовых типов).
 * </ul>
 *
 *  @sa SignalType, ValueType, valueType, fieldName, fieldDesc, defaultValue, minValue, maxValue
 *
 * Массив описаний полностью дает информацию о том, какие данные можно получать от оборудования,
 * и какие данные (команды) можно ему отправлять.
 */
struct MetaSignal
{
    public :
        /**
         * @brief Типы сигналов
         */
        enum SignalType
        {
            /**
            * Наблюдение
            */
            Observation,

            /**
             *Управление
             */
            Control
        };

        /**
         * @brief Типы значений сигналов
         */
        enum ValueType
        {
            //! Логический
            Boolean = 0,

            //! Целочисленный
            Integer = 1,

            //! Число с плавающей точкой
            Real = 2,

            //! Время
            Time = 3
        };

        /**
         * @brief Имя поля
         */
        QString fieldName;

        /**
         * @brief Тип сигнала
         */
        SignalType signalType;

        /**
         * @brief Тип значения
         */
        ValueType valueType;

        /**
         * @brief Минимально допустимое значение сигнала.
         *
         * Используется только для целочисленных и числовых полей.
         * При использовании с целочисленными полями округляется до целого по правилам математики.
         */
        double minValue;

        /**
         * @brief Максимально допустимое значение сигнала.
         * Используется только для целочисленных и числовых полей.
         * При использовании с целочисленными полями округляется до целого по правилам математики.
         */
        double maxValue;

        /**
         * @brief Значение сигнала по умолчанию.
         * Если значение сигнала вещественное, то значение используется "как есть".
         * Если значение сигнала целочисленное, то значение округляется по правилам математики.
         * Если значение сигнала булево, то значению "ложь" соответствует 0.0, остальные значения соответствуют значению "истина".
         * Для полей типа время не используется.
         */
        double defaultValue;

        /**
         * @brief Описание сигнала
         * Используется в качестве комментария.
         */
        QString fieldDesc;

        MetaSignal()
        {
            minValue = 0.0;
            maxValue = 0.0;
            defaultValue = 0.0;
        }

        /**
         * @brief Возвращает список строк имен типов сигналов.
         * @return список строк имен типов сигналов.
         */
        static inline QStringList signalTypeNames()
        {
            QStringList retVal;
            retVal << QObject::tr("Observation") << QObject::tr("Control");
            return retVal;
        }

        /**
         * @brief Возвращает список имен типов значений сигналов.
         * @return список имен типов значений сигналов.
         */
        static inline QStringList valueTypeNames()
        {
            QStringList retVal;
            retVal
                    << QObject::tr("Boolean")
                    << QObject::tr("Integer")
                    << QObject::tr("Real")
                    << QObject::tr("Time");
            return retVal;
        }

        static QString valueType2String(ValueType type);

        bool operator ==(const MetaSignal& rhv) const
        {
            return
                    (fieldName == rhv.fieldName &&
                     signalType == rhv.signalType &&
                     valueType == rhv.valueType &&
                     minValue == rhv.minValue &&
                     maxValue == rhv.maxValue &&
                     defaultValue == rhv.defaultValue &&
                     fieldDesc == rhv.fieldDesc);
        }
};

/**
 * @brief Тип для хранения массива метаданных сигналов, предоставляемых драйвером оборудования.
 *
 * Значения метаданных сигналов хранятся в карте соответствия QMap<quint16, MetaSignal>.
 * Доступ к значению метаданных сигнала осуществляется по его (сигнала) индексу.
 *
 * В настроящий момент времени сигналы могут иметь номера в диапазоне от 0 до 255.
 * Для сигналов управления зарезервирован диапазон 0...127. Для сигналов управления 128...254.
 *
 * Номер 255 зарезервирован для сигнала, не имеющего привязки.
 */
typedef QMap<SignalID, MetaSignal> MetaSignals;

/**
 * @brief Итератор в стиле JAVA для доступа к элементам массива метаданных сигналов, предоставляемых драйвером оборудования.
 */
typedef QMapIterator<SignalID, MetaSignal> MetaSignalsJavaStyleIterator;

/**
 * @brief Итератор в для доступа к элементам массива метаданных сигналов, предоставляемых драйвером оборудования.
 */
typedef QMap<SignalID, MetaSignal>::iterator MetaSignalsIterator;

/**
 * @brief Константный итератор для доступа к элементам массива метаданных сигналов, предоставляемых драйвером оборудования.
 */
typedef QMap<SignalID, MetaSignal>::const_iterator MetaSignalsConstIterator;

/**
 * @brief Сохраняет в поток QDataStream значение типа сигнала.
 * @param stream - поток для вывода
 * @param st - значение типа сигнала
 * @return ссылка на поток, в который осуществлен вывод.
 *
 * При вводе значение типа преобразовывается в quint8.
 *
 * @sa MetaSignal::SignalType
 */
QDataStream& operator<<(QDataStream& stream, const MetaSignal::SignalType& st);

/**
 * @brief Читает из потока QDataStream значение типа сигнала.
 * @param stream - поток, из которого осуществляется чтение
 * @param st - читаемое значение
 * @return ссылка на поток, из которого было прочитано значение
 *
 * При чтении происходит обратное преобразование из quint8 в MetaSignal::SignalType.
 *
 * @sa MetaSignal::SignalType
 */
QDataStream& operator>>(QDataStream& stream, MetaSignal::SignalType& st);

/**
 * @brief Сохраняет в поток QDataStream значение типа значения сигнала.
 * @param stream - поток для вывода
 * @param st - значение типа значения сигнала
 * @return ссылка на поток, в который осуществлен вывод.
 *
 * При вводе значение типа преобразовывается в quint8.
 *
 * @sa MetaSignal::ValueType
 */
QDataStream& operator<<(QDataStream& stream, const MetaSignal::ValueType& st);

/**
 * @brief Читает из потока QDataStream значение типа значения сигнала.
 * @param stream - поток, из которого осуществляется чтение
 * @param st - читаемое значение
 * @return ссылка на поток, из которого было прочитано значение
 *
 * При чтении происходит обратное преобразование из quint8 в MetaSignal::ValueType.
 *
 * @sa MetaSignal::ValueType
 */
QDataStream& operator>>(QDataStream& stream, MetaSignal::ValueType& st);

/**
 * @brief Сохраняет в поток QDataStream экземлпяр структуры MetaSignal.
 * @param stream - поток для вывода
 * @param st - значение типа значения сигнала
 * @return ссылка на поток, в который осуществлен вывод.
 *
 * Вывод в поток осуществляется в следующем порядке:
 * fieldName, signalType, valueType, minValue, maxValue, fieldDesc
 *
 * @sa MetaSignal
 */
QDataStream& operator<<(QDataStream& stream, const MetaSignal& ms);

//! Читает из потока QDataStream экземлпяр класса MetaSignal
/**
 * @brief Читает из потока QDataStream экземлпяр структуры MetaSignal
 * @param stream - поток, из которого осуществляется чтение
 * @param st - читаемое значение
 * @return ссылка на поток, из которого было прочитано значение
 *
 * Чтение из потока осуществляется в следующем порядке:
 * fieldName, signalType, valueType, minValue, maxValue, fieldDesc
 *
 * @sa MetaSignal
 */
QDataStream& operator>>(QDataStream& stream, MetaSignal& ms);

/**
 * @brief Сохраняет в поток QTextStream экземлпяр структуры MetaSignal.
 * @param stream - поток для вывода
 * @param st - значение типа значения сигнала
 * @return ссылка на поток, в который осуществлен вывод.
 *
 * Вывод в поток осуществляется в следующем порядке:
 * fieldName, signalType, valueType, minValue, maxValue, fieldDesc
 *
 * Для разделения выводимых значений используется последовательность " : ".
 *
 * @sa MetaSignal
 */
QTextStream& operator<<(QTextStream& stream, const MetaSignal& ms);

/**
 * @brief Сохраняет в поток Logger экземлпяр структуры MetaSignal.
 * @param stream - поток для вывода
 * @param st - значение типа значения сигнала
 * @return ссылка на поток, в который осуществлен вывод.
 *
 * Вывод в поток осуществляется в следующем порядке:
 * fieldName, signalType, valueType, minValue, maxValue, fieldDesc
 *
 * Для разделения выводимых значений используется последовательность " : ".
 *
 * @sa MetaSignal
 */
//Core::Logger& operator<<(Core::Logger& s, const MetaSignal& ms);

}
#endif // META_SIGNAL_H
