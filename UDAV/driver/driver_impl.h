#ifndef DRIVER_IMPL_H
#define DRIVER_IMPL_H

#include <QObject>
#include <QTimer>

#include "drivers_global.h"

#include "drivers.h"
#include "driver_interface.h"
#include "driver_settings.h"
#include "meta_signal.h"
#include "process.h"

#include "elvactools/logger.h"

#include <QSerialPort>

class AbstractDevice;
class SerialPort;
class SerialDevice;

namespace modbus4qt
{
    class RtuClient;
}

namespace udav_drivers
{

/**
 * @brief The DriverImpl class - base class for any driver implementation.
 */
class UDAV_DRIVERS_EXPORT DriverImpl : public QObject
{
    Q_OBJECT
    ELVACTOOLS_LOGGER_SYSTEM_DECLARE

    friend class DriverInterface;

    public :

        /**
         * @brief Return copy of control signal values.
         * @return Copy of control signal values at calling time.
         *
         * @todo decide if we need it!
         */
        SignalValues controlValues() const;

        /**
         * @brief Return the user-friendly description of the driver.
         * @return Description of the driver.
         */
        QString description() const;

        /**
         * @brief Return message about last error.
         * @return Message about an error.
         *
         * Message will be cleared after returning.
         */
        QString errorString() const;

        /**
         * @brief Return reference to metasignals describing signals provided
         * by equipment.
         *
         * @return Reference to metasignals.
         */
        const MetaSignals& metaSignals() const;

        /**
         * @brief Return driver name.
         * @return Driver name.
         */
        QString name() const;

        /**
         * @brief Return the user interface description provided by the hardware.
         * @return The user interface description provided by the hardware.
         *
         * The user interface description is passed as a string containing an XML.
         */
        virtual QString userInterface() const = 0;

        /**
         * @brief Return copy of view signal values.
         * @return Copy of view signal values at calling time.
         *
         * @todo decide if we need it!
         */
        virtual SignalValues viewValues() const;

    signals :

        /**
         * @brief Report setting all signals to default.
         *
         */
        void signalValuesResetted();

        /**
         * @brief Report a change in signal value.
         * @param signalID - signal id.
         * @param message - additional text message.
         */
        void signalValueChanged(udav_drivers::SignalID signalID, const QString& message = QString(""));

        /**
         * @brief Notify when reading surveillance signals is complete.
         */
        void viewSignalsReaded();


    public slots: //? Public ???

        /**
         * @brief Process the boolean control signal received from the client.
         * @return CommandOk in success; CommandError in case of an error.
         */
        virtual int setControlSignalBoolean(udav_drivers::SignalID signalID, bool value) = 0;

        /**
         * @brief Process the double control signal received from the client.
         * @return CommandOk in success; CommandError in case of an error.
         */
        virtual int setControlSignalDouble(udav_drivers::SignalID signalID, double value) = 0;

        /**
         * @brief Process the int control signal received from the client.
         * @return CommandOk in success; CommandError in case of an error.
         */
        virtual int setControlSignalInteger(udav_drivers::SignalID signalID, int value) = 0;

    signals: // protected - Logging subsystem - do not use them outside of class

        /**
         * @brief Send a fatal error message to the driver's message processing
         * subsystem
         *
         * @param message - message to log.
         *
         * Message will be processed by processCriticalMessage_()
         *
         * @sa processCriticalMessage_()
         */
        void criticalMessage_(const QString& message) const;

        /**
         * @brief Send a debug message to the driver's message processing
         * subsystem
         *
         * @param message - message to log.
         *
         * Message will be processed by processDebugMessage_()
         *
         * @sa processDebugMessage_();
         */
        void debugMessage_(const QString& message) const;

        /**
         * @brief Send an info message to the driver's message processing
         * subsystem
         *
         * @param message - message to log.
         *
         * Message will be processed by processInfoMessage_()
         *
         * @sa processInfoMessage_();
         */
        void infoMessage_(const QString& message) const;

    protected: // methods

        /**
         * @brief Default constructor.
         * @param metaSignals - desciption of metasignals.
         * @param settings - driver settings.
         * @param parent - object of the driver interface.
         * @param simulate - use or not simulation mode.
         */
        explicit DriverImpl
        (
            const MetaSignals& metaSignals,
            const DriverSettings& settings,
            DriverInterface* driverInterface,
            bool simulate,
            elvactools::Logger* logger
        );

        /**
         * @brief ~DriverImpl - destructor.
         */
        virtual ~DriverImpl();

        /**
         * @brief Create a connection to the MODBUS/RTU bus.
         * @param name - bus user-friendly name.
         * @param port - port name.
         * @param baudRate - baud rate.
         * @param dataBits - data bits.
         * @param stopBits - stop bits.
         * @param parity - parity checking mode.
         * @param timeout - max timeout.
         * @return Pointer to the created MODBUS connection object.
         */
        modbus4qt::RtuClient* createModBusConnection_(const QString& name, const QString& port, QSerialPort::BaudRate  baudRate, QSerialPort::DataBits dataBits, QSerialPort::StopBits stopBits, QSerialPort::Parity parity, int timeout);

        /**
         * @param name - bus user-friendly name.
         * @param port - port name.
         * @param baudRate - baud rate.
         * @param dataBits - data bits.
         * @param stopBits - stop bits.
         * @param parity - parity checking mode.
         * @param timeout - max timeout.
         * @return Pointer to the created serial line connection object.
         */
        SerialPort* createSerialPortConnection_(const QString& name, const QString& port, QSerialPort::BaudRate  baudRate, QSerialPort::DataBits dataBits, QSerialPort::StopBits stopBits, QSerialPort::Parity parity, QSerialPort::FlowControl flowControl);

        /**
         * @brief Connect to the device.
         * @param device - device to be connected.
         * @param msgWinId
         * @return true in success, false overwise.
         */
        bool connectDevice_(AbstractDevice* device, QUuid msgWinId);

        /**
         * @brief Init middle level devices.
         */
        virtual void initDevices_();

        /**
         * @brief Init low level equipment.
         * @return true in success; false overwise.
         */
        virtual bool initEquipment_() = 0;

        /**
         * @brief Set logger for messaging.
         * @param logger - pointer to logger object.
         */
        void setLogger(elvactools::Logger* logger);

        /**
         * @brief Запустить процесс обмена данными с оборудованием или процесс симуляции работы оборудования
         *
         * Обмен данными производится через заданные интервалы времени. Читаются значения сигналов датчиков и записываются
         * в локальные переменные класса.
         *
         * @todo Выверить информацию. Она не верна!
         * В классе наследнике, если в качестве оборудования выступает симулятор, должен быть создан класс
         * симулятора и в отдельном потоке запущен процесс симуляции работы оборудования.
         */
        void start_();

        /**
         * @brief Остановить процесс обмена данными с оборудованием или процесс симуляции работы оборудования
         */
        void stop_();

        /**
         * @brief Инициализировать настройки драйвера оборудования
         * @param messageWindow - идентификатор окна, в который клиенту будут отправлены сообщения о возникших ошибках
         * @return true - если инициализация прошла успешно, false в противном случае
         */
        virtual bool initSettings_(QUuid messageWindow) = 0;

        /**
         * @brief На основании метаданных сигналов заполнить массивы controlSygnalNames и viewSygnalNames символическим именами сигналов
         */
        void initSignalNames_();

        /**
         * @brief На основании метаданных сигналов заполнить значения сигналов по умолчанию
         */
        void fillDefaultValues_();

    protected slots:

        /**
         * @brief Read Scontol signal values from equipment.
         *
         * The function is called on the event of the readTimer.
         * The function must be implemented in the derived class.
         */
        virtual void readViewValues_() = 0;

        /**
         * @brief Reset hardware settings to default and report about it.
         *
         */
        void reset_();

        /**
         * @brief Process a critical message, appending the driver name to it,
         * and pass it on to the logger.
         *
         * @param message - message to log.
         *
         * @sa criticalMessage_();
         */
        void processCriticalMessage_(const QString message);

        /**
         * @brief Process a debug message, appending the driver name to it,
         * and pass it on to the logger.
         *
         * @param message - message to log.
         *
         * @sa debugMessage_();
         */
        void processInfoMessage_(const QString message);

        /**
         * @brief Process an info message, appending the driver name to it,
         * and pass it on to the logger.
         *
         * @param message - message to log.
         *
         * @sa infoMessage_();
         */
        void processDebugMessage_(const QString message);

    protected: // fields

        /**
         * @brief Массив мнемонических имен сигналов управления.
         */
        QMap<quint8, QString> controlSignalNames_;

        /**
         * @brief Массив данных управляющих сигналов, передаваемых оборудованию.
         */
        SignalValues controlValues_;

        /**
         * @brief Флаг, показывающий, что устройства среднего уровня инициализированы
         */
        bool devicesInited_;

        /**
         * @brief Указатель на интерфейс драйвера, владеющий экземпляром класса реализации драйвера.
         */
        DriverInterface* driverInterface_;

        /**
         * @brief Настройки драйвера оборудования.
         *
         * @todo Работу с настройками надо унифицировать для всех классов проекта.
         */
        const DriverSettings& driverSettings_;

        /**
         * @brief Массив метаданных сигналов. Локальная копия (константная ссылка) данных экземпляра драйвера.
         */
        const MetaSignals& metaSignals_;

        /**
         * @brief Таймер, контролирующий процесс чтения значений сигналов от оборудования.
         */
        QTimer readTimer_;

        /**
         * @brief Интервал чтения данных от оборудования. Значение по умолчанию - 1000 мсек.
         */
        quint16 readTimerInterval_;

        /**
         * @brief Флаг, показывающий работу в режиме симулятора
         */
        bool simulate_;

        /**
         * @brief time_
         */
        QDateTime time_;

        //! Мнемонические имена сигнгалов наблюдения.
        QMap<quint8, QString> viewSignalNames_;

        //! Данные сигналов наблюдения, снимаемых с оборудования.
        mutable SignalValues viewValues_;

        /**
         * @brief Строка с последним сообщением об ошибке
         */
        mutable QString errorString_;

        elvactools::Logger* logger_;
};

}

#endif // DRIVER_IMPL_H
