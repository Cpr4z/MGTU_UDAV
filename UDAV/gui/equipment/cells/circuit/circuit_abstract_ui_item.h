#ifndef CIRCUIT_ABSTRACT_UI_ITEM_H
#define CIRCUIT_ABSTRACT_UI_ITEM_H

#include <QtGlobal>

namespace udav_gui
{

/**
 * @brief Базовый класс для элементов мнемосхемы.
 *
 * Для активных элементов (кнопок) используется подкласс SymbolicCircuitButton
 * Для пассивных наследование идет напрямую - ?
 */
class CircuitAbstractUiItem
{
    public:

        /**
         * @brief Константы для описания визуализации подключения и позиции текста к элементу мнемосхемы
         */
        struct Position
        {

            static const quint8 Top		= 1;    //!< Сверху
            static const quint8 Right	= 2;    //!< Справа
            static const quint8 Bottom	= 4;    //!< Снизу
            static const quint8 Left	= 8;    //!< Слева
        };

    protected :

        /**
         * @brief Расположение подключения соседних элементов к элементу мнемосхемы
         */
        quint8 connection_;

        /**
         * @brief Расположение текста обозначения элемент мнемосхемы
         */
        quint8 textPos_;

    public:
        explicit CircuitAbstractUiItem()
            : connection_(0),
              textPos_(0)
        {
        }

        /**
         * @brief Прочитать текущее подключения к элементу мнемосхемы
         * @return Текущее значение подключения к элементу мнемосхемы
         *
         * @sa Position, connection_, setConnection()
         */
        quint8 getConnection() const
        {
            return connection_;
        }

        /**
         * @brief Задать значение визуализации подключения к элементу мнемосхемы
         *
         * @sa Position, connection_, getConnection()
         */
        void setConnection(quint8 c)
        {
            connection_ = c;
        }

        /**
         * @brief Прочитать текущее положения текста элемента мнемосхемы
         * @return Текущее значение положения текста элемента мнемосхемы
         *
         * @sa Position, textPos_, setTextPos()
         */
        quint8 textPos() const
        {
            return textPos_;
        }

        /**
         * @brief Задать положение текста элемента мнемосхемы
         * @param textPos - позиция текста
         *
         * @sa Position, textPos_, setTextPos()
         */
        void setTextPos(const quint8 &textPos)
        {
            textPos_ = textPos;
        }

};

}

#endif // CIRCUIT_ABSTRACT_UI_ITEM_H
