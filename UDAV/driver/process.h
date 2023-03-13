#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QDebug>
#include "drivers.h"

namespace udav_drivers
{


class DriverImpl;

/**
 * @brief Абстрактный класс описания процесса автоматизируемого оборудования.
 */

class Process : public QObject
{
    Q_OBJECT

    public:

        /**
         * @brief Конструктор по умолчанию
         * @param driverImpl - указатель на реализацию драйвера оборудования
         * @param signalID - номер сигнала, отвечающего за запуск и остановку процесса
         * @param name - имя процесса
         */
        Process(DriverImpl* driverImpl, SignalID signalID, const QString& name = "");

        /**
         * @brief Возвращает номер сигнала, отвечающего за запуск и остановку процесса
         * @return номер сигнала, отвечающего за запуск и остановку процесса
         */
        inline SignalID getSignalID() const;

        /**
         * @brief Возвращает имя процесса
         * @return имя процесса
         */
        inline QString getProcessName() const;

        /**
         * @brief Возвращает состояние процесса: запущен или нет
         * @return true - если процесс запущен, false в противном случае
         */
        inline bool isStarted() const;

    public slots:
        /**
         * @brief Запускает процесс
         * @return Возвращает Drivers::CommandOk, если процесс запустился; Drivers::CommandError, если при запуске процесса произошла ошибка
         */
        inline virtual int start();

        /**
         * @brief Останавливает процесс
         */
        inline virtual void stop();

        /**
         * @brief Проверяет состояние оборудования при запущенном процессе
         *
         * Функция используется для контроля оборудования и аварийного завершешния процесса при возникновении
         * внештатных ситуаций. Вызов функции происходит по таймеру. Функционал должен быть реализован в классе
         * реального процесса.
         */
        virtual void checkState() = 0;

    protected:
        /**
         * @brief Указатель на реализацию драйвера оборудования
         */
        DriverImpl* driverImpl_;

        /**
         * @brief Имя процесса
         */
        QString name_;

        /**
         * @brief Номер сигнала, отвечающего за запуск и остановку процесса
         */
        SignalID signalID_;

        /**
         * @brief Флаг показывающий, что процесс остановлен
         */
        volatile bool stopped_;

        /**
         * @brief Находимся в ожидании заверешения выполнения команды
         */
        bool waiting_;

        /**
         * @brief Команда успешно выполнена
         */
        bool success_;

        /**
         * @brief Не удалось выполнить текущую команду
         */
        bool failed_;


    signals:

        /**
         * @brief Сигнал формируется после успешного запуска процесса
         * @param signalID - номер сигнала процесса
         * @param name - имя процесса
         */
        void processStarted(SignalID signalID, const QString& name);

        /**
         * @brief Сигнал формируется после остановки процесса
         * @param signalID - номер сигнала процесса
         * @param name - имя процесса
         */
        void processStopped(SignalID signalID, const QString& name);

        /**
         * @brief Сигнал формируется при возникновении ошибки
         * @param message - текст сообщения об ошибке
         */
        void errorMessage(const QString& message);

        /**
         * @brief Сигнал на останов внешнего процесса
         * @param processID
         *
         * Процесс может остановить другой процесс, но запустить процесс может только сам драйвер...
         */
        void stopProcess(SignalID processID);

        /**
         * @brief Сигнал на внеочередное чтение значений сигналов наблюдения
         */
        void readViewValues();

    protected slots: // Подсистема вывода сообщений

        /**
         * @brief Обработать критическое сообщение, добавив в него имя драйвера, и передать его дальше в систему
         * @param message
         */
        inline void processCriticalMessage(const QString message);

        /**
         * @brief Обработать информационное сообщение, добавив в него имя драйвера, и передать его дальше в систему
         * @param message
         */
        inline void processInfoMessage(const QString message);

        /**
         * @brief Обработать отладочное сообщение, добавив в него имя драйвера, и передать его дальше в систему
         * @param message
         */
        inline void processDebugMessage(const QString message);

    signals: // Подсистема вывода сообщений

        /**
         * @brief Отправить сообщение о критической ошибке в глобальную подсистему обработки сообщений
         * @param message - текст сообщения
         */
        void criticalMessage(const QString& message) const;

        /**
         * @brief Отправить отладочное сообщение в глобальную подсистему обработки сообщений
         * @param message - текст сообщения
         */
        void debugMessage(const QString& message) const;

        /**
         * @brief Отправить информационное сообщение в глобальную подсистему обработки сообщений
         * @param message - текст сообщения
         */
        void infoMessage(const QString& message) const;

    signals: // protected
        /**
         * @brief Отправить сообщение о критической ошибке в подсистему обработки сообщений экземпляра класса
         * @param message - текст сообщения
         *
         * В сообщение будет добавлено имя драйвера, после чего оно будет отправлено в глобальную подсистему обработки сообещений
         */
        void criticalMessage_(const QString& message) const;

        /**
         * @brief Отправить отладочное сообщение в подсистему обработки сообщений экземпляра класса
         * @param message - текст сообщения
         *
         * В сообщение будет добавлено имя драйвера, после чего оно будет отправлено в глобальную подсистему обработки сообещений
         */
        void debugMessage_(const QString& message) const;

        /**
         * @brief Отправить информационное сообщение в подсистему обработки сообщений экземпляра класса
         * @param message - текст сообщения
         *
         * В сообщение будет добавлено имя драйвера, после чего оно будет отправлено в глобальную подсистему обработки сообещений
         */
        void infoMessage_(const QString& message) const;



};

//-----------------------------------------------------------------------------

void
Process::processCriticalMessage(const QString message)
{
    QString fullMessage = "[" + name_ + "] " + message;
    emit criticalMessage(fullMessage);
}

//-----------------------------------------------------------------------------

void
Process::processInfoMessage(const QString message)
{
    QString fullMessage = "[" + name_ + "] " + message;
    emit infoMessage(fullMessage);
}

//-----------------------------------------------------------------------------

void
Process::processDebugMessage(const QString message)
{
    QString fullMessage = "[" + name_ + "] " + message;
    emit debugMessage(fullMessage);
}

//-----------------------------------------------------------------------------

SignalID
Process::getSignalID() const
{
    return signalID_;
}

//-----------------------------------------------------------------------------

QString
Process::getProcessName() const
{
    return name_;
}

//-----------------------------------------------------------------------------

bool
Process::isStarted() const
{
    return !stopped_;
}

//-----------------------------------------------------------------------------

int
Process::start()
{
    stopped_ = false;

    waiting_ = true;
    success_ = false;
    failed_ = false;

    emit processStarted(signalID_, name_);

    return CommandOk;
}

//-----------------------------------------------------------------------------

void
Process::stop()
{
    stopped_ = true;
    emit processStopped(signalID_, name_);
}

//-----------------------------------------------------------------------------

}

#endif // #ifndef PROCESS_H
