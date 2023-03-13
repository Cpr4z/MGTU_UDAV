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


#ifndef ABSTRACT_TCP_SERVER_H
#define ABSTRACT_TCP_SERVER_H

#include <QObject>
#include <QTcpServer>

#include "types.h"

#include "global.h"

/*

unit IdModBusServer;

interface

uses
  Classes
 ,SysUtils
{$IFDEF DMB_INDY10}
 ,IdContext
 ,IdCustomTCPServer
 ,IdGlobal
{$ELSE}
 ,IdTCPClient
 ,IdTCPServer
{$ENDIF}
 ,ModBusConsts
 ,ModbusTypes
 ,ModbusUtils
 ,SyncObjs;


{$IFDEF DMB_INDY10}
type
  TModBusCoilReadEvent = procedure(const Sender: TIdContext;
    const RegNr, Count: Integer; var Data: TModCoilData;
    const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte) of object;
  TModBusRegisterReadEvent = procedure(const Sender: TIdContext;
    const RegNr, Count: Integer; var Data: TModRegisterData;
    const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte) of object;
  TModBusCoilWriteEvent = procedure(const Sender: TIdContext;
    const RegNr, Count: Integer; const Data: TModCoilData;
    const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte) of object;
  TModBusRegisterWriteEvent = procedure(const Sender: TIdContext;
    const RegNr, Count: Integer; const Data: TModRegisterData;
    const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte) of object;
  TModBusErrorEvent = procedure(const Sender: TIdContext;
    const FunctionCode: Byte; const ErrorCode: Byte;
    const RequestBuffer: TModBusRequestBuffer) of object;
  TModBusInvalidFunctionEvent = procedure(const Sender: TIdContext;
    const FunctionCode: TModBusFunction;
    const RequestBuffer: TModBusRequestBuffer) of object;
{$ELSE}
type
  TModBusCoilReadEvent = procedure(const Sender: TIdPeerThread;
    const RegNr, Count: Integer; var Data: TModCoilData;
    const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte) of object;
  TModBusRegisterReadEvent = procedure(const Sender: TIdPeerThread;
    const RegNr, Count: Integer; var Data: TModRegisterData;
    const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte) of object;
  TModBusCoilWriteEvent = procedure(const Sender: TIdPeerThread;
    const RegNr, Count: Integer; const Data: TModCoilData;
    const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte) of object;
  TModBusRegisterWriteEvent = procedure(const Sender: TIdPeerThread;
    const RegNr, Count: Integer; const Data: TModRegisterData;
    const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte) of object;
  TModBusErrorEvent = procedure(const Sender: TIdPeerThread;
    const FunctionCode: Byte; const ErrorCode: Byte;
    const RequestBuffer: TModBusRequestBuffer) of object;
  TModBusInvalidFunctionEvent = procedure(const Sender: TIdPeerThread;
    const FunctionCode: TModBusFunction;
    const RequestBuffer: TModBusRequestBuffer) of object;
{$ENDIF}

type
{$IFDEF DMB_INDY10}
  TIdModBusServer = class(TIdCustomTCPServer)
{$ELSE}
  TIdModBusServer = class(TIdTCPServer)
{$ENDIF}
  private
    FBaseRegister: Word;
    FOneShotConnection: Boolean;
    FLogCriticalSection: TCriticalSection;
    FLogEnabled: Boolean;
    FLogFile: String;
    FLogTimeFormat: String;
    FMaxRegister: Word;
    FMinRegister: Word;
    FOnError: TModBusErrorEvent;
    FOnInvalidFunction: TModBusInvalidFunctionEvent;
    FOnReadCoils: TModBusCoilReadEvent;
    FOnReadHoldingRegisters: TModBusRegisterReadEvent;
    FOnReadInputBits: TModBusCoilReadEvent;
    FOnReadInputRegisters: TModBusRegisterReadEvent;
    FOnWriteCoils: TModBusCoilWriteEvent;
    FOnWriteRegisters: TModBusRegisterWriteEvent;
    FPause: Boolean;
    FUnitID: Byte;
    function GetVersion: String;
    procedure SetVersion(const Value: String);
    function IsLogTimeFormatStored: Boolean;
    procedure LogByteBuffer(const LogType: String; const PeerIP: String; const ByteBuffer: array of Byte; const Size: Integer);
  {$IFDEF DMB_INDY10}
    procedure InternalReadCoils(const AContext: TIdContext; const RegNr, Count: Integer;
      var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
    procedure InternalReadInputBits(const AContext: TIdContext; const RegNr, Count: Integer;
      var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
    procedure InternalWriteCoils(const AContext: TIdContext; const RegNr, Count: Integer;
      const Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
  {$ELSE}
    procedure InternalReadCoils(const Sender: TIdPeerThread; const RegNr, Count: Integer;
      var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
    procedure InternalReadInputBits(const Sender: TIdPeerThread; const RegNr, Count: Integer;
      var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
    procedure InternalWriteCoils(const Sender: TIdPeerThread; const RegNr, Count: Integer;
      const Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
  {$ENDIF}
  protected
  {$IFDEF DMB_INDY10}
    procedure InitComponent; override;
  {$ENDIF}
  {$IFDEF DMB_INDY10}
    procedure DoError(const AContext: TIdContext; const FunctionCode: Byte;
      const ErrorCode: Byte; const RequestBuffer: TModBusRequestBuffer); virtual;
    function DoExecute(AContext: TIdContext): Boolean; override;
    procedure DoInvalidFunction(const AContext: TIdContext;
      const FunctionCode: TModBusFunction; const RequestBuffer: TModBusRequestBuffer); virtual;
    procedure DoReadHoldingRegisters(const AContext: TIdContext; const RegNr, Count: Integer;
      var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
    procedure DoReadInputRegisters(const AContext: TIdContext; const RegNr, Count: Integer;
      var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
    procedure DoReadCoils(const AContext: TIdContext; const RegNr, Count: Integer;
      var Data: TModCoilData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
    procedure DoReadInputBits(const AContext: TIdContext; const RegNr, Count: Integer;
      var Data: TModCoilData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
    procedure DoWriteCoils(const AContext: TIdContext; const RegNr, Count: Integer;
      const Data: TModCoilData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
    procedure DoWriteRegisters(const AContext: TIdContext; const RegNr, Count: Integer;
      const Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
    procedure LogExceptionBuffer(const AContext: TIdContext; const Buffer: TModBusExceptionBuffer);
    procedure LogRequestBuffer(const AContext: TIdContext; const Buffer: TModBusRequestBuffer; const Size: Integer);
    procedure LogResponseBuffer(const AContext: TIdContext; const Buffer: TModBusResponseBuffer; const Size: Integer);
    procedure ReadCommand(const AContext: TIdContext);
    procedure SendError(const AContext: TIdContext; const ErrorCode: Byte;
      const ReceiveBuffer: TModBusRequestBuffer);
    procedure SendResponse(const AContext: TIdContext; const ReceiveBuffer: TModBusRequestBuffer;
      const Data: TModRegisterData);
  {$ELSE}
    procedure DoError(const Sender: TIdPeerThread; const FunctionCode: Byte;
      const ErrorCode: Byte; const RequestBuffer: TModBusRequestBuffer); virtual;
    function DoExecute(AThread: TIdPeerThread): Boolean; override;
    procedure DoInvalidFunction(const Sender: TIdPeerThread; const FunctionCode: TModBusFunction;
      const RequestBuffer: TModBusRequestBuffer); virtual;
    procedure DoReadHoldingRegisters(const Sender: TIdPeerThread; const RegNr, Count: Integer;
      var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
    procedure DoReadInputRegisters(const Sender: TIdPeerThread; const RegNr, Count: Integer;
      var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
    procedure DoReadCoils(const Sender: TIdPeerThread; const RegNr, Count: Integer;
      var Data: TModCoilData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
    procedure DoReadInputBits(const Sender: TIdPeerThread; const RegNr, Count: Integer;
      var Data: TModCoilData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
    procedure DoWriteCoils(const Sender: TIdPeerThread; const RegNr, Count: Integer;
      const Data: TModCoilData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
    procedure DoWriteRegisters(const Sender: TIdPeerThread; const RegNr, Count: Integer;
      const Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
    procedure LogExceptionBuffer(const AThread: TIdPeerThread; const Buffer: TModBusExceptionBuffer);
    procedure LogRequestBuffer(const AThread: TIdPeerThread; const Buffer: TModBusRequestBuffer; const Size: Integer);
    procedure LogResponseBuffer(const AThread: TIdPeerThread; const Buffer: TModBusResponseBuffer; const Size: Integer);
    procedure ReadCommand(const AThread: TIdPeerThread);
    procedure SendError(const AThread: TIdPeerThread; const ErrorCode: Byte;
      const ReceiveBuffer: TModBusRequestBuffer);
    procedure SendResponse(const AThread: TIdPeerThread; const ReceiveBuffer: TModBusRequestBuffer;
      const Data: TModRegisterData);
  {$ENDIF}
  public
  {$IFNDEF DMB_INDY10}
    constructor Create(AOwner: TComponent); override;
  {$ENDIF}
  { public properties }
    property Pause: Boolean read FPause write FPause;
  published
    property BaseRegister: Word read FBaseRegister write FBaseRegister default 1;
    property DefaultPort default MB_PORT;
    property LogEnabled: Boolean read FLogEnabled write FLogEnabled default False;
    property LogFile: String read FLogFile write FLogFile;
    property LogTimeFormat: String read FLogTimeFormat write FLogTimeFormat stored IsLogTimeFormatStored;
    property OneShotConnection: Boolean read FOneShotConnection write FOneShotConnection default False;
    property MaxRegister: Word read FMaxRegister write FMaxRegister default $FFFF;
    property MinRegister: Word read FMinRegister write FMinRegister default 1;
    property UnitID: Byte read FUnitID write FUnitID default MB_IGNORE_UNITID;
    property Version: String read GetVersion write SetVersion stored False;
  { events }
    property OnError: TModBusErrorEvent read FOnError write FOnError;
    property OnInvalidFunction: TModBusInvalidFunctionEvent read FOnInvalidFunction write FOnInvalidFunction;
    property OnReadCoils: TModBusCoilReadEvent read FOnReadCoils write FOnReadCoils;
    property OnReadHoldingRegisters: TModBusRegisterReadEvent read FOnReadHoldingRegisters write FOnReadHoldingRegisters;
    property OnReadInputBits: TModBusCoilReadEvent read FOnReadInputBits write FOnReadInputBits;
    property OnReadInputRegisters: TModBusRegisterReadEvent read FOnReadInputRegisters write FOnReadInputRegisters;
    property OnWriteCoils: TModBusCoilWriteEvent read FOnWriteCoils write FOnWriteCoils;
    property OnWriteRegisters: TModBusRegisterWriteEvent read FOnWriteRegisters write FOnWriteRegisters;
  end; { TIdModBusServer }


*/

namespace modbus4qt
{

//! Абстрактный Tcp сервер для протокола modbus

/**

    Абстрактный сервер реализует функционал modbus сервера без привязки к
    какому-либо оборудованию контроллера или устройства.

    Функции чтения-записи данных регистров контроллера объявлены как чистые
    виртуальные фнукции.

    Для реализации общения с реальным контроллером или оборудованием необходимо
    создать класс-потомок, в котором надо реализовать функционал общения с
    оборудованием.

*/

class MODBUS4QT_EXPORT AbstractTcpServer : public QObject
{

    /*
    {$IFDEF DMB_INDY10}
      TIdModBusServer = class(TIdCustomTCPServer)
    {$ELSE}
      TIdModBusServer = class(TIdTCPServer)
    {$ENDIF}
      private
        FBaseRegister: Word;
        FOneShotConnection: Boolean;
        FLogCriticalSection: TCriticalSection;
        FLogEnabled: Boolean;
        FLogFile: String;
        FLogTimeFormat: String;
        FMaxRegister: Word;
        FMinRegister: Word;
        FOnError: TModBusErrorEvent;
        FOnInvalidFunction: TModBusInvalidFunctionEvent;
        FOnReadCoils: TModBusCoilReadEvent;
        FOnReadHoldingRegisters: TModBusRegisterReadEvent;
        FOnReadInputBits: TModBusCoilReadEvent;
        FOnReadInputRegisters: TModBusRegisterReadEvent;
        FOnWriteCoils: TModBusCoilWriteEvent;
        FOnWriteRegisters: TModBusRegisterWriteEvent;
        FPause: Boolean;
        FUnitID: Byte;
        function GetVersion: String;
        procedure SetVersion(const Value: String);
        function IsLogTimeFormatStored: Boolean;
      {$IFDEF DMB_INDY10}
        procedure InternalReadCoils(const AContext: TIdContext; const RegNr, Count: Integer;
          var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
        procedure InternalReadInputBits(const AContext: TIdContext; const RegNr, Count: Integer;
          var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
        procedure InternalWriteCoils(const AContext: TIdContext; const RegNr, Count: Integer;
          const Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
      {$ELSE}
        procedure InternalReadCoils(const Sender: TIdPeerThread; const RegNr, Count: Integer;
          var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
        procedure InternalReadInputBits(const Sender: TIdPeerThread; const RegNr, Count: Integer;
          var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
        procedure InternalWriteCoils(const Sender: TIdPeerThread; const RegNr, Count: Integer;
          const Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
      {$ENDIF}
      protected
      {$IFDEF DMB_INDY10}
        procedure InitComponent; override;
      {$ENDIF}
      {$IFDEF DMB_INDY10}
        procedure DoError(const AContext: TIdContext; const FunctionCode: Byte;
          const ErrorCode: Byte; const RequestBuffer: TModBusRequestBuffer); virtual;
        function DoExecute(AContext: TIdContext): Boolean; override;
        procedure DoInvalidFunction(const AContext: TIdContext;
          const FunctionCode: TModBusFunction; const RequestBuffer: TModBusRequestBuffer); virtual;
        procedure DoReadHoldingRegisters(const AContext: TIdContext; const RegNr, Count: Integer;
          var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
        procedure DoReadInputRegisters(const AContext: TIdContext; const RegNr, Count: Integer;
          var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
        procedure DoReadCoils(const AContext: TIdContext; const RegNr, Count: Integer;
          var Data: TModCoilData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
        procedure DoReadInputBits(const AContext: TIdContext; const RegNr, Count: Integer;
          var Data: TModCoilData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
        procedure DoWriteCoils(const AContext: TIdContext; const RegNr, Count: Integer;
          const Data: TModCoilData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
        procedure DoWriteRegisters(const AContext: TIdContext; const RegNr, Count: Integer;
          const Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
        procedure LogExceptionBuffer(const AContext: TIdContext; const Buffer: TModBusExceptionBuffer);
        procedure LogRequestBuffer(const AContext: TIdContext; const Buffer: TModBusRequestBuffer; const Size: Integer);
        procedure LogResponseBuffer(const AContext: TIdContext; const Buffer: TModBusResponseBuffer; const Size: Integer);
        procedure ReadCommand(const AContext: TIdContext);
        procedure SendError(const AContext: TIdContext; const ErrorCode: Byte;
          const ReceiveBuffer: TModBusRequestBuffer);
        procedure SendResponse(const AContext: TIdContext; const ReceiveBuffer: TModBusRequestBuffer;
          const Data: TModRegisterData);
      {$ELSE}
        procedure DoError(const Sender: TIdPeerThread; const FunctionCode: Byte;
          const ErrorCode: Byte; const RequestBuffer: TModBusRequestBuffer); virtual;
        function DoExecute(AThread: TIdPeerThread): Boolean; override;
        procedure DoInvalidFunction(const Sender: TIdPeerThread; const FunctionCode: TModBusFunction;
          const RequestBuffer: TModBusRequestBuffer); virtual;
        procedure DoReadHoldingRegisters(const Sender: TIdPeerThread; const RegNr, Count: Integer;
          var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
        procedure DoReadInputRegisters(const Sender: TIdPeerThread; const RegNr, Count: Integer;
          var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
        procedure DoReadCoils(const Sender: TIdPeerThread; const RegNr, Count: Integer;
          var Data: TModCoilData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
        procedure DoReadInputBits(const Sender: TIdPeerThread; const RegNr, Count: Integer;
          var Data: TModCoilData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
        procedure DoWriteCoils(const Sender: TIdPeerThread; const RegNr, Count: Integer;
          const Data: TModCoilData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
        procedure DoWriteRegisters(const Sender: TIdPeerThread; const RegNr, Count: Integer;
          const Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte); virtual;
        procedure LogExceptionBuffer(const AThread: TIdPeerThread; const Buffer: TModBusExceptionBuffer);
        procedure LogRequestBuffer(const AThread: TIdPeerThread; const Buffer: TModBusRequestBuffer; const Size: Integer);
        procedure LogResponseBuffer(const AThread: TIdPeerThread; const Buffer: TModBusResponseBuffer; const Size: Integer);
        procedure ReadCommand(const AThread: TIdPeerThread);
        procedure SendError(const AThread: TIdPeerThread; const ErrorCode: Byte;
          const ReceiveBuffer: TModBusRequestBuffer);
        procedure SendResponse(const AThread: TIdPeerThread; const ReceiveBuffer: TModBusRequestBuffer;
          const Data: TModRegisterData);
      {$ENDIF}
      public
      {$IFNDEF DMB_INDY10}
        constructor Create(AOwner: TComponent); override;
      {$ENDIF}

    */

    Q_OBJECT

    private :

        //! Номер регистра, от которого ведется нумерация регистров по умолчанию
        /**
            Значение по умолчанию: 0
        */
        quint16 baseRegister_;

        /* FLogCriticalSection := SyncObjs.TCriticalSection.Create; */

        //! Ведение лог-файла включено или выключено
        /**
            Значение по умолчанию: false
        */
        bool logEnabled_;

        //! Имя лог-файла
        /**
            Значение по умолчанию: пустая строка
        */
        QString logFileName_;

        //! Формат вывода времени в лог-файл
        /**
          @sa DefaultLogTimeFormat
        */
        QString logTimeFormat_;

        //! Максимальный номер регистра
        /**
            Значение по умолчанию: 0xffff
        */
        quint16 maxRegister_;

        //! Мимнимальный номер регистра
        /**
            Значение по умолчанию: 0x0
        */
        quint16 minRegister_;

        //! Флаг, показывающий необходимость закрытия соединения после каждого сеанса обмена данными с клиентом.
        /**
            Значение по умолчанию: false
        */
        bool oneShotConnection_;

        //! Флаг, показывающий, что сервер приостановлен
        /**
            Значение по умолчанию: false
        */
        bool pause_;

        //! Номер TCP порта
        /**
            Значение по умолчанию: 502
            @sa Port
        */
        int port_;

        //! Идентификатор устройства сервера
        /**
            Значение по умолчанию: 255 @sa IgnoreUnitId
        */
        quint8 unitID_;

        //! Сервер, принимающий подключения
        QTcpServer tcpServer_;

        //! Сокет, через который осуществляется подключение клиента
        QTcpSocket* tcpSocket_;


/*
{ events }
  property OnError: TModBusErrorEvent read FOnError write FOnError;
  property OnInvalidFunction: TModBusInvalidFunctionEvent read FOnInvalidFunction write FOnInvalidFunction;
  property OnReadCoils: TModBusCoilReadEvent read FOnReadCoils write FOnReadCoils;
  property OnReadHoldingRegisters: TModBusRegisterReadEvent read FOnReadHoldingRegisters write FOnReadHoldingRegisters;
  property OnReadInputBits: TModBusCoilReadEvent read FOnReadInputBits write FOnReadInputBits;
  property OnReadInputRegisters: TModBusRegisterReadEvent read FOnReadInputRegisters write FOnReadInputRegisters;
  property OnWriteCoils: TModBusCoilWriteEvent read FOnWriteCoils write FOnWriteCoils;
  property OnWriteRegisters: TModBusRegisterWriteEvent read FOnWriteRegisters write FOnWriteRegisters;
end; { TIdModBusServer }
*/

/*
        procedure DoError(const Sender: TIdPeerThread; const FunctionCode: Byte;
          const ErrorCode: Byte; const RequestBuffer: TModBusRequestBuffer); virtual;
*/
        //! @todo Похоже, что надо сделать через слот/сигнал
        void doError_(quint8 functionCode, quint8 errorCode, const RequestBuffer& requestBuffer);

        //! Возвращает номер регистра с учетом регистра, от которого ведется отсчет
        /**
          @sa baseRegister_
        */
        quint16 getRegNum_(quint16 regNum) const
        {

        /*
        function GetRegNr(const RegNr: Integer): Integer;
        begin
          Result := RegNr;
          if (RegNr < 0) then
            Result := Result + $FFFF
          else if (RegNr > $FFFF) then
            Result := RegNr - ($FFFF + 1);
          Result := Result + FBaseRegister;
        end; { GetRegNr }
        */

            return regNum + baseRegister_;
        }


        //! Возвращает строку, содержающую обозначение версии библиотеки
        /**
            Готовность: полностью готова <br>
            Исходное объявление: function GetVersion: String;
        */
        QString getVersion_() const;

        /*procedure InternalReadInputBits(const Sender: TIdPeerThread; const RegNr, Count: Integer;
          var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);*/
        /*procedure InternalWriteCoils(const Sender: TIdPeerThread; const RegNr, Count: Integer;
          const Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);*/

        //! Проверяет соответствует ли формат вывода времени и даты в лог-файл значению по умолчанию
        /**
          @return true, если соответствует; false в противном случае
        */
        bool isLogTimeFormatStored_() const;

        /**
            Исходное объявление: procedure LogByteBuffer(const LogType: String; const PeerIP: String; const ByteBuffer: array of Byte; const Size: Integer);
        */
        void logByteBuffer_(const QString& logType, const QString peerIP, const QByteArray& buffer) const;

        void logExceptionBuffer_(const ExceptionBuffer& buffer) const;

        void logRequestBuffer_(const RequestBuffer& buffer) const;

        void logResponseBuffer_(const ResponseBuffer& buffer) const;

        //! Передает клиенту сообщение об ошибке
        /**
          @param errorCode - код ошибки
          @param recieveBuffer - данные, полученные от клиента при обращении к серверу
        */
        void sendError_(quint8 errorCode, const RequestBuffer& recieveBuffer);

        void sendResponse_(const RequestBuffer& recieveBuffer, QVector< quint16 > &registerData);

        //! Пустая фукнция. Поле версии предназначено только для чтения.
        /** @todo To be deleted! */
        void setVersion_(const QString& value);

    protected :

        /**
            Исходное объявление: procedure ReadCommand(const AContext: TIdContext);
        */
        virtual void readCommand_() = 0;

        //! Производит чтение данных витков устройства. Должна быть реализована в классе-потомке.
        /**
          Исходное объявление:
            @code
            procedure InternalReadCoils(const Sender: TIdPeerThread; const RegNr, Count: Integer;
            var Data: TModRegisterData; const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
            @endcode
        */
        virtual quint8 readCoils_(quint8 regNum, quint8 regCount, RegisterData* data, RequestBuffer& buf) = 0;

        virtual void readInputBits_() = 0;

        virtual void readInputRegs_() = 0;

        virtual void readHoldingRegs_() = 0;

        virtual void writeOneReg_() = 0;

        virtual void writeRegs_() = 0;

        virtual void writeCoils_() = 0;

    public :

        //! Конструктор по умполчанию
        explicit AbstractTcpServer(QObject *parent = 0);

        //! Проверяет, приостановлена ли работа сервера
        /**
          @return true, если работа сервера приостановлена; false в противном случае
        */
        bool isPaused() const
        {
            return pause_;
        }

        //! Присваивает имя файлу протокола работы
        void setLogFileName(const QString logFileName)
        {
            logFileName_ = logFileName;
        }

        //! Приостанавливает работу сервера, устанавливая режим паузы, или запускает отменяя режим паузы
        /**
          Если передано значение true, то сервер переводится в режим паузы
          независимо от текущего режима работы. Если передано значение false,
          то сервер переводится в рабочий режим.
          @param pause - новое значение флага режима паузы
        */
        void setPause(bool pause)
        {
            pause_ = pause;
        }

    signals:

    private slots:
        /**
            Вызывается всякий раз, когда клиент пытается подсоединиться к порту,
            который прослушивает сервер.

            Присваивает переменной tcpSocket_ адрес сокета, через который
            установлена связь с клиентом.
        */
        void incomingConnection_();

        /**
            Вызывается когда сокет сообщает о завершении соединения. Дает команду на
            удаление сокета и присваивает переменной tcpSocket_ значение NULL.
        */
        void disconnected_();

    public slots:

};

}

#endif // ABSTRACT_TCP_SERVER_H
