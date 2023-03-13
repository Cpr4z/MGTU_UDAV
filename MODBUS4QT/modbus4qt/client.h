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

#ifndef MODBUS4QT_CLIENT_H
#define MODBUS4QT_CLIENT_H

#include <QObject>

#include "abstract_device.h"

class QIODevice;

namespace modbus4qt
{

/**
* @brief
* @en Abtract modbus client
* @ru Абстрактный клиент протокола modbus
*
* @en
* After call method of data exchange with server forms protocol data unit, and after
* that call method to form application data unit. Formed block will be sent through
* input-output device.
*
* When data sended client waits for a answer and process it after reciveing.
* First process of data recieved will be done in descendant class.
*
* Descendant class must realize abstract methods prepareADU_() and processADU_().
* Also it is important to initialize ioDevice_ field with address of real object resposible
* for data exchange.
*
* @ru
* При вызове метода обмена данными с сервером формирует блок данных протокола
* (Protocol Data Unit) после чего вызывает метод формирования блока данных
* приложения (Apllication Data Unit) класса-потомка. Сформированный блок данных
* передается в устройство ввода-вывода.
*
* После отправки данных серверу ожидает ответа и разбирает полученный ответ.
* Первичная обработка полученного блока данных и выделение полученного блока
* данных протокола происходит в классе-потомке.
*
* Класс-потомок должен релизовать абстрактные методы prepareADU_(),
* processADU_(), а также правильно инициализировать указатель ioDevice_,
* присвоив его значение указателю на экземпляр реального объекта, отвечающего
* за обмен данными.
*
*/
class MODBUS4QT_EXPORT Client : public AbstractDevice
{
    Q_OBJECT

    protected:

        /**
         * @brief
         * @en Message about last error occured. Cleared by errorMessage()
         * @ru Текст сообщения о последней возникшей ошибке. Очищается при вызове метода errorMessage()
         *
         * @sa errorMessage();
         */
        QString errorMessage_;

        /**
         * @brief
         * @en IO device for data exchange with server
         * @ru Устройство ввода-вывода, через которое идет обмен данными с сервером
         */
        QIODevice* ioDevice_;

        /**
         * @brief
         * @en Maximum allowed timeout for server responce.
         * @ru Максимальное время ожидания чтения данных от сервера.
         *
         * @en Default value 5000 ms (5 sec).
         * @ru Значение по умолчанию 5000 мс (5 с).
         */
        int readTimeout_;

        /**
         * @brief
         * @en Maximum allowed timeout for sending data to server
         * @ru Максимальное время ожидания запси данных на сервер.
         *
         * @en Default value 5000 ms (5 sec).
         * @ru Значение по умолчанию 5000 мс (5 с).
         */
        int writeTimeout_;

        /**
         * @brief
         * @en Identifier number of server device using for data exchange
         * @ru Идентификатор устройства-сервера, с которым производится обмен данными
         */
        quint8 unitID_;

    protected :

        /**
         * @brief sendRequestToServer_
         * @param requestPDU
         * @param requestPDUSize
         * @param responsePDU
         *
         * @return
         * @en true if request is successfull; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        virtual bool sendRequest_(const ProtocolDataUnit& requestPDU,  int requestPDUSize, ProtocolDataUnit* responsePDU);

        /**
         * @brief sendRequestToServer_
         * @param requestPDU
         * @param requestPDUSize
         *
         * @return
         * @en true if request is successfull; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         * @en It is short version. You can use it if no need to analize reply.
         * @ru Это сокращенная версия. Используется для операций записи, где нет
         * необходимости в анализе ответа от сервера.
         */
        virtual bool sendRequest_(const ProtocolDataUnit& requestPDU,  int requestPDUSize);

    public:
        /**
         * @brief
         * @en Default constructor
         * @ru Конструктор по умолчанию
         *
         * @param
         * @en parent - parent object
         * @ru parent - указатель на объект-родитель.
         */
        explicit Client(QObject *parent = 0);

        /**
         * @brief
         * @en Return last error message and clear it
         * @ru Возвращает значение строки, содержащей сообщение о последней ошибке и очищает его
         *
         * @return
         * @en Last error message
         * @ru Сообщение о последней возникшей ошибке
         *
         */
        QString lastErrorMessage();

        /**
         * @brief
         * @en Read single value from coil
         * @ru Читает значение из регистра дискретного вывода
         *
         * @param
         * @en regNo - coil address
         * @ru regNo - адрес регистра
         *
         * @param
         * @en value - result will be putted here
         * @ru value - переменная для получения результата чтения
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool readCoil(quint16 regNo, bool& value);

        /**
         * @brief
         * @en Read array of values from coils
         * @ru Читает массив значений из регистров дискретного вывода
         *
         * @param regNo
         * @en regNo - address of first coil
         * @ru regNo - адрес регистра, с которого необходимо начать чтение
         *
         * @param
         * @en regQty - quantity of coils reading
         * @ru regQty - количество читаемых значений регистров
         *
         * @param
         * @en values - array for values readed
         * @ru values - массив для прочитанных значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool readCoils(quint16 regStart, quint16 regQty, QVector<bool>& values);

        /**
         * @brief
         * @en Read array of values from descrete inputs
         * @ru Читает массив значений из регистров дискретного ввода
         *
         * @param
         * @en regStart - address of first register
         * @ru regStart - адрес регистра, с которого необходимо начать чтение
         *
         * @param
         * @en regQty - quantity of inputs reading
         * @ru regQty - количество читаемых значений регистров
         *
         * @param
         * @en values - array for values readed
         * @ru values - массив для прочитанных значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool readDescreteInputs(quint16 regStart, quint16 regQty, QVector<bool>& values);

//        /**
//         * @brief readDouble
//         * @param regNo
//         * @param value
//         * @return
//         */
//        bool readDouble(quint16 regNo, double& value);

//        /**
//         * @brief readDWord
//         * @param regNo
//         * @param value
//         * @return
//         */
//        bool readDWord(quint16 regNo, quint32& value);

//        /**
//         * @brief readFloat
//         * @param regNo
//         * @param value
//         * @return
//         *
//         * @sa readSingle()
//         */
//        bool readFloat(quint16 regNo, float& value);

        /**
         * @brief
         * @en Read value from holding register
         * @ru Читает значение из регистра вывода
         *
         * @param regNo
         * @en regNo - address of register
         * @ru regNo - адрес регистра
         *
         * @param
         * @en value - readed value
         * @ru value - прочитанное значение
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool readHoldingRegister(quint16 regNo, quint16& value);

        /**
         * @brief
         * @en Read array of values from holding registers
         * @ru Читает массив значений из регистров вывода
         *
         * @param
         * @en regStart - address of first register
         * @ru regStart - адрес регистра, с которого необходимо начать чтение
         *
         * @param
         * @en regQty - quantity of registers reading
         * @ru regQty - количество читаемых значений регистров
         *
         * @param
         * @en values - array for values readed
         * @ru values - массив для прочитанных значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool readHoldingRegisters(quint16 regStart, quint16 regQty, QVector<quint16>& values);

        /**
         * @brief
         * @en Read value from input register
         * @ru Читает значение из регистра ввода
         *
         * @param regNo
         * @en regNo - address of register
         * @ru regNo - адрес регистра
         *
         * @param
         * @en value - readed value
         * @ru value - прочитанное значение
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool readInputRegister(quint16 regNo, quint16& value);

        /**
         * @brief
         * @en Read array of values from input registers
         * @ru Читает массив значений из регистров ввода
         *
         * @param regNo
         * @en regStart - address of first register
         * @ru regStart - адрес регистра, с которого необходимо начать чтение
         *
         * @param
         * @en regQty - quantity of registers reading
         * @ru regQty - количество читаемых значений регистров
         *
         * @param
         * @en values - array for values readed
         * @ru values - массив для прочитанных значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool readInputRegisters(quint16 regStart, quint16 regQty, QVector<quint16>& values);

        /**
         * @brief
         * Read response from slave device
         *
         * @return
         * Readed data array
         *
         * As timeout processing is differ for RTU and TCP we need to implement
         * method in descendance.
         */
        virtual QByteArray readResponse_() = 0;


//        /**
//         * @brief readSingle
//         * @param regNo
//         * @param value
//         * @return
//         *            @sa readFloat()
//         */
//        bool readSingle(quint16 regNo, float& value /* Single*/);

//        /**
//         * @brief readString
//         * @param regNo
//         * @param length
//         * @return
//         */
//        QString readString(quint16 regNo, quint16 length);

        /**
         * @brief
         * @en Return value for reading timeout
         * @ru Возвращает время ожидания ответа от сервера
         *
         * @return
         * @en Value for reading timeout
         * @ru Время ожидания ответа от сервера
         */
        int readTimeout() const;

        /**
         * @brief
         * @en Set new value for reading timeout
         * @ru Устанавливает время ожидания ответа от сервера
         *
         * @param
         * @en readTimeout - new value for reading timeout, ms
         * @ru readTimeout - максимальное время ожидания ответа от сервера, мс
         */
        void setReadTimeOut(int readTimeout);

        /**
         * @brief
         * @en Set current value of server unit ID
         * @ru Устанавливает текущее значение адреса устройства сервера, с которым идет обмен данными
         *
         * @param
         * @en unitID - server device unit id
         * @ru unitID - адрес устройства сервера
         */
        void setUnitID(quint8 unitID);

        /**
         * @brief
         * @en Set new value for reading timeout
         * @ru Устанавливает время ожидания ответа от сервера
         *
         * @param
         * @en readTimeout - new value for reading timeout, ms
         * @ru readTimeout - максимальное время ожидания ответа от сервера, мс
         */
        void setWriteTimeOut(int writeTimeout);

        /**
         * @brief
         * @en Write value to coil
         * @ru Записывает значение в дискретный выход
         *
         * @param regNo
         * @en regNo - address of coil
         * @ru regNo - адрес выхода, в который производится запись
         *
         * @param
         * @en value - value to write
         * @ru value - записываемое значение
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         * @en It is alias of writeSingleCoil() method for convinience.
         * @ru Этот метод является псевдонимом метода writeSingleCoil().
         */
        bool writeCoil(quint16 regNo, bool value);

        /**
         * @brief
         * @en Write array of values to coils
         * @ru Записывает массив значений в дискретные выходы
         *
         * @param
         * @en regStart - address of first coil
         * @ru regStart - адрес выхода, с которого необходимо начать запись
         *
         * @param
         * @en values - array for values to write
         * @ru values - массив записываемых значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         * @en It is alias of writeMultipleCoils() method for convinience.
         * @ru Этот метод является псевдонимом метода writeMultipleCoils().
         */
        bool writeCoils(quint16 regStart, const QVector<bool>& values);

        /**
         * @brief
         * @en Write value to holding register
         * @ru Записывает значение в регистр вывода
         *
         * @param
         * @en regAddress - address of register
         * @ru regAddress - адрес регистра
         *
         * @param
         * @en value - value to write
         * @ru value - записываемое значение
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         * @en It is alias of writeSingleRegister() method for convinience.
         * @ru Этот метод является псевдонимом метода writeSingleRegister().
         *
         */
        bool writeHoldingRegister(quint16 regAddress, quint16 value);

        /**
         * @brief
         * @en Write array of values to holding registers
         * @ru Записывает массив значений в регистры вывода
         *
         * @param
         * @en regStart - address of first register
         * @ru regStart - адрес регистра, с которого необходимо начать запись
         *
         * @param
         * @en values - array for values to write
         * @ru values - массив записываемых значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         * @en It is alias of writeMultipleRegisters() method for convinience.
         * @ru Этот метод является псевдонимом метода writeMultipleRegisters().
         *
         */
        bool writeHoldingRegisters(quint16 regStart, const QVector<quint16>& values);

        /**
         * @brief
         * @en Write array of values to coils
         * @ru Записывает массив значений в дискретные выходы
         *
         * @param
         * @en regStart - address of first coil
         * @ru regStart - адрес выхода, с которого необходимо начать запись
         *
         * @param
         * @en values - array for values to write
         * @ru values - массив записываемых значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool writeMultipleCoils(quint16 regStart, const QVector<bool>& values);

        /**
         * @brief
         * @en Write array of values to holding registers
         * @ru Записывает массив значений в регистры вывода
         *
         * @param
         * @en regStart - address of first register
         * @ru regStart - адрес регистра, с которого необходимо начать запись
         *
         * @param
         * @en values - array for values to write
         * @ru values - массив записываемых значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool writeMultipleRegisters(quint16 regStart, const QVector<quint16>& values);

        //        /**
//         * @brief writeDouble
//         * @param regNo
//         * @param value
//         * @return
//         */
//        bool writeDouble(quint16 regNo, double value);

//        /**
//         * @brief writeDWord
//         * @param regNo
//         * @param value
//         * @return
//         */
//        bool writeDWord(quint16 regNo, quint32 value);

//        /**
//         * @brief writeFloat
//         * @param regNo
//         * @param value
//         * @return
//         */
//        bool writeFloat(quint16 regNo, float value)
//        {
//            return writeSingle(regNo, value);
//        }

        /**
         * @brief
         * @en Write value to coil
         * @ru Записывает значение в дискретный выход
         *
         * @param
         * @en regAddress - address of coil
         * @ru regAddress - адрес выхода, в который производится запись
         *
         * @param
         * @en value - value to write
         * @ru value - записываемое значение
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool writeSingleCoil(quint16 regAddress, bool value);

        /**
         * @brief
         * @en Write value to holding register
         * @ru Записывает значение в регистр вывода
         *
         * @param
         * @en regAddress - address of register
         * @ru regAddress - адрес регистра
         *
         * @param
         * @en value - value to write
         * @ru value - записываемое значение
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool writeSingleRegister(quint16 regAddress, quint16 value);

//        /**
//         * @brief writeSingle
//         * @param regNo
//         * @param value
//         * @return
//         */
//        bool writeSingle(quint16 regNo, float value /* Single*/);

//        /**
//         * @brief writeString
//         * @param regNo
//         * @param text
//         * @return
//         */
//        bool writeString(quint16 regNo, const QString& text);

        /**
         * @brief
         * @en Return value for writing timeout
         * @ru Возвращает время ожидания передачи данных серверу
         *
         * @return
         * @en Value for reading timeout
         * @ru Время ожидания передачи данных серверу
         */
        int writeTimeout() const;

        /**
         * @brief
         * @en Return current value of server unit ID
         * @ru Возвращает текущее значение адреса устройства сервера, с которым идет обмен данными
         *
         * @return
         * @en Server unit id
         * @ru Адрес устройства сервера
         */
        quint8 unitID() const;

        /**
         * @brief
         * @en Send user defined function command to server
         * @ru Отправляет на сервер команду функции, определяемой пользователем (производителем)
         *
         * @param
         * @en function - Function code
         * @ru function - Код функции
         *
         * @param
         * @en data - Extra data to be sended to server
         * @ru data - Дополнительные данные, передаваемые серверу
         *
         * @param
         * @en retData - Vector to plase data returned from server
         * @ru retData - Массив, в который будет помещены данные, полученные от сервера
         *
         * @return
         * @en true if function successfull; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         */
        bool userDefinedFunction(quint8 function, const QVector<quint8>& data, QVector<quint8>& retData);

        /**
         * @brief
         * @en Send user defined function command to server
         * @ru Отправляет на сервер команду функции, определяемой пользователем (производителем)
         *
         * @param
         * @en function - Function code
         * @ru function - Код функции
         *
         * @param
         * @en subFunction - Sub function code
         * @ru subFunction - Код подфункции
         *
         * @param
         * @en data - Extra data to be sended to server
         * @ru data - Дополнительные данные, передаваемые серверу
         *
         * @param
         * @en retData - Vector to plase data returned from server
         * @ru retData - Массив, в который будет помещены данные, полученные от сервера
         *
         * @return
         * @en true if function successfull; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         */
        bool userDefinedFunction(quint8 function, quint8 subFunction, const QVector<quint8>& data, QVector<quint8>& retData);

    signals:

        //!
        //! \brief Signal for debugging informing
        //! \param msg - Debug message
        //!
        void debugMessage(const QString& msg);

        //!
        //! \brief Signal for informing about error occured
        //! \param msg - Message with error description
        //!
        void errorMessage(const QString& msg);

        //!
        //! \brief Signal for general purposes informing
        //! \param msg - message
        //!
        void infoMessage(const QString& msg);

        /**
         * @brief Signal for dbugging informing
         *
         * @param
         * @en unitID - unit ID of device
         * @en unitID - адрес устройства, при обмене данными с которым произошла ошибка
         *
         * @en msg - Message with error description
         * @ru msg - Строка с описанием ошибки
         */
        void debugMessage(quint8 unitID, const QString& msg);


        /**
         * @brief
         * @en Signal for informing about error occured
         * @ru Сигнал для сообщения о возникновении ошибки
         *
         * @param
         * @en unitID - unit ID of device
         * @en unitID - адрес устройства, при обмене данными с которым произошла ошибка
         *
         * @en msg - Message with error description
         * @ru msg - Строка с описанием ошибки
         */
        void errorMessage(quint8 unitID, const QString& msg);

        /**
         * @brief
         * @en Signal for informing about error occured
         * @ru Сигнал для сообщения о возникновении ошибки
         *
         * @param
         * @en unitID - unit ID of device
         * @en unitID - адрес устройства, при обмене данными с которым произошла ошибка
         *
         * @en msg - Message with error description
         * @ru msg - Строка с описанием ошибки
         */
        void infoMessage(quint8 unitID, const QString& msg);
};

} // namespace modbus4qt

#endif // MODBUS4QT_CLIENT_H
