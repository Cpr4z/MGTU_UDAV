/*****************************************************************************
 * modbus4qt Library
 * Author: Leonid Kolesnik, l.kolesnik@m-i.ru
 * Copyright (C) 2012-2015
 * http://mt11.net.ru
 *****************************************************************************/

/*****************************************************************************
* This file is part of modbus4qt.
*
* modbus4qt is free software: you can redistribute it and/or modify it under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option) any
* later version.
*
* modbus4qt is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
* A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with
* modbus4qt.
*
* If not, see <https://www.gnu.org/licenses/>.
*****************************************************************************/


#ifndef RTU_CLIENT_H
#define RTU_CLIENT_H

#include "client.h"
#include "rtu_device.h"

#include <QSerialPort>
#include <QTimer>

namespace modbus4qt
{

/**
 * @brief Класс MODBUS/RTU клиента.
 */
class MODBUS4QT_EXPORT RtuClient : public Client, public RTUDevice
{
    Q_OBJECT

    private:
        /**
         * @brief
         * @en Name of port using for data exchange
         * @ru Имя порта для обмена данными
         */
        QString portName_;

        /**
         * @brief
         * @en Port for data exchange
         * @ru Порт, используемый для обмена данными
         */
        QSerialPort* serialPort_;

        /**
         * @brief
         * @en Boud rate
         * @ru Используемая скорость обмена данными
         *
         * @en Default value: 9600
         * @ru Значение по умолчанию: 9600.
         */
        QSerialPort::BaudRate baudRate_;

        /**
         * @brief
         * @en Quantity of data bits
         * @ru Используемое количество бит данных
         *
         * @en Default value: 8
         * @ru Значение по умолчанию: 8
         */
        QSerialPort::DataBits dataBits_;

        /**
         * @brief
         * @en Quantity of stop bits
         * @ru Используемое количество стоповых бит
         *
         * @en Default value: 1
         * @ru Значение по умолчанию: 1
         */
        QSerialPort::StopBits stopBits_;

        /**
         * @brief
         * @en Parity mode
         * @ru Используемый бит четности
         *
         * @en Default value: Even
         * @ru Значение по умолчанию: Even
         */
        QSerialPort::Parity parity_;

        /**
         * @brief
         * @en Timer for controlling silence time in serial line
         * @ru Таймер для обеспечения заданного периода тишины в линии после получения ответа от сервера
         *
         * @en For baud rate more than 19200 bps silence time should be not less 1750 mcs.
         * For less baud rates silence time should be not less than 3.5 time for transmitting one character.
         *
         * @ru Для скоростей более, чем 19200 бод период тишины должен быть не менее 1750 мкс.
         * Для меньших скоростей период тишины должен быть не менее, чем 3,5 времени на передачу одного символа.
         */
        QTimer silenceTimer_;

        /**
         * @brief
         * @en Flag for silence mode
         * @ru Флаг, показывающий, что клиент находится в состоянии тишины
         */
        bool inSilenceState_;

        /**
         * @brief
         * @en Time of silence interval, ms
         * @ru Время на интервал тишины, в мс
         */
        int silenceTime_;

        /**
         * @brief
         * @en If port is open set data exchange settings with current values
         * @ru Если порт открыт, то устанавливает параметры обмена данными в соответствие с текущими настройками
         *
         * @en If port is not open errorMessage() signal will be emited.
         * @ru Если порт не открыт, инициирует сигнал errorMessage();
         *
         * @sa baudRate_, dataBits_, stopBits_, parity_, errorMessage();
         */
        bool configurePort_();

        /**
         * @brief
         * @en Prepare application data unit for MODBUS/RTU specification
         * @ru Формирует блок данных реализации (Application Data Unit) по спецификации MODBUS/RTU.
         *
         * @param
         * @en pdu - protocol data unit
         * @ru pdu - блок данных протокола (Protocol Data Unit)
         *
         * @param
         * @en pduSize - size of protocol data unit
         * @ru pduSize - размер блока данных протокола
         *
         * @return
         * @en Prepared application data unit
         * @ru Сформированный блок данных реализации (Application Data Unit) по спецификации MODBUS/RTU
         */
        virtual QByteArray prepareADU_(const ProtocolDataUnit &pdu, int pduSize) override;

        /**
         * @brief
         * @en Process application data unit revieved from server and return protocol data unit
         * @ru Обрабатывает полученный блок данных приложения (Application Data Unit) и возвращает блок данных протокола (protocol data unit)
         *
         * @param
         * @en buf - array with application data unit
         * @ru buf - массив, содержащий блок данных реализации
         *
         * @return
         * @en Protocol data unit
         * @ru Блок даных протокола
         */
        virtual ProtocolDataUnit processADU_(const QByteArray &buf) override;

        /**
         * @brief
         * @en Read response from slave device
         * @ru Читает ответ от сервера MODBUS
         *
         * @return
         * @en Readed data array
         * @ru Массив прочитанных данных
         *
         */
        virtual QByteArray readResponse_() override;

        virtual bool sendRequest_(const ProtocolDataUnit& requestPDU,  int requestPDUSize, ProtocolDataUnit* responsePDU) override;

        /**
         * @brief
         * @en Set silence time depending of current baud rate
         * @ru Рассчитывает и устанавливает время периода тишины в линии в зависимости от текущей скорости передачи данных
         */
        void setSilenceTime_();

    private slots:

        /**
         * @brief
         * @en Start silence time
         * @ru Начинает период тишины
         */
        void startSilence_();

        /**
         * @brief
         * @en Finish silence time
         * @ru Завершает период тишины
         */
        void stopSilence_();

    public:

        /**
         * @brief
         * @en Constructor. Full version
         * @ru Конструктор. Версия со всеми параметрами
         *
         * @param
         * @en portName - port name for data exchange
         * @ru portName - имя порта для обмена данными
         *
         * @param
         * @en baudRate - baud rate
         * @ru baudRate - скорость обмена данными
         *
         * @param
         * @en dataBits - quantity of bits
         * @ru dataBits - количество бит данных
         *
         * @param
         * @en stopBits - quantity of stop bits
         * @ru stopBits - количество стоповых бит
         *
         * @param
         * @en parity - type of pariry control
         * @ru parity - используемый метод контроля четности
         *
         * @param
         * @en parent - Qt parent object
         * @ru parent - объект-родитель
         *
         * @en Create connection to port and set up paraeters for data exchange.
         *
         * @ru Создает подключение к порту и устанавливает параметры порта обмена данными.
         */
        explicit RtuClient(const QString& portName,
                           QSerialPort::BaudRate baudRate = QSerialPort::Baud9600,
                           QSerialPort::DataBits dataBits = QSerialPort::Data8,
                           QSerialPort::StopBits stopBits = QSerialPort::OneStop,
                           QSerialPort::Parity parity = QSerialPort::EvenParity,
                           QObject *parent = 0);

        /**
         * @brief
         * @en Constructor. Short version
         * @ru Конструктор. Сокращенная версия
         *
         * @param
         * @en portName - port name for data exchange
         * @ru portName - имя порта для обмена данными
         *
         * @param
         * @en baudRate - baud rate
         * @ru baudRate - скорость обмена данными
         *
         * @param
         * @en dataBits - quantity of bits
         * @ru dataBits - количество бит данных
         *
         * @param
         * @en stopBits - quantity of stop bits
         * @ru stopBits - количество стоповых бит
         *
         * @param
         * @en parent - Qt parent object
         * @ru parent - объект-родитель
         *
         * @en Create connection to port and set up paraeters for data exchange.
         * parity_ will be setted to default value QSerialPort::EvenParity
         *
         * @ru Создает подключение к порту и устанавливает параметры порта обмена данными.
         * parity_ устанавливается в значение QSerialPort::EvenParity
         */
        explicit RtuClient(const QString& portName,
                           QSerialPort::BaudRate baudRate = QSerialPort::Baud9600,
                           QSerialPort::DataBits dataBits = QSerialPort::Data8,
                           QSerialPort::StopBits stopBits = QSerialPort::OneStop,
                           QObject *parent = 0);

        /**
         * @brief
         * @en Constructor. Short version
         * @ru Конструктор. Сокращенная версия
         *
         * @param
         * @en portName - port name for data exchange
         * @ru portName - имя порта для обмена данными
         *
         * @param
         * @en baudRate - baud rate
         * @ru baudRate - скорость обмена данными
         *
         * @param
         * @en dataBits - quantity of bits
         * @ru dataBits - количество бит данных
         *
         * @param
         * @en parent - Qt parent object
         * @ru parent - объект-родитель
         *
         * @en Create connection to port and set up paraeters for data exchange.
         * stopBits_ will be setted to default value QSerialPort::OneStop
         * parity_ will be setted to default value QSerialPort::EvenParity
         *
         * @ru Создает подключение к порту и устанавливает параметры порта обмена данными.
         * stopBits_ устанавливается в значение QSerialPort::OneStop
         * parity_ устанавливается в значение QSerialPort::EvenParity
         */
        explicit RtuClient(const QString& portName,
                           QSerialPort::BaudRate baudRate = QSerialPort::Baud9600,
                           QSerialPort::DataBits dataBits = QSerialPort::Data8,
                           QObject *parent = 0);

        /**
         * @brief
         * @en Constructor. Short version
         * @ru Конструктор. Сокращенная версия
         *
         * @param
         * @en portName - port name for data exchange
         * @ru portName - имя порта для обмена данными
         *
         * @param
         * @en baudRate - baud rate
         * @ru baudRate - скорость обмена данными
         *
         * @param
         * @en parent - Qt parent object
         * @ru parent - объект-родитель
         *
         * @en Create connection to port and set up paraeters for data exchange.
         * dataBits_ will be setted to default value QSerialPort::Data8
         * stopBits_ will be setted to default value QSerialPort::OneStop
         * parity_ will be setted to default value QSerialPort::EvenParity
         *
         * @ru Создает подключение к порту и устанавливает параметры порта обмена данными.
         * dataBits_ устанавливается в значение QSerialPort::Data8
         * stopBits_ устанавливается в значение QSerialPort::OneStop
         * parity_ устанавливается в значение QSerialPort::EvenParity
         */
        explicit RtuClient(const QString& portName,
                           QSerialPort::BaudRate baudRate = QSerialPort::Baud9600,
                           QObject *parent = 0);

        /**
         * @brief
         * @en Constructor. Short version
         * @ru Конструктор. Сокращенная версия
         *
         * @param
         * @en portName - port name for data exchange
         * @ru portName - имя порта для обмена данными
         *
         * @param
         * @en baudRate - baud rate
         * @ru baudRate - скорость обмена данными
         *
         * @param
         * @en parent - Qt parent object
         * @ru parent - объект-родитель
         *
         * @en Create connection to port and set up paraeters for data exchange.
         * baudRate_ will be setted to default value QSerialPort::QSerialPort::Baud9600
         * dataBits_ will be setted to default value QSerialPort::Data8
         * stopBits_ will be setted to default value QSerialPort::OneStop
         * parity_ will be setted to default value QSerialPort::EvenParity
         *
         * @ru Создает подключение к порту и устанавливает параметры порта обмена данными.
         * baudRate_ устанавливается в значение QSerialPort::QSerialPort::Baud9600
         * dataBits_ устанавливается в значение QSerialPort::Data8
         * stopBits_ устанавливается в значение QSerialPort::OneStop
         * parity_ устанавливается в значение QSerialPort::EvenParity
         */
        explicit RtuClient(const QString& portName, QObject *parent = 0);

        virtual ~RtuClient();

        /**
         * @brief
         * @ru Open port and set up it with current data exchange settings
         * @ru Открывает порт и настраивает его в соответствие с текущими параметрами обмена данными
         *
         * @return
         * @en true, if port opened succesfullty; false in case of error
         * @ru true, если порт открыт успешно; false в противном случае
         *
         * @en In case of error errorMessage() signal will be emitted for addition.
         *
         * @ru В случае возникновения ошибки дополнительно инициирует сигнал errorMessage();
         */
        bool openPort();

        /**
         * @brief
         * @en Set up baud rate
         * @ru Устанавливает скорость обмена данными
         *
         * @param
         * @ru baudRate - new baud rate
         * @ru baudRate - скорость обмена данными
         *
         * @en If port is open then baud rate will be changed on fly.
         * @ru Если порт открыт, то скорость обмена изменяется "на лету".
         */
        void setBaudRate(QSerialPort::BaudRate baudRate);

        /**
         * @brief
         * @en Set up quantity of data bits
         * @ru Устанавливает количество бит данных
         *
         * @param
         * @en dataBits - quantity of data bits
         * @ru dataBits - количество бит данных
         *
         * @en If port is open then data bits quantity will be changed on fly.
         * @ru Если порт открыт, то количество бит данных изменяется "на лету".
         */
        void setDataBits(QSerialPort::DataBits dataBits);

        /**
         * @brief
         * @en Set up type of parity control
         * @ru Устанавливает метод контроля четности
         *
         * @param
         * @en parity - type of parity control
         * @ru parity - метод контроля четности
         *
         * @en If port is open then parity control will be changed on fly.
         * @ru Если порт открыт, то метод контроля четности изменяется "на лету".
         */
        void setParity(QSerialPort::Parity parity);

        /**
         * @brief
         * @en Set up quantity of stop bits
         * @ru Устанавливает количество стоповых бит
         *
         * @param
         * @ru stopBits - количество стоповых бит
         *
         * Если порт открыт, то количество стоповых бит изменяется "на лету".
         */
        void setStopBits(QSerialPort::StopBits stopBits);

        /**
         * @brief
         * @en Set up port name for data exchange
         * @ru Задает имя порта для обмена данными
         *
         * @param
         * @en portName - new port name
         * @ru portName - новое имя порта для обмена данными
         *
         * @en If port is open it will be closed. You should open it by hand.
         *
         * @ru Если порт был открыт, то он будет закрыт перед сменой имени порта.
         * Автоматическое открытие порта после смены имени не производится.
         */
        void setPortName(const QString& portName);

        /**
         * @brief
         * @en Return current port name
         * @ru Возвращает текущее имя порта для обмена данными
         *
         * @return
         * @en Current port name
         * @ru Текущее имя порта для обмена данными
         */
        QString portName() const;

    signals:

        /**
         * @brief dataReaded
         */
        void dataReaded();
};

} // namespace modbus4qt


#endif // RTU_CLIENT_H
