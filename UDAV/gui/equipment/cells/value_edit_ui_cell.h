#ifndef VALUEEDITUICELL_H
#define VALUEEDITUICELL_H

#include <QWidget>

class QGroupBox;
class QDoubleSpinBox;
class QPushButton;

#include "abstract_ui_cell.h"

namespace udav_gui
{

/**
 * @brief Поле ввода значения с кнопкой подтверждения и отмены
 *
 * К полю прикрепляется экранная клавиатура
 */
class ValueEditUiCell : public QWidget, public AbstractUiCell
{
    Q_OBJECT
    private:

        /**
         * @brief Элемент для отображения заголовка поля ввода
         */
        QGroupBox* groupBox_;

        /**
         * @brief Поле ввода значения
         */
        QDoubleSpinBox* spinBox_;

        QPushButton* acceptButton;

        QPushButton* revertButton;

        /**
         * @brief Показывать или нет заголовок
         */
        bool showTitle_;
        /**
         * @brief Текст заголовка
         */
        QString title_;

        /**
         * @brief Текст единицы измерения
         */
        QString unit_;

        double value_;

    public:
        explicit ValueEditUiCell(QWidget *parent, const QString& title, const QString unit);

        /**
         * @brief Вернуть текущий режим отображения заголовка
         * @return
         */
        bool showTitle() const;

        /**
         * @brief Установить режим отображения заголовка
         * @param showTitle - true: включить вывод заголовка; false: отключить вывод заголовка
         */
        void setShowTitle(bool showTitle);

        void setRange(double min, double max);

        void setValue(double value);

    protected:
        virtual bool eventFilter(QObject* object, QEvent* event);

    private slots:
        /**
         * @brief Отправить введенное значение на сервер
         */
        void acceptValue_();

        /**
         * @brief Вернуться к предудущему значению
         */
        void revertValue_();

    signals:
        /**
         * @brief Сообщает об изменении пользователем значения сигнала
         */
        void controlSignalChanged(udav_drivers::SignalID, QVariant);

    public slots:
};

//-----------------------------------------------------------------------------

inline bool
ValueEditUiCell::showTitle() const
{
    return showTitle_;
}

//-----------------------------------------------------------------------------

inline void
ValueEditUiCell::setShowTitle(bool showTitle)
{
    showTitle_ = showTitle;
}


}

#endif // VALUEEDITUICELL_H
