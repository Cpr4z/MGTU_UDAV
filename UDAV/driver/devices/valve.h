#ifndef VALVE_H
#define VALVE_H

#include "device.h"

class QTimer;

/**
 * @brief Класс конечного автомата состояний для клапана/затвора с дачтиком положений
 */
class Valve : public Device
{
    Q_OBJECT
    private:
        QTimer *closingTimer_;
        QTimer *openingTimer_;

        /**
         * @brief Состояние клапан/затвор закрыт
         */
        QState *closed_;

        /**
         * @brief Состояние клапан/затвор закрывается
         */
        QState *closing_;

        /**
         * @brief Состояние клапан/завтор открыт
         */
        QState *opened_;

        /**
         * @brief Состояние клапан/затвор закрывается
         */
        QState *opening_;

        /**
         * @brief Адрес фукнции, выполняющей закрытие клапана/затвора
         */
        bool (*close_func_)();

        /**
         * @brief Адрес фукнции, выполняющей открытие клапана/затвора
         */
        bool (*open_func_)();

        bool (*is_closed_func_)(bool& state);

        bool (*is_opened_func_)(bool& state);

    public:
        /**
         * @brief Конструктор. Инициализирует состояния и взаимосвязь между ними
         * @param parent
         */
        explicit Valve(QObject* parent = 0);

        /**
         * @brief Деструктор
         */
        ~Valve();

        /**
         * @brief Установить функцию, выполняющую закрытие клапана/затвора
         * @param sender - инициатор закрытия
         * @param signal - сигнал закрытия
         * @param close_func - функция, выполняющая закрытие
         */
        void installCloseFunc(QObject *sender, const char *signal, bool (*close_func)());

        void installIsClosedFunc(bool (*is_closed_func)(bool&));

        /**
         * @brief Установить функцию, выполняющую открытие клапана/затвора
         * @param sender - инициатор открытия
         * @param signal - сигнал открытия
         * @param close_func - функция, выполняющая открытие
         */
        void installOpenFunc(QObject *sender, const char *signal, bool (*open_func)());

        void installIsOpenedFunc(bool (*is_opend_func)(bool&));

//        bool isClosed() const;

//        bool isOpened() const;

    public slots:

        void close();

        void open();

    private slots:
        /**
         * @brief Закрыть клапан/затвор
         *
         * Если установлена фукнция закрытия вызывает ее. В противном случае ничего не делаетю
         */
        void close_();

        /**
         * @brief Открыть клапан/затвор
         *
         * Если установлена фукнция открытия вызывает ее. В противном случае ничего не делаетю
         */
        void open_();

        void checkClosing_();

        void checkOpening_();

    signals:
        /**
         * @brief Сигнал информирует о том, что клапан/затвор закрылся
         */
        void closed();

        /**
         * @brief Сигнал информирует о том, что клапан/затвор открылся
         */
        void opened();
};

#endif // VALVE_H
