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

#include "abstract_tcp_server.h"
#include "utils.h"
#include "consts.h"

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QByteArray>
#include <QTcpSocket>

#include <iostream>

namespace modbus4qt
{

/*
implementation

uses
  Math;

{ TIdModBusServer }



{$IFDEF DMB_INDY10}
procedure TIdModBusServer.InternalReadCoils(const AContext: TIdContext;
  const RegNr, Count: Integer; var Data: TModRegisterData;
    const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ELSE}
procedure TIdModBusServer.InternalReadCoils(const Sender: TIdPeerThread;
  const RegNr, Count: Integer; var Data: TModRegisterData;
    const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ENDIF}
var
  CoilData: TModCoilData;
  i: Integer;
begin
  FillChar(CoilData, SizeOf(CoilData), 0);
{$IFDEF DMB_INDY10}
  DoReadCoils(AContext, RegNr, Count, CoilData, RequestBuffer, ErrorCode);
{$ELSE}
  DoReadCoils(Sender, RegNr, Count, CoilData, RequestBuffer, ErrorCode);
{$ENDIF}
  for i := 0 to (Count - 1) do
  begin
    if CoilData[i] then
      Data[i] := 1;
  end;
end;


{$IFDEF DMB_INDY10}
procedure TIdModBusServer.InternalReadInputBits(const AContext: TIdContext;
  const RegNr, Count: Integer; var Data: TModRegisterData;
  const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ELSE}
procedure TIdModBusServer.InternalReadInputBits(const Sender: TIdPeerThread;
  const RegNr, Count: Integer; var Data: TModRegisterData;
  const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ENDIF}
var
  CoilData: TModCoilData;
  i: Integer;
begin
  FillChar(CoilData, SizeOf(CoilData), 0);
{$IFDEF DMB_INDY10}
  DoReadInputBits(AContext, RegNr, Count, CoilData, RequestBuffer, ErrorCode);
{$ELSE}
  DoReadInputBits(Sender, RegNr, Count, CoilData, RequestBuffer, ErrorCode);
{$ENDIF}
  for i := 0 to (Count - 1) do
  begin
    if CoilData[i] then
      Data[i] := 1;
  end;

end;


{$IFDEF DMB_INDY10}
procedure TIdModBusServer.InternalWriteCoils(const AContext: TIdContext;
  const RegNr, Count: Integer; const Data: TModRegisterData;
  const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ELSE}
procedure TIdModBusServer.InternalWriteCoils(const Sender: TIdPeerThread;
  const RegNr, Count: Integer; const Data: TModRegisterData;
  const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ENDIF}
var
  CoilData: TModCoilData;
  i: Integer;
begin
  FillChar(CoilData, SizeOf(CoilData), 0);
  for i := 0 to (Count - 1) do
    CoilData[i] := (Data[i] = 1);
{$IFDEF DMB_INDY10}
  DoWriteCoils(AContext, RegNr, Count, CoilData, RequestBuffer, ErrorCode);
{$ELSE}
  DoWriteCoils(Sender, RegNr, Count, CoilData, RequestBuffer, ErrorCode);
{$ENDIF}
end;




{$IFDEF DMB_INDY10}
procedure TIdModBusServer.DoError(const AContext: TIdContext;
  const FunctionCode: Byte; const ErrorCode: Byte; const RequestBuffer: TModBusRequestBuffer);
{$ELSE}
procedure TIdModBusServer.DoError(const Sender: TIdPeerThread;
  const FunctionCode: Byte; const ErrorCode: Byte; const RequestBuffer: TModBusRequestBuffer);
{$ENDIF}
begin
  if Assigned(FOnError) then
  {$IFDEF DMB_INDY10}
    FOnError(AContext, FunctionCode, ErrorCode, RequestBuffer);
  {$ELSE}
    FOnError(Sender, FunctionCode, ErrorCode, RequestBuffer);
  {$ENDIF}
end;


{$IFDEF DMB_INDY10}
function TIdModBusServer.DoExecute(AContext: TIdContext): Boolean;
{$ELSE}
function TIdModBusServer.DoExecute(AThread: TIdPeerThread): Boolean;
{$ENDIF}
begin
  Result := False;
  if not FPause then
  begin
  {$IFDEF DMB_INDY10}
    ReadCommand(AContext);
    Result := inherited DoExecute(AContext);
  {$ELSE}
    ReadCommand(AThread);
    Result := inherited DoExecute(AThread);
  {$ENDIF}
  end;
end;


{$IFDEF DMB_INDY10}
procedure TIdModBusServer.DoInvalidFunction(const AContext: TIdContext;
  const FunctionCode: TModBusFunction; const RequestBuffer: TModBusRequestBuffer);
{$ELSE}
procedure TIdModBusServer.DoInvalidFunction(const Sender: TIdPeerThread;
  const FunctionCode: TModBusFunction; const RequestBuffer: TModBusRequestBuffer);
{$ENDIF}
begin
  if Assigned(FOnInvalidFunction) then
  {$IFDEF DMB_INDY10}
    FOnInvalidFunction(AContext, FunctionCode, RequestBuffer);
  {$ELSE}
    FOnInvalidFunction(Sender, FunctionCode, RequestBuffer);
  {$ENDIF}
end;


{$IFDEF DMB_INDY10}
procedure TIdModBusServer.DoReadCoils(const AContext: TIdContext;
  const RegNr, Count: Integer; var Data: TModCoilData;
  const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ELSE}
procedure TIdModBusServer.DoReadCoils(const Sender: TIdPeerThread;
  const RegNr, Count: Integer; var Data: TModCoilData;
  const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ENDIF}
begin
  if Assigned(FOnReadCoils) then
  {$IFDEF DMB_INDY10}
    FOnReadCoils(AContext, RegNr, Count, Data, RequestBuffer, ErrorCode);
  {$ELSE}
    FOnReadCoils(Sender, RegNr, Count, Data, RequestBuffer, ErrorCode);
  {$ENDIF}
end;


{$IFDEF DMB_INDY10}
procedure TIdModBusServer.DoReadInputBits(const AContext: TIdContext;
  const RegNr, Count: Integer; var Data: TModCoilData;
  const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ELSE}
procedure TIdModBusServer.DoReadInputBits(const Sender: TIdPeerThread;
  const RegNr, Count: Integer; var Data: TModCoilData;
  const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ENDIF}
begin
  if Assigned(FOnReadInputBits) then
  {$IFDEF DMB_INDY10}
    FOnReadInputBits(AContext, RegNr, Count, Data, RequestBuffer, ErrorCode);
  {$ELSE}
    FOnReadInputBits(Sender, RegNr, Count, Data, RequestBuffer, ErrorCode);
  {$ENDIF}
end;


{$IFDEF DMB_INDY10}
procedure TIdModBusServer.DoReadHoldingRegisters(const AContext: TIdContext;
  const RegNr, Count: Integer; var Data: TModRegisterData;
  const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ELSE}
procedure TIdModBusServer.DoReadHoldingRegisters(const Sender: TIdPeerThread;
  const RegNr, Count: Integer; var Data: TModRegisterData;
  const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ENDIF}
begin
  if Assigned(FOnReadHoldingRegisters) then
  {$IFDEF DMB_INDY10}
    FOnReadHoldingRegisters(AContext, RegNr, Count, Data, RequestBuffer, ErrorCode);
  {$ELSE}
    FOnReadHoldingRegisters(Sender, RegNr, Count, Data, RequestBuffer, ErrorCode);
  {$ENDIF}
end;


{$IFDEF DMB_INDY10}
procedure TIdModBusServer.DoReadInputRegisters(const AContext: TIdContext;
  const RegNr, Count: Integer; var Data: TModRegisterData;
  const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ELSE}
procedure TIdModBusServer.DoReadInputRegisters(const Sender: TIdPeerThread;
  const RegNr, Count: Integer; var Data: TModRegisterData;
  const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ENDIF}
begin
  if Assigned(FOnReadInputRegisters) then
  {$IFDEF DMB_INDY10}
    FOnReadInputRegisters(AContext, RegNr, Count, Data, RequestBuffer, ErrorCode);
  {$ELSE}
    FOnReadInputRegisters(Sender, RegNr, Count, Data, RequestBuffer, ErrorCode);
  {$ENDIF}
end;


{$IFDEF DMB_INDY10}
procedure TIdModBusServer.DoWriteCoils(const AContext: TIdContext;
  const RegNr, Count: Integer; const Data: TModCoilData;
  const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ELSE}
procedure TIdModBusServer.DoWriteCoils(const Sender: TIdPeerThread;
  const RegNr, Count: Integer; const Data: TModCoilData;
  const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ENDIF}
begin
  if Assigned(FOnWriteCoils) then
  {$IFDEF DMB_INDY10}
    FOnWriteCoils(AContext, RegNr, Count, Data, RequestBuffer, ErrorCode);
  {$ELSE}
    FOnWriteCoils(Sender, RegNr, Count, Data, RequestBuffer, ErrorCode);
  {$ENDIF}
end;


{$IFDEF DMB_INDY10}
procedure TIdModBusServer.DoWriteRegisters(const AContext: TIdContext;
  const RegNr, Count: Integer; const Data: TModRegisterData;
  const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ELSE}
procedure TIdModBusServer.DoWriteRegisters(const Sender: TIdPeerThread;
const RegNr, Count: Integer; const Data: TModRegisterData;
const RequestBuffer: TModBusRequestBuffer; var ErrorCode: Byte);
{$ENDIF}
begin
  if Assigned(FOnWriteRegisters) then
  {$IFDEF DMB_INDY10}
    FOnWriteRegisters(AContext, RegNr, Count, Data, RequestBuffer, ErrorCode);
  {$ELSE}
    FOnWriteRegisters(Sender, RegNr, Count, Data, RequestBuffer, ErrorCode);
  {$ENDIF}
end;


*/

AbstractTcpServer::AbstractTcpServer(QObject *parent) :
    QObject(parent)
//	QTcpServer(parent)
{
    /*
    {$IFDEF DMB_INDY10}
    procedure TIdModBusServer.InitComponent;
    {$ELSE}
    constructor TIdModBusServer.Create(AOwner: TComponent);
    {$ENDIF}
    begin
    {$IFDEF DMB_INDY10}
      inherited;
    {$ELSE}
      inherited Create(AOwner);
    {$ENDIF}
      FBaseRegister := 1;
      DefaultPort := MB_PORT;
      FLogCriticalSection := SyncObjs.TCriticalSection.Create;
      FLogEnabled := False;
      FLogFile := '';
      FLogTimeFormat := DefaultLogTimeFormat;
      FMaxRegister := $FFFF;
      FMinRegister := 1;
      FOneShotConnection := False;
      FOnError := nil;
      FOnInvalidFunction := nil;
      FOnReadCoils := nil;
      FOnReadHoldingRegisters := nil;
      FOnReadInputBits := nil;
      FOnReadInputRegisters := nil;
      FOnWriteCoils := nil;
      FOnWriteRegisters := nil;
      FPause := False;
      FUnitID := MB_IGNORE_UNITID;
    end;
    */
    baseRegister_ = 0;

    /* FLogCriticalSection := SyncObjs.TCriticalSection.Create; */

    logEnabled_ = false;
    logFileName_ = QString("");
    logTimeFormat_ = DefaultLogTimeFormat;
    maxRegister_ = 0xffff;
    minRegister_ = 0;
    oneShotConnection_ = false;
    /*
        FOnError := nil;
        FOnInvalidFunction := nil;
        FOnReadCoils := nil;
        FOnReadHoldingRegisters := nil;
        FOnReadInputBits := nil;
        FOnReadInputRegisters := nil;
        FOnWriteCoils := nil;
        FOnWriteRegisters := nil;
    */
    pause_ = false;
    port_ = DefaultTcpPort;
    unitID_ = IgnoreUnitId;

    tcpSocket_ = NULL;

    connect(&tcpServer_, SIGNAL(newConnection()), this, SLOT(incomingConnection_()));

    //! @todo Вынести старт/стоп сервера в отдельный метод и сделать отработку ошибок подключения
    if (!tcpServer_.listen(QHostAddress::Any, port_))
    {
        std::cerr << "Failed to bind MODBUS TcpServer to port" << std::endl;
    }
}

//-----------------------------------------------------------------------------

void
AbstractTcpServer::disconnected_()
{
    tcpSocket_->deleteLater();
    tcpSocket_ = NULL;
}

//-----------------------------------------------------------------------------

void
AbstractTcpServer::doError_(quint8 functionCode, quint8 errorCode, const RequestBuffer& requestBuffer)
{
/*
    {$IFDEF DMB_INDY10}
    procedure TIdModBusServer.DoError(const AContext: TIdContext;
      const FunctionCode: Byte; const ErrorCode: Byte; const RequestBuffer: TModBusRequestBuffer);
    {$ELSE}
    procedure TIdModBusServer.DoError(const Sender: TIdPeerThread;
      const FunctionCode: Byte; const ErrorCode: Byte; const RequestBuffer: TModBusRequestBuffer);
    {$ENDIF}
    begin
      if Assigned(FOnError) then
      {$IFDEF DMB_INDY10}
        FOnError(AContext, FunctionCode, ErrorCode, RequestBuffer);
      {$ELSE}
        FOnError(Sender, FunctionCode, ErrorCode, RequestBuffer);
      {$ENDIF}
    end;
*/
}


//-----------------------------------------------------------------------------

QString
AbstractTcpServer::getVersion_() const
{
    return QString(MODBUSTCPVersion);
}

//-----------------------------------------------------------------------------

void
AbstractTcpServer::incomingConnection_()
{
    tcpSocket_ = tcpServer_.nextPendingConnection();
    connect(tcpSocket_, SIGNAL(disconnected()), this, SLOT(disconnected_()));

    readCommand_();
}

//-----------------------------------------------------------------------------

bool
AbstractTcpServer::isLogTimeFormatStored_() const
{
    /*
    function TIdModBusServer.IsLogTimeFormatStored: Boolean;
    begin
      Result := (FLogTimeFormat <> DefaultLogTimeFormat);
    end;
    */
    return (logTimeFormat_ == DefaultLogTimeFormat);
}

//-----------------------------------------------------------------------------

void
AbstractTcpServer::logByteBuffer_(const QString& logType, const QString peerIP, const QByteArray& buffer) const
{
    /*
    procedure TIdModBusServer.LogByteBuffer(const LogType: String;
      const PeerIP: String; const ByteBuffer: array of Byte; const Size: Integer);
    var
      F: TextFile;
    begin
      if FLogEnabled and (FLogFile <> '') then
      begin
        FLogCriticalSection.Enter;
        try
          AssignFile(F, FLogFile);
          if FileExists(FLogFile) then
            Append(F)
          else
            Rewrite(F);
          try
            WriteLn(F, FormatDateTime(FLogTimeFormat, Now)
                      ,'; ', LogType
                      ,'; ', PeerIP
                      ,'; ', IntToStr(Size)
                      ,'; ', BufferToHex(ByteBuffer));
          finally
            CloseFile(F);
          end;
        finally
          FLogCriticalSection.Leave;
        end;
      end;
    end;
    */

    if (logEnabled_ && !logFileName_.isEmpty())
    {
        QFile f(logFileName_);
        if (f.open(QFile::ReadWrite | QFile::Text))
        {
            const QString Tab = "; ";

            QTextStream os(&f);
            os.setCodec("UTF-8");

            os << QDateTime::currentDateTime().toString(DefaultLogTimeFormat) << Tab
               << logType << Tab
               << peerIP << Tab
               << buffer.size() << Tab
               << bufferToHex(buffer)
               << '\n';

            f.close();
        }
    }
}

//-----------------------------------------------------------------------------

void
AbstractTcpServer::logExceptionBuffer_(const ExceptionBuffer& buffer) const
{
    /*
    {$IFDEF DMB_INDY10}
    procedure TIdModBusServer.LogExceptionBuffer(const AContext: TIdContext;
      const Buffer: TModBusExceptionBuffer);
    {$ELSE}
    procedure TIdModBusServer.LogExceptionBuffer(const AThread: TIdPeerThread;
      const Buffer: TModBusExceptionBuffer);
    {$ENDIF}
    var
      PeerIP: String;
      ByteBuffer: array of Byte;
    begin
    {$IFDEF DMB_INDY10}
      PeerIP := AContext.Connection.Socket.Binding.PeerIP;
    {$ELSE}
      PeerIP := AThread.Connection.Socket.Binding.PeerIP;
    {$ENDIF}
      SetLength(ByteBuffer, SizeOf(Buffer));
      Move(Buffer, ByteBuffer[0], SizeOf(Buffer));
      LogByteBuffer('excp', PeerIP, ByteBuffer, SizeOf(Buffer));
    end;
    */

    //! @todo Отработать получение ip адреса подключенного клиента

    QByteArray byteBuffer = QByteArray::fromRawData((const char*)(&buffer), sizeof(buffer));
    logByteBuffer_("excp", "PeerIP", byteBuffer);
}

//-----------------------------------------------------------------------------

void
AbstractTcpServer::logRequestBuffer_(const RequestBuffer& buffer) const
{
    //! @todo Отработать получение ip адреса подключенного клиента
    QByteArray byteBuffer = QByteArray::fromRawData((const char*)(&buffer), sizeof(buffer));
    logByteBuffer_("recv", "PeerIP", byteBuffer);
}

//-----------------------------------------------------------------------------

void
AbstractTcpServer::logResponseBuffer_(const ResponseBuffer& buffer) const
{
    //! @todo Отработать получение ip адреса подключенного клиента
    QByteArray byteBuffer = QByteArray::fromRawData((const char*)(&buffer), sizeof(buffer));
    logByteBuffer_("sent", "PeerIP", byteBuffer);
}

//-----------------------------------------------------------------------------

void
AbstractTcpServer::readCommand_()
{
/*
{$IFDEF DMB_INDY10}
procedure TIdModBusServer.ReadCommand(const AContext: TIdContext);
{$ELSE}
procedure TIdModBusServer.ReadCommand(const AThread: TIdPeerThread);
{$ENDIF}

  function GetRegNr(const RegNr: Integer): Integer;
  begin
    Result := RegNr;
    if (RegNr < 0) then
      Result := Result + $FFFF
    else if (RegNr > $FFFF) then
      Result := RegNr - ($FFFF + 1);
    Result := Result + FBaseRegister;
  end; { GetRegNr }

var
  iCount: Integer;
  iRegNr: Integer;
  ErrorCode: Byte;
  ReceiveBuffer: TModBusRequestBuffer;
  Data: TModRegisterData;
{$IFDEF DMB_INDY10}
  Buffer: TIdBytes;
{$ENDIF}
begin
{ Initialize all register data to 0 }
  FillChar(Data[0], SizeOf(Data), 0);
  FillChar(ReceiveBuffer, SizeOf(ReceiveBuffer), 0);
{ Read the data from the peer connection }
{$IFDEF DMB_INDY10}
{ Ensure receiving databuffer is completely empty, and filled with zeros }
  SetLength(Buffer, SizeOf(ReceiveBuffer));
  FillChar(Buffer[0], SizeOf(ReceiveBuffer), 0);
{ Wait max. 250 msecs. for available data }
  AContext.Connection.IOHandler.CheckForDataOnSource(250);
  if not AContext.Connection.IOHandler.InputBufferIsEmpty then
  begin
    AContext.Connection.IOHandler.InputBuffer.ExtractToBytes(Buffer, -1, False, -1);
    iCount := Length(Buffer);
    if (iCount > 0) then
    begin
      Move(Buffer[0], ReceiveBuffer, Min(iCount, SizeOf(ReceiveBuffer)));
      if FLogEnabled then
        LogRequestBuffer(AContext, ReceiveBuffer, iCount);
    end
    else
      Exit;
  end
  else
    Exit;
{$ELSE}
  iCount := AThread.Connection.Socket.Recv(ReceiveBuffer, SizeOf(ReceiveBuffer));
  if (iCount > 0) then
  begin
    if FLogEnabled then
      LogRequestBuffer(AThread, ReceiveBuffer, iCount);
  end
  else
    Exit;
{$ENDIF}
{ Process the data }
  if ((FUnitID <> MB_IGNORE_UNITID) and (ReceiveBuffer.Header.UnitID <> FUnitID)) or
     (ReceiveBuffer.Header.ProtocolID <> MB_PROTOCOL)
  then
  begin
  // When listening for a specific UnitID, only except data for that ID
  {$IFDEF DMB_INDY10}
    SendError(AContext, mbeServerFailure, ReceiveBuffer);
  {$ELSE}
    SendError(AThread, mbeServerFailure, ReceiveBuffer);
  {$ENDIF}
  end
  else if ((Byte(ReceiveBuffer.FunctionCode) and not $80) <> 0) then
  begin
    ErrorCode := Integer(ReceiveBuffer.MBPData[0]);
  {$IFDEF DMB_INDY10}
    DoError(AContext, ReceiveBuffer.FunctionCode and not $80, ErrorCode, ReceiveBuffer);
  {$ELSE}
    DoError(AThread, ReceiveBuffer.FunctionCode and not $80, ErrorCode, ReceiveBuffer);
  {$ENDIF}
  end
*/

    RequestBuffer requestBuffer;
    QDataStream in(tcpSocket_);

    qint64 bytes = tcpSocket_->bytesAvailable();

    if (bytes)
    {
        in.readRawData((char *)(&requestBuffer), bytes);

        if (logEnabled_) logRequestBuffer_(requestBuffer);
    }
    else return;

    // When listening for a specific UnitID, only except data for that ID
    if ((unitID_ != IgnoreUnitId) &&
            (requestBuffer.header.unitId != unitID_ ||
            requestBuffer.header.protocolId != Protocol))
    {
        //! @todo Необходимо вывести сообщение об ошибке в лог
        sendError_(Exceptions::ServerFailure, requestBuffer);
    }
    else
    {
        quint8 test = requestBuffer.functionCode & (~0x80); //! @todo  0x80 перенести в константы. Это стандарт MODBUS для кодирования сообщения об ошибке
        if (test)
        {
            quint8 errorCode = requestBuffer.mbpData[0];
            doError_(test, errorCode, requestBuffer);
        }
        else
        {
            quint8 errorCode = Exceptions::Ok;
            switch (requestBuffer.functionCode)
            {
                case Functions::ReadCoils :
                {
                    quint16 regNum = getRegNum_(net2host(requestBuffer.mbpData[0]));
                    quint16 regCount = net2host(requestBuffer.mbpData[2]);
                    if ((regNum < minRegister_) || (regNum + regCount > maxRegister_))
                    {
                        sendError_(Exceptions::IllegalRegister, requestBuffer);
                    }
                    else
                    {
                        //quint8 cmdResult = readCoils_(regNum, regCount, data, requestBuffer);
                    }
                }
                case Functions::ReadDescereteInputs :
                {
                /*
                   begin
                      iRegNr := GetRegNr(Swap(Word((@ReceiveBuffer.MBPData[0])^)));
                      iCount := Swap(Word((@ReceiveBuffer.MBPData[2])^));
                      if ((iRegNr < FMinRegister) or ((iRegNr + iCount) > FMaxRegister)) then
                      {$IFDEF DMB_INDY10}
                        SendError(AContext, mbeIllegalRegister, ReceiveBuffer)
                      {$ELSE}
                        SendError(AThread, mbeIllegalRegister, ReceiveBuffer)
                      {$ENDIF}
                      else
                      begin
                      { Signal the user that data is needed }
                      {$IFDEF DMB_INDY10}
                        if (ReceiveBuffer.FunctionCode = mbfReadCoils) then
                          InternalReadCoils(AContext, iRegNr, iCount, Data, ReceiveBuffer, ErrorCode)
                        else
                          InternalReadInputBits(AContext, iRegNr, iCount, Data, ReceiveBuffer, ErrorCode);
                        if (ErrorCode = mbeOk) then
                          SendResponse(AContext, ReceiveBuffer, Data)
                        else
                          SendError(AContext, ErrorCode, ReceiveBuffer);
                      {$ELSE}
                        if (ReceiveBuffer.FunctionCode = mbfReadCoils) then
                          InternalReadCoils(AThread, iRegNr, iCount, Data, ReceiveBuffer, ErrorCode)
                        else
                          InternalReadInputBits(AThread, iRegNr, iCount, Data, ReceiveBuffer, ErrorCode);
                        if (ErrorCode = mbeOk) then
                          SendResponse(AThread, ReceiveBuffer, Data)
                        else
                          SendError(AThread, ErrorCode, ReceiveBuffer);
                      {$ENDIF}
                      end;
                    end;
                */
                }
                case Functions::ReadInputRegs :
                case Functions::ReadHoldingRegs :
                {
                /*
                    begin
                      iRegNr := GetRegNr(Swap(Word((@ReceiveBuffer.MBPData[0])^)));
                      iCount := Swap(Word((@ReceiveBuffer.MBPData[2])^));
                      if ((iRegNr < FMinRegister) or ((iRegNr + iCount) > FMaxRegister)) then
                      {$IFDEF DMB_INDY10}
                        SendError(AContext, mbeIllegalRegister, ReceiveBuffer)
                      {$ELSE}
                        SendError(AThread, mbeIllegalRegister, ReceiveBuffer)
                      {$ENDIF}
                      else
                      begin
                      { Signal the user that data is needed }
                      {$IFDEF DMB_INDY10}
                        if (ReceiveBuffer.FunctionCode = mbfReadInputRegs) then
                          DoReadInputRegisters(AContext, iRegNr, iCount, Data, ReceiveBuffer, ErrorCode)
                        else
                          DoReadHoldingRegisters(AContext, iRegNr, iCount, Data, ReceiveBuffer, ErrorCode);
                        if (ErrorCode = mbeOk) then
                          SendResponse(AContext, ReceiveBuffer, Data)
                        else
                          SendError(AContext, ErrorCode, ReceiveBuffer);
                      {$ELSE}
                        if (ReceiveBuffer.FunctionCode = mbfReadInputRegs) then
                          DoReadInputRegisters(AThread, iRegNr, iCount, Data, ReceiveBuffer, ErrorCode)
                        else
                          DoReadHoldingRegisters(AThread, iRegNr, iCount, Data, ReceiveBuffer, ErrorCode);
                        if (ErrorCode = mbeOk) then
                          SendResponse(AThread, ReceiveBuffer, Data)
                        else
                          SendError(AThread, ErrorCode, ReceiveBuffer);
                      {$ENDIF}
                      end;
                    end;
                */
                }
                case Functions::WriteOneHoldingReg :
                {
                /*
                    begin
                    { Get the register number }
                      iRegNr := GetRegNr(Swap(Word((@ReceiveBuffer.MBPData[0])^)));
                    { Get the register value }
                      Data[0] := Swap(Word((@ReceiveBuffer.MBPData[2])^));
                    { This function always writes one register }
                      iCount := 1;

                      if ((iRegNr < FMinRegister) or ((iRegNr + iCount) > FMaxRegister)) then
                      {$IFDEF DMB_INDY10}
                        SendError(AContext, mbeIllegalRegister, ReceiveBuffer)
                      {$ELSE}
                        SendError(AThread, mbeIllegalRegister, ReceiveBuffer)
                      {$ENDIF}
                      else
                      begin
                      { Send back the response to the master }
                      {$IFDEF DMB_INDY10}
                        DoWriteRegisters(AContext, iRegNr, iCount, Data, ReceiveBuffer, ErrorCode);
                        if (ErrorCode = mbeOk) then
                          SendResponse(AContext, ReceiveBuffer, Data)
                        else
                          SendError(AContext, ErrorCode, ReceiveBuffer);
                      {$ELSE}
                        DoWriteRegisters(AThread, iRegNr, iCount, Data, ReceiveBuffer, ErrorCode);
                        if (ErrorCode = mbeOk) then
                          SendResponse(AThread, ReceiveBuffer, Data)
                        else
                          SendError(AThread, ErrorCode, ReceiveBuffer);
                      {$ENDIF}
                      end;
                    end;
                */
                }
                case Functions::WriteHoldingRegs :
                {
                /*
                    begin
                      iRegNr := GetRegNr(Swap(Word((@ReceiveBuffer.MBPData[0])^)));
                      iCount := Swap(Word((@ReceiveBuffer.MBPData[2])^));
                      if ((iRegNr < FMinRegister) or ((iRegNr + iCount) > FMaxRegister)) then
                      {$IFDEF DMB_INDY10}
                        SendError(AContext, mbeIllegalRegister, ReceiveBuffer)
                      {$ELSE}
                        SendError(AThread, mbeIllegalRegister, ReceiveBuffer)
                      {$ENDIF}
                      else
                      begin
                      { Decode the contents of the Registers }
                        GetRegistersFromBuffer(@ReceiveBuffer.MbpData[5], iCount, Data);
                      { Send back the response to the master }
                      {$IFDEF DMB_INDY10}
                        DoWriteRegisters(AContext, iRegNr, iCount, Data, ReceiveBuffer, ErrorCode);
                        if (ErrorCode = mbeOk) then
                          SendResponse(AContext, ReceiveBuffer, Data)
                        else
                          SendError(AContext, ErrorCode, ReceiveBuffer);
                      {$ELSE}
                        DoWriteRegisters(AThread, iRegNr, iCount, Data, ReceiveBuffer, ErrorCode);
                        if (ErrorCode = mbeOk) then
                          SendResponse(AThread, ReceiveBuffer, Data)
                        else
                          SendError(AThread, ErrorCode, ReceiveBuffer);
                      {$ENDIF}
                      end;
                    end;
                */
                }
                case Functions::WriteCoils :
                {
                /*
                    begin
                      iRegNr := GetRegNr(Swap(Word((@ReceiveBuffer.MBPData[0])^)));
                      iCount := Swap(Word((@ReceiveBuffer.MBPData[2])^));
                      if ((iRegNr < FMinRegister) or ((iRegNr + iCount) > FMaxRegister)) then
                      {$IFDEF DMB_INDY10}
                        SendError(AContext, mbeIllegalRegister, ReceiveBuffer)
                      {$ELSE}
                        SendError(AThread, mbeIllegalRegister, ReceiveBuffer)
                      {$ENDIF}
                      else
                      begin
                      { Decode the contents of the Registers }
                        GetCoilsFromBuffer(@ReceiveBuffer.MbpData[5], iCount, Data);
                      { Send back the response to the master }
                      {$IFDEF DMB_INDY10}
                        InternalWriteCoils(AContext, iRegNr, iCount, Data, ReceiveBuffer, ErrorCode);
                        if (ErrorCode = mbeOk) then
                          SendResponse(AContext, ReceiveBuffer, Data)
                        else
                          SendError(AContext, ErrorCode, ReceiveBuffer);
                      {$ELSE}
                        InternalWriteCoils(AThread, iRegNr, iCount, Data, ReceiveBuffer, ErrorCode);
                        if (ErrorCode = mbeOk) then
                          SendResponse(AThread, ReceiveBuffer, Data)
                        else
                          SendError(AThread, ErrorCode, ReceiveBuffer);
                      {$ENDIF}
                      end;
                    end;
                */
                }
                default :
                {
                /*
                  if (ReceiveBuffer.FunctionCode <> 0) then
                  begin
                  { Illegal or unsupported function code }
                  {$IFDEF DMB_INDY10}
                    SendError(AContext, mbeIllegalFunction, ReceiveBuffer);
                    DoInvalidFunction(AContext, ReceiveBuffer.FunctionCode, ReceiveBuffer);
                  {$ELSE}
                    SendError(AThread, mbeIllegalFunction, ReceiveBuffer);
                    DoInvalidFunction(AThread, ReceiveBuffer.FunctionCode, ReceiveBuffer);
                  {$ENDIF}
                  end;
                */
                }
            }
        }
    }
/*
    { If needed: the server terminates the connection, after the request has been handled }
      if FOneShotConnection then
      {$IFDEF DMB_INDY10}
        AContext.Connection.Disconnect;
      {$ELSE}
        AThread.Connection.Disconnect;
      {$ENDIF}
*/
}

//-----------------------------------------------------------------------------

void
AbstractTcpServer::sendError_(quint8 errorCode, const RequestBuffer& recieveBuffer)
{
/*
    {$IFDEF DMB_INDY10}
    procedure TIdModBusServer.SendError(const AContext: TIdContext; const ErrorCode: Byte; const ReceiveBuffer: TModBusRequestBuffer);
    {$ELSE}
    procedure TIdModBusServer.SendError(const AThread: TIdPeerThread;
      const ErrorCode: Byte; const ReceiveBuffer: TModBusRequestBuffer);
    {$ENDIF}
    var
      SendBuffer: TModBusExceptionBuffer;
    {$IFDEF DMB_INDY10}
      Buffer: TIdBytes;
    {$ENDIF}
    begin
      if Active then
      begin
        SendBuffer.Header := ReceiveBuffer.Header;
        SendBuffer.ExceptionFunction := ReceiveBuffer.FunctionCode or $80;
        SendBuffer.ExceptionCode := ErrorCode;
        SendBuffer.Header.RecLength := Swap(3);

      {$IFDEF DMB_INDY10}
        Buffer := RawToBytes(SendBuffer, SizeOf(SendBuffer));
        AContext.Connection.Socket.WriteDirect(Buffer);
        if FLogEnabled then
          LogExceptionBuffer(AContext, SendBuffer);
      {$ELSE}
        AThread.Connection.Socket.Send(SendBuffer, SizeOf(SendBuffer));
        if FLogEnabled then
          LogExceptionBuffer(AThread, SendBuffer);
      {$ENDIF}
      end;
    end;
*/
    ExceptionBuffer sendBuffer;

    sendBuffer.header = recieveBuffer.header;
    sendBuffer.function = recieveBuffer.functionCode | 0x80;
    sendBuffer.exceptionCode = errorCode;
    sendBuffer.header.recLength = host2net(3); //! @todo Подумать. Может вычислять автоматически?

    // @todo Проверить корректность
    QByteArray buf;
    QDataStream out(&buf, QIODevice::WriteOnly);
    out.writeRawData((char*)&(sendBuffer), net2host(sendBuffer.header.recLength));

    tcpSocket_->write(buf);

    if (logEnabled_) logExceptionBuffer_(sendBuffer);
}

//-----------------------------------------------------------------------------

void
AbstractTcpServer::sendResponse_(const RequestBuffer& recieveBuffer, QVector< quint16 > &registerData)
{
/*
    {$IFDEF DMB_INDY10}
    procedure TIdModBusServer.SendResponse(const AContext: TIdContext;
      const ReceiveBuffer: TModBusRequestBuffer; const Data: TModRegisterData);
    {$ELSE}
    procedure TIdModBusServer.SendResponse(const AThread: TIdPeerThread;
      const ReceiveBuffer: TModBusRequestBuffer; const Data: TModRegisterData);
    {$ENDIF}
    var
      SendBuffer: TModBusResponseBuffer;
      L: Integer;
    {$IFDEF DMB_INDY10}
      Buffer: TIdBytes;
    {$ENDIF}
    begin
      if Active then
      begin
        FillChar(SendBuffer, SizeOf(SendBuffer), 0);
        SendBuffer.Header.TransactionID := ReceiveBuffer.Header.TransactionID;
        SendBuffer.Header.ProtocolID := ReceiveBuffer.Header.ProtocolID;
        SendBuffer.Header.UnitID := ReceiveBuffer.Header.UnitID;
        SendBuffer.FunctionCode := ReceiveBuffer.FunctionCode;
        SendBuffer.Header.RecLength := Swap(0);

        case ReceiveBuffer.FunctionCode of
          mbfReadCoils,
          mbfReadInputBits:
            begin
              L := Swap(Word((@ReceiveBuffer.MBPData[2])^));
              if (L <= MaxBlockLength) then
              begin
                SendBuffer.MBPData[0] := Byte(L shl 1);
                PutCoilsIntoBuffer(@SendBuffer.MBPData[1], L, Data);
                SendBuffer.Header.RecLength := Swap(3 + SendBuffer.MBPData[0]);
              end;
            end;
          mbfReadInputRegs,
          mbfReadHoldingRegs:
            begin
              L := Swap(Word((@ReceiveBuffer.MBPData[2])^));
              if (L <= MaxBlockLength) then
              begin
                SendBuffer.MBPData[0] := Byte(L shl 1);
                PutRegistersIntoBuffer(@SendBuffer.MBPData[1], L, Data);
                SendBuffer.Header.RecLength := Swap(3 + SendBuffer.MBPData[0]);
              end;
            end;
        else
          begin
            SendBuffer.MBPData[0] := ReceiveBuffer.MBPData[0];
            SendBuffer.MBPData[1] := ReceiveBuffer.MBPData[1];
            SendBuffer.MBPData[2] := ReceiveBuffer.MBPData[2];
            SendBuffer.MBPData[3] := ReceiveBuffer.MBPData[3];
            SendBuffer.Header.RecLength := Swap(6);
          end;
        end;
      {$IFDEF DMB_INDY10}
        Buffer := RawToBytes(SendBuffer, Swap(SendBuffer.Header.RecLength) + 6);
        AContext.Connection.Socket.WriteDirect(Buffer);
        if FLogEnabled then
          LogResponseBuffer(AContext, SendBuffer, Swap(SendBuffer.Header.RecLength) + 6);
      {$ELSE}
        AThread.Connection.Socket.Send(SendBuffer, Swap(SendBuffer.Header.RecLength) + 6);
        if FLogEnabled then
          LogResponseBuffer(AThread, SendBuffer, Swap(SendBuffer.Header.RecLength) + 6);
      {$ENDIF}
      end;
    end;
*/

/*
    ResponseBuffer sendBuffer;
    int len = 0;

    sendBuffer.header.transactionId = recieveBuffer.header.transactionId;
    sendBuffer.header.protocolId = recieveBuffer.header.protocolId;
    sendBuffer.header.unitId = recieveBuffer.header.unitId;
    sendBuffer.functionCode = recieveBuffer.functionCode;
    sendBuffer.header.recLength = host2net(0);

    switch (recieveBuffer.functionCode)
    {
        case Functions::ReadCoils :
        case Functions::ReadInputBits :
        {
            len = host2net(*(static_cast<const quint16*>(recieveBuffer.mbpData[2]))); //! @todo Проверить правильность индекса элемента массива
            if (len <= MaxBlockLength)
            {
                sendBuffer.data[0] = lo(len);
                putCoilsIntoBuffer(sendBuffer.data[1], len, registerData);
                sendBuffer.header.recLength = host2net(3 + sendBuffer.data[0]); //! @todo Проверить правильность обращения к ячейке памяти
            }
        }
        case Functions::ReadInputBits :
        case Functions::ReadHoldingRegs :
        {
            len = host2net(*(static_cast<quint16*>recieveBuffer.mbpData[2])); //! @todo Проверить правильность индекса элемента массива
            if (len <= MaxBlockLength)
            {
                sendBuffer.data[0] = lo(len);
                putRegistersIntoBuffer(sendBuffer.data[1], len, registerData);
                sendBuffer.header.recLength = host2net(3 + sendBuffer.data[0]); //! @todo Проверить правильность обращения к ячейке памяти
            }

        }
        default :
        {
            sendBuffer.data[0] = recieveBuffer.mbpData[0];
            sendBuffer.data[1] = recieveBuffer.mbpData[1];
            sendBuffer.data[2] = recieveBuffer.mbpData[2];
            sendBuffer.data[3] = recieveBuffer.mbpData[3];
            sendBuffer.header.recLength = host2net(6);
        }
    }

    // @todo Проверить корректность
    QByteArray buf;
    QDataStream out(&buf, QIODevice::WriteOnly);
    out.writeRawData((char*)&(sendBuffer), net2host(sendBuffer.header.recLength));

    //! @todo Отослать данные: tcpSocket_.write(buf);

    if (logEnabled_) logResponseBuffer_(sendBuffer);
*/
}

//-----------------------------------------------------------------------------

void
AbstractTcpServer::setVersion_(const QString& value)
{
/*
    procedure TIdModBusServer.SetVersion(const Value: String);
    begin
    { This intentionally is a readonly property }
    end;
    */
}

} // namespace modbus
