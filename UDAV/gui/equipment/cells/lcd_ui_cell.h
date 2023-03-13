#ifndef LCD_UI_CELL_H
#define LCD_UI_CELL_H

#include <QWidget>

#include "abstract_ui_cell.h"

class QLCDNumber;
class QLabel;

namespace udav_gui
{

/**
 * @brief Индикатор для вывода на экран числового значения сигнала.
 */
class LcdUiCell : public QWidget, public AbstractUiCell
{
    Q_OBJECT
    private:

        /**
         * @brief Текущее значение в стиле обычного текста
         */
        QLabel* valueText_;

        /**
         * @brief Текст заголовка
         */
        QString title_;

        /**
         * @brief Текст единицы измерения
         */
        QString unit_;

        /**
         * @brief Текстовое описание стиля отображения значения
         */
        QString style_;

        /**
         * @brief Показывать или нет заголовок
         */
        bool showTitle_;

    public:
        /**
         * @brief Конструктор по умолчанию
         * @param parent - объект владелец
         * @param title - заголовок
         * @param showTitle - показывать или нет заголовок
         * @param unit - единица измерения
         * @param style - стиль отображения
         */
        explicit LcdUiCell(QWidget *parent, const QString& title, bool showTitle, const QString& unit, const QString& style);

        /**
         * @brief Устанавливает текущее значение, выводимое индикатором
         * @param Выводимое значение
         */
        void setValue(double value);

    protected:
        void resizeEvent(QResizeEvent* event);

};

}

#endif // LCD_UI_ITEM_H
