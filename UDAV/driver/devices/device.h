#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QState>
#include <QStateMachine>

class Device : public QObject
{
    Q_OBJECT
    protected:
        QStateMachine *machine_;

        /**
         * @brief Состояние "сбой устройства"
         */
        QState *failed_;

        /**
         * @brief Состояние "инициализация устройства"
         */
        QState *initing_;

        /**
         * @brief Флаг, показывающий упешность инициализации
         */
        bool inited_;


    public:
        explicit Device(QObject *parent = 0);
        ~Device();

    signals:
        void errorMessage();

    public slots:
        void run();

signals:

    /**
     * @brief Отправляет сообщение о критической ошибке в глобальную систему обработки сообщений
     * @param message - текст сообщения
     */
    void criticalMessage(const QString& message) const;

    /**
     * @brief Отправляет отладочное сообщение в глобальную систему обработки сообщений
     * @param message - текст сообщения
     */
    void debugMessage(const QString& message) const;

    /**
     * @brief Отправляет информационное сообщение в глобальную систему обработки сообщений
     * @param message - текст сообщения
     */
    void infoMessage(const QString& message) const;

/*protected*/ signals: // Подсистема вывода сообщений

    /**
     * @brief Отправляет сообщение о критической ошибке в локальную систему обработки сообщений
     * @param message - текст сообщения
     */
    void criticalMessage_(const QString& message) const;

    /**
     * @brief Отправляет отладочное сообщение в локальную систему обработки сообщений
     * @param message - текст сообщения
     */
    void debugMessage_(const QString& message) const;

    /**
     * @brief Отправляет информационное сообщение в локальную систему обработки сообщений
     * @param message - текст сообщения
     */
    void infoMessage_(const QString& message) const;


protected slots:

    virtual void failedEntered_();

    /**
     * @brief Обрабатать критическое сообщение, добавив в него имя устройства, и передать его дальше в глобальную систему обработки сообщений
     * @param message - текст сообщения
     */
    inline void processCriticalMessage_(const QString& message) const;

    /**
     * @brief Обрабатать информационное сообщение, добавив в него имя устройства, и передать его дальше в глобальную систему обработки сообщений
     * @param message - текст сообщения
     */
    inline void processInfoMessage_(const QString& message) const;

    /**
     * @brief Обрабатать отладочное сообщение, добавив в него имя устройства, и передать его дальше в глобальную систему обработки сообщений
     * @param message - текст сообщения
     */
    inline void processDebugMessage_(const QString& message) const;


};

void Device::processCriticalMessage_(const QString &message) const
{
    QString fullMessage = "[" + objectName() + "] " + message;
    emit criticalMessage(fullMessage);
}

void Device::processInfoMessage_(const QString &message) const
{
    QString fullMessage = "[" + objectName() + "] " + message;
    emit infoMessage(fullMessage);
}

void Device::processDebugMessage_(const QString &message) const
{
    QString fullMessage = "[" + objectName() + "] " + message;
    emit debugMessage(fullMessage);
}

#endif // DEVICE_H
