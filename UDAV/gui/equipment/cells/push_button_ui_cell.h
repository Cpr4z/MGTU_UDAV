#ifndef PUSH_BUTTON_UI_ITEM_H
#define PUSH_BUTTON_UI_ITEM_H

#include <QPushButton>
#include <QMap>

#include "abstract_ui_cell.h"

namespace udav_gui
{

/**
 * @brief Кнопка включения-выключения элементов оборудования.
 */
class PushButtonUiCell :  public QPushButton, public AbstractUiCell
{
    Q_OBJECT
    private:

        /**
         * @brief Вопрос, задаваемый пользователю при переключении кнопки в состояние "Включено"
         */
        QString onQuestion_;

        /**
         * @brief Режим вопроса, задаваемого пользователю при переключении кнопки в состояние "Включено"
         */
        bool onQuestionOn_;

        /**
         * @brief Вопрос, задаваемый пользователю при переключении кнопки в состояние "Выключено"
         */
        QString offQuestion_;

        /**
         * @brief Режим вопроса, задаваемого пользователю при переключении кнопки в состояние "Выключено"
         */
        bool offQuestionOn_;

    public:
        /**
         * @brief Конструктор по умолчанию
         * @param parent - объект владелец/родитель
         */
        explicit PushButtonUiCell(QWidget *parent = 0);

        /**
         * @brief Вернуть текст вопроса, который должен быть задан пользователю при переключении кнопки в состояние "Выключено".
         * @return Текст вопроса, который должен быть задан пользователю при переключении кнопки в состояние "Выключено".
         */
        QString offQuestion() const;

        /**
         * @brief Вернусть статус включения режима вопроса пользователю при переключении кнопки в состояние "Выключено".
         * @return true - режим вопроса включен; false - выключен.
         */
        bool offQuestionOn() const;

        /**
         * @brief Вернуть текст вопроса, который должен быть задан пользователю при переключении кнопки в состояние "Включено".
         * @return Текст вопроса, который должен быть задан пользователю при переключении кнопки в состояние "Включено".
         */
        QString onQuestion() const;

        /**
         * @brief Вернуть статус включения режима вопроса пользователю при переключении кнопки в состояние "Включено".
         * @return true - режим вопроса включен; false - выключен.
         */
        bool onQuestionOn() const;

        /**
         * @brief setOnQuestion
         * @param onQuestion
         */
        void setOnQuestion(const QString& onQuestion);

        /**
         * @brief setOffQuestion
         * @param offQuestion
         */
        void setOffQuestion(const QString& offQuestion);

        /**
         * @brief setOnQuestionOn
         * @param onQuestionOn
         */
        void setOnQuestionOn(bool onQuestionOn);

        /**
         * @brief setOffQuestionOn
         * @param offQuestionOn
         */
        void setOffQuestionOn(bool offQuestionOn);

        virtual void setSignalId(udav_drivers::SignalID signalId);

    protected:
        /**
         * @brief paintEvent
         */
        virtual void paintEvent(QPaintEvent *);

        /**
         * @brief This virtual handler is called when a button is clicked.
         *
         * The default implementation calls setChecked(!isChecked()) if the button isCheckable().
         * It allows subclasses to implement intermediate button states.
         */
        virtual void nextCheckState();

    private:

        /**
         * @brief Отрисовать кнопку на экране
         * @param painter
         */
        void draw_(QPainter* painter);

    private slots:
        /**
         * @brief Вызывается при изменении состоянии кнопки
         * @param checked - новое состояние кнопки
         *
         * Формирует сигнал toggled(PushButtonUiItem* button, bool checked) с указанием нового состония кнопки.
         */
        void buttonToggled_(bool checked);

    signals:
        /**
         * @brief Сообщает об изменении пользователем значения сигнала
         */
        void controlSignalChanged(udav_drivers::SignalID, QVariant);
};

//-----------------------------------------------------------------------------

inline void
PushButtonUiCell::buttonToggled_(bool state)
{
    emit controlSignalChanged(signalId_, QVariant(state));
}

//-----------------------------------------------------------------------------

inline QString
PushButtonUiCell::offQuestion() const
{
    return offQuestion_;
}

//-----------------------------------------------------------------------------

inline bool
PushButtonUiCell::offQuestionOn() const
{
    return offQuestionOn_;
}

//-----------------------------------------------------------------------------

inline QString
PushButtonUiCell::onQuestion() const
{
    return onQuestion_;
}

//-----------------------------------------------------------------------------

inline bool
PushButtonUiCell::onQuestionOn() const
{
    return onQuestionOn_;
}

//-----------------------------------------------------------------------------

inline void
PushButtonUiCell::setOffQuestionOn(bool offQuestionOn)
{
    offQuestionOn_ = offQuestionOn;
}

//-----------------------------------------------------------------------------

inline void
PushButtonUiCell::setOnQuestionOn(bool onQuestionOn)
{
    onQuestionOn_ = onQuestionOn;
}

//-----------------------------------------------------------------------------

inline void
PushButtonUiCell::setOffQuestion(const QString& offQuestion)
{
    offQuestion_ = offQuestion;
}

//-----------------------------------------------------------------------------

inline void
PushButtonUiCell::setOnQuestion(const QString& onQuestion)
{
    onQuestion_ = onQuestion;
}

}
#endif // PUSH_BUTTON_UI_ITEM_H
