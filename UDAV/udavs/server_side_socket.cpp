#include <QTimer>

// Объявления подсистемы ядра
#include "core.h"

#include "server.h"

#ifdef UDAV_COMBO
    #include "udavc.h"
#else
    #include "server_application.h"
#endif

#include "server_side_socket.h"
#include "elvactools/block_stream.h"
#include "server_settings.h"

// Объявления подсистемы драйверов оборудования
#include "driver_interface.h"
#include "driver_impl.h"
#include "meta_signal.h"

namespace Core
{

//-----------------------------------------------------------------------------

ServerSideSocket::ServerSideSocket(Server* parent) :
    QTcpSocket(parent),
    allowedReplies_(),
    connectionCheckTimer_(0),
    isConnected_(false),
    dataSendTimer_(0),
    driverImpl_(0),
    lastRequest_(PacketType::Empty),
    lostPackets_(0),
    nextBlockSize_(0),
    server_(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(readDataFromClient_()));
    connect(this, SIGNAL(disconnected()), this, SLOT(socketDisconnected_()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));

    allowedReplies_ << PacketType::RequestConnection;
}

//-----------------------------------------------------------------------------

ServerSideSocket::~ServerSideSocket()
{
}

//-----------------------------------------------------------------------------

bool
ServerSideSocket::checkHandShake_(const QString& clientName, const QString& clientVersion, QString& message)
{
    bool retVal = true;

    emit infoMessage(tr("Checking handshake recieved from client... "));
    if (clientName != ClientName)
    {
        message = tr("Incorrect client name");
        retVal = false;
    }
    if (clientVersion != RevisionNumber)
    {
        message = tr("Incorrect client version");
        retVal = false;
    }

    if (retVal)
        emit infoMessage(tr("Handshake OK!"));
    else
        emit criticalMessage(tr("FAILED: %1").arg(message));

    return retVal;
}

//-----------------------------------------------------------------------------

void
ServerSideSocket::readDataFromClient_()
{
    QDataStream in(this);
    in.setVersion(StreamVersion);

    forever
    {
        if (nextBlockSize_ == 0)
        {
            if (bytesAvailable() < int(sizeof(qint16))) // Количество данных для чтения должно быть хотя бы 2 байта
                return;

            in >> nextBlockSize_;

            emit debugMessage(tr("Incoming block size: %1  byte(s)").arg(nextBlockSize_));
        }

        if (bytesAvailable() < nextBlockSize_)
        {
            QString message;
            QTextStream messageStream(&message);

            messageStream << "Data error! Data size less than expected,"
                          << "Expected: " << nextBlockSize_
                          << "; recieved: " << bytesAvailable() << ::endl;

            emit debugMessage(message);

            return;
        }

        // Пакет от клиента означает наличие связи
        // Счетчик потерянных пакетов обнуляем
        // Если таймеры проверки связи запущены, перезапускаем их

        lostPackets_ = 0;
        if (connectionCheckTimer_)
        {
            connectionCheckTimer_->stop();
            connectionCheckTimer_->start();
        }

        quint8 requestType = PacketType::Empty;
        in >> requestType;

        // Клиент в любой момент может запросить разрыв соединения

        if (requestType == PacketType::RefuseConnection)
        {
            QString reason;
            in >> reason;

            emit infoMessage(tr("Client refused connection!"));
            emit infoMessage(tr("Reason: %1").arg(reason));

            close();

            return;
        }

        if (!isConnected_)
        {
            // Режим установки соединения. Здесь имеет значение порядок обмена пакетами

            if (!allowedReplies_.contains(requestType))
            {
                QString message = tr("Wrong request recieved from client!");
                emit criticalMessage(message);
                emit criticalMessage(tr("Request is: %1 (%2)").arg(requestType).arg(packetNames[requestType]));

                emit criticalMessage(tr("Expected: "));
                quint8 curItem;
                foreach(curItem, allowedReplies_)
                    emit criticalMessage(tr("\t%1 (%2)").arg(curItem).arg(packetNames[curItem]));

                sendRefuseConnection_(message);
            }

            switch (requestType)
            {
                // Метки расположены по порядку обмена пакетами согласно спецификации протокола

                case PacketType::RequestConnection : // Клиент запросил подключение
                {
                    emit infoMessage(tr("Client address: %1").arg(peerAddress().toString()));
                    sendInitConnection_();
                } break;

                case PacketType::HandShake : // Клиент прислал "рукопожатие"
                {
                    QString prgName, prgVersion;
                    in >> prgName >> prgVersion;

                    QString message;
                    if (!checkHandShake_(prgName, prgVersion, message))
                        sendRefuseConnection_(message);
                    else
                        sendHandShake_();
                } break;

                case PacketType::ConnectionOK : // Клиент сообщил о возможности подключения
                {
                    emit infoMessage(tr("Client said: Connection OK!"));
                    if (lastRequest_ == PacketType::HandShake)
                    {
                        // Теперь мы должны прочитать тип подключения... И выбрать, как же вести себя дальше...
                        // Заполняем список допустимых значений типами подключения

                        allowedReplies_.clear();
                        allowedReplies_
                                << ConnectionType::WithoutTicket
                                << ConnectionType::DriversListRequest;
                    }
                }
                break;

                case ConnectionType::WithoutTicket : // Клиент запросил подключение без билета
                case ConnectionType::DriversListRequest : // Клиент запросил список драйверов
                    sendDriversList_();
                break;

                case PacketType::SelectedDriver : // Клиент сообщил о выбранном драйвере для подключения
                {
                    QUuid selDrvUuid;
                    in >> selDrvUuid;
                    emit infoMessage(tr("Client selected driver with uuid: %1") .arg(selDrvUuid.toString()));
                    //! @todo Сообщать имя драйвера, а не только uuid #107

                    driverImpl_ = server_->getDriver(selDrvUuid);
                    if (!driverImpl_)
                    {
                        QString message = tr("Cannot create driver implementation!");

                        emit criticalMessage(message);
                        sendRefuseConnection_(message);
                    }
                    else
                    {
                        driverImpl_->setParent(this);

                        //-------------------------------------------------
                        // Сигналы для передачи соообщений пользователю (MessageWindow)

                        connect(driverImpl_, SIGNAL(openMessageWindow(QUuid, QString)), this, SLOT(sendOpenMessageWindow_(QUuid, QString)));
                        connect(driverImpl_, SIGNAL(closeMessageWindow(QUuid,quint16,bool)), this, SLOT(sendCloseMessageWindow_(QUuid,quint16,bool)));
                        connect(driverImpl_, SIGNAL(sendTextMessage2Window(QUuid,quint16,QString)), this, SLOT(sendTextMessage2Window_(QUuid,quint16,QString)));

                        bool ok = driverImpl_->initEquipment();

                        if (!ok)
                        {
                            emit criticalMessage(tr("Cannot init equipment!"));

                            delete driverImpl_;
                            driverImpl_ = NULL;
                        }
                        else
                        {
                            //! @todo Может это должно быть не здесь?
                            driverImpl_->initDevices();
                            driverImpl_->initProcesses();
                            driverImpl_->initSignalValues();
                            // Оборудование успешно инициализировано. Можно связать сигналы-слоты для управления и контроля

                            //-------------------------------------------------
                            // Сигналы от драйвера оборудования к клиенту (BOF)

                            //connect(driver, SIGNAL(controlSignalSetted(Drivers::SignalID,qint16)), this, SLOT(sendIntSignalValue(Drivers::SignalID,qint16)));
                            //$ connect(driver, SIGNAL(controlSignalSetted(Drivers::SignalID,qint16,QString)), this, SLOT(sendIntSignalValue(Drivers::SignalID,qint16,QString)));
                            //$ connect(driver, SIGNAL(controlSignalSetted(Drivers::SignalID,bool,QString)), this, SLOT(sendBoolSignalValue(Drivers::SignalID,bool,QString)));

                            //$ connect(driver, SIGNAL(equipmentResetted()), this, SLOT(sendViewSignals()));

                            // Драйвер прочитал массив сигналов наблюдения
                            connect(driverImpl_, SIGNAL(viewSignalsReaded()), this, SLOT(sendViewSignals_()));
                            connect(driverImpl_, SIGNAL(controlSignalsReaded()), this, SLOT(sendControlSignals_()));

                            //$ connect(driver, SIGNAL(sendMessage(Drivers::SignalID,QString)), this, SLOT(sendTextMessage(Drivers::SignalID,QString)));

                            // На оборудовании изменилось значение сигнала
                            connect(driverImpl_, SIGNAL(signalValueChanged(Drivers::SignalID,QString)), this, SLOT(sendSignalValue_(Drivers::SignalID,QString)));

                            // Сигналы от драйвера оборудования к клиенту (EOF)
                            //-------------------------------------------------

                            // Связываем сигналы и слоты (EOF)
                            //----------------------------------

                            sendMetaSignals_();
                            sendUiDescription_();
                            sendDriverSettings_();

                            // После успешной инициализации запускаем таймер регулярного получения значений сигналов с датчиков оборудования

                            driverImpl_->start();
                        }
                    }
                }
                break;

                case PacketType::ConnectionEstablsihed : // Клиент сообщил, что подключение установлено
                {
                    emit infoMessage(tr("Client said: Connection Established!"));

                    // Соденинение установлено. Ожидаем от клиента управляющих данных,
                    // а ему отправляем через заданный интервал данные с датчиков драйвера
                    // и запросы на подтверждение наличия связи.

#ifdef UDAV_COMBO
                    if (serverSettings->connectCheckingIterval())
                    {
                        emit infoMessage(tr("Connection will be checked every %1 sec.").arg(serverSettings->connectCheckingIterval()));
#else
                    if (serverApp->settings()->connectCheckingIterval())
                    {
                        emit infoMessage(tr("Connection will be checked every %1 sec.").arg(serverApp->settings()->connectCheckingIterval()));
#endif
                        connectionCheckTimer_ = new QTimer(this);

#ifdef UDAV_COMBO
                        connectionCheckTimer_->setInterval(serverSettings->connectCheckingIterval() * 1000);
#else
                        connectionCheckTimer_->setInterval(serverApp->settings()->connectCheckingIterval() * 1000);
#endif
                        connect(connectionCheckTimer_, SIGNAL(timeout()), this, SLOT(sendCheckConnection_()));

                        connectionCheckTimer_->start();
                    }

                    // Отправляем клиенту начальные значения сигналов набдюдения и управления,
                    // а затем переходим в режим регулярного обмена данными, снимаемыми с датчиков

                    sendViewSignals_();
                    sendControlSignals_();

#ifdef UDAV_COMBO
                    if (serverSettings->sendDataInterval())
                    {
                        emit infoMessage(tr("Data will be sent to client every %1 sec.").arg(serverSettings->sendDataInterval()));
#else
                    if (serverApp->settings()->sendDataInterval())
                    {
                        emit infoMessage(tr("Data will be sent to client every %1 sec.").arg(serverApp->settings()->sendDataInterval()));
#endif
                        dataSendTimer_ = new QTimer(this);

#ifdef UDAV_COMBO
                        dataSendTimer_->setInterval(serverSettings->sendDataInterval() * 1000);
#else
                        dataSendTimer_->setInterval(serverApp->settings()->sendDataInterval() * 1000);
#endif
                        //connect(dataSendTimer_, SIGNAL(timeout()), this, SLOT(sendSignals_()));

                        //! @todo Нужно ли передавать клиенту данные сигналов управления? Возможно, достаточно обойтись данными сигналов наблюдения и иметь контрольные сигналы обратной связи
                        //connect(dataSendTimer_, SIGNAL(timeout()), this, SLOT(sendControlSignals_()));

                        dataSendTimer_->start();
                    }
                    else
                    {
                        emit infoMessage(tr("Connection established. No data sending inerval setted. Entering in manual data exchange mode"));
                    }

                    emit infoMessage(tr("Connection established. Regular data exchange started"));

                    isConnected_ = true;

                    // В режиме установленного соединения список разрешенных пакетов не используется
                    allowedReplies_.clear();
                } break;
            }
        }
        else
        {
            // Режим работы при установленном соединении
            // Теперь работа ведется в терминах запрос-ответ
            // Клиент присылает запрос, сервер формирует ответ

            // Сервер может послать без этого только значения сигналов

            /*
             * В режиме установленного соединения клиент может прислать:
             * - запрос на передачу файла
             * - настройки драйвера
             * - управляющий сигнал
             * - подтверждение наличия связи
             * - запрос на перезагрузку интерфейса пользователя
            */

            switch (requestType)
            {
                case PacketType::ConnectionOK :
                {
                    emit infoMessage(tr("Client said: Connection OK!"));
                } break;

                case PacketType::FileRequest : // Запрос на передачу файла. Используется для передачи файлов SVG в мнемосхемах
                {
                    QString fname;
                    in >> fname;
                    emit infoMessage(tr("Client requested file: %1").arg(fname));

                    sendFile_(fname);
                } break;

                case PacketType::ReloadInterface :
                {
                    emit infoMessage(tr("Client requested user interface reload"));
                    sendUiDescription_();
                } break;

                case PacketType::DriverSettings : // Клиент прислал настройки драйвера оборудования
                {
                    emit infoMessage(tr("Client sent new driver settings..."));

                    QMap<QString, QVariant> s;
                    in >> s;

                    //$ driver->setDriverSettings(s);

                    emit infoMessage(tr("New settings have been applied..."));
                } break;

                case PacketType::Control : // Клиент прислал управляющий сигнал
                {
                    emit infoMessage(tr("Recieved a control signal!"));

                    Drivers::SignalID signalId;
                    QVariant value;
                    in >> signalId >> value;
                    emit infoMessage(tr("SignalID: %1").arg(signalId));

                    if (!driverImpl_->metaSignals().contains(signalId))
                    {
                        sendControlError_(0, tr("Unknown signal ID!"));
                        return;
                    }

                    Drivers::MetaSignal::ValueType  valueType(driverImpl_->metaSignals()[signalId].valueType);

                    emit infoMessage(tr("Signal type: %2").arg(Drivers::MetaSignal::valueType2String(valueType)));

                    if (valueType == Drivers::MetaSignal::Boolean)
                    {
                        bool boolValue = value.toBool();

                        emit infoMessage(tr("Signal ID: %1; Value: %2").arg(signalId).arg(boolValue));

                        int result = driverImpl_->setControlSignalBoolean(signalId, boolValue);

                        if (result == Drivers::CommandOk)
                        {
                            sendControlOk_();
                        }
                        else
                        {
                            QString errorMessage = driverImpl_->errorString();
                            sendControlError_(signalId, errorMessage);
                        }
                    }
                    else if (valueType == Drivers::MetaSignal::Real)
                    {
                        double doubleValue = value.toDouble();

                        emit infoMessage(tr("Signal ID: %1; Value: %2").arg(signalId).arg(doubleValue));

                        int result = driverImpl_->setControlSignalDouble(signalId, doubleValue);

                        if (result == Drivers::CommandOk)
                        {
                            sendControlOk_();
                        }
                        else
                        {
                            QString errorMessage = driverImpl_->errorString();
                            sendControlError_(signalId, errorMessage);
                        }
                    }
                    else if (valueType == Drivers::MetaSignal::Integer)
                    {
                        int intValue = value.toInt();

                        emit infoMessage(tr("Signal ID: %1; Value: %2").arg(signalId).arg(intValue));

                        int result = driverImpl_->setControlSignalInteger(signalId, intValue);
                        if (result == Drivers::CommandOk)
                        {
                            sendControlOk_();
                        }
                        else
                        {
                            QString errorMessage = driverImpl_->errorString();
                            sendControlError_(signalId, errorMessage);
                        }
                    }
                } break;

                default:
                {
                    QString message = tr("Wrong request is recieved from client in the mode of established connection!");

                    emit criticalMessage(message);
                    emit criticalMessage(tr("Request is: %1 (%2)").arg(requestType).arg(packetNames[requestType]));

                    sendRefuseConnection_(message);
                }
            }
        }

        nextBlockSize_ = 0;
    }
}


//-----------------------------------------------------------------------------

void
ServerSideSocket::sendBoolSignalValue(Drivers::SignalID signalId, bool signalValue, QString message)
{
    emit infoMessage(tr("Sending bool signal value to client..."));

    Q_ASSERT(false);

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::SignalValue;
    out << PacketType::SignalValue;
    out << signalId << signalValue << message << endp;

    write(block);
}

//-----------------------------------------------------------------------------

void
ServerSideSocket::sendCheckConnection_()
{
    emit debugMessage(tr("Sending connection check packet to client..."));

#ifdef UDAV_COMBO
    if (lostPackets_ >= serverSettings->connectLostAfterAttempts())
#else
    if (lostPackets_ >= serverApp->settings()->connectLostAfterAttempts())
#endif
    {
        emit criticalMessage(tr("Connection with client has been lost!"));
        emit criticalMessage(tr("%1 checking packet(s) has been send with no reply!").arg(lostPackets_));
        emit criticalMessage(tr("Closing connection!"));

        close();

        return;
    }

    // Еще есть шанс, что все наладится...
    // Отправляем клиенту запрос на подтверждение подключения

    lostPackets_++;

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::CheckConnection;
    out << PacketType::CheckConnection << endp;

    write(block);
}


//-----------------------------------------------------------------------------

void
ServerSideSocket::sendControlError_(Drivers::SignalID signalID, const QString& message)
{
    emit infoMessage(tr("Sending 'Control error' to client..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::CommandError;

    QVariant value = QVariant();
    if (!signalID) value = driverImpl_->controlValues()[signalID];

    out << PacketType::CommandError
        << signalID << value << message
        << endp;

    write(block);
}

//-----------------------------------------------------------------------------

void
ServerSideSocket::sendControlOk_()
{
    emit infoMessage(tr("Sending 'Control OK' to client..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::CommandOk;

    out << PacketType::CommandOk << endp;

    write(block);
}

//-----------------------------------------------------------------------------

void
ServerSideSocket::sendControlSignals_()
{
    emit infoMessage(tr("Sending control signals packet to client..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::ControlArray;
    out << PacketType::ControlArray << driverImpl_->controlValues() << endp;

    write(block);
}

//-----------------------------------------------------------------------------

void
ServerSideSocket::sendDriverSettings_()
{
    emit infoMessage(tr("Sending driver settings to client..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::DriverSettings;
    out << PacketType::DriverSettings;

    //QMap<QString, QVariant> s = driver->getDriverSettings();

    out << /*s <<*/ endp;

    allowedReplies_.clear();
    allowedReplies_
            << PacketType::ConnectionEstablsihed
            << PacketType::RefuseConnection
            << PacketType::Control;

    write(block);

    //logger << tr("Total ") << s.count() << tr(" item(s) sended") << endl;
}

//-----------------------------------------------------------------------------

void
ServerSideSocket::sendDriversList_()
{
    emit infoMessage(tr("Sending drivers list to client..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::DriversList;
    out << PacketType::DriversList;

    const Drivers::EquipmentArray& driversArray = server_->getDriversArray();

    out << driversArray.count();

    emit infoMessage(tr("Total %1 driver(s) loaded").arg(driversArray.count()));

    int count = 0;
    foreach(Drivers::DriverInterface* item, driversArray)
    {
        item->save2stream(out);
        count++;
    }
    out << endp;

    write(block);

    emit infoMessage(tr("Total %1 driver's info(s) sended").arg(count));
    emit infoMessage(tr("Waiting for selection..."));

    allowedReplies_.clear();
    allowedReplies_
            << PacketType::SelectedDriver
            << PacketType::RefuseConnection;
}

//-----------------------------------------------------------------------------

void
ServerSideSocket::sendFile_(const QString& fname)
{
    QByteArray block;
    BlockStream out(&block);

    // Читаем файл и записываем его в поток
    QByteArray buffer;

#ifdef QT_NO_DEBUG
    QFile f(QString(":") + fname);
#else
    QFile f(fname);
#endif

    if (f.open(QFile::ReadOnly))
    {
        char c;

        QDataStream s(&f);
        s.setVersion(StreamVersion);

        while (!s.atEnd())
        {
            s.readRawData(&c, 1);
            buffer.append(c);
        }
    }
    else
    {
        emit criticalMessage(tr("Cannot read file %1").arg(fname));
        //! @todo Если файла нет, то программа встает!
    }
    emit infoMessage(tr("Sending %1. File size: %2 byte(s)").arg(fname).arg(buffer.size()));

    lastRequest_ = PacketType::File;
    out << PacketType::File << fname << buffer << endp;

    write(block);

    allowedReplies_.clear();
    allowedReplies_
            << PacketType::ConnectionOK
            << PacketType::Control
            << PacketType::RefuseConnection;

}

//-----------------------------------------------------------------------------

void
ServerSideSocket::sendHandShake_()
{
    emit infoMessage(tr("Sending handshake packet to client..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::HandShake;
    out << PacketType::HandShake;
    out << ServerName << RevisionNumber << endp;

    write(block);

    allowedReplies_.clear();
    allowedReplies_ << PacketType::ConnectionOK << PacketType::RefuseConnection;

}

//-----------------------------------------------------------------------------

void
ServerSideSocket::sendInitConnection_()
{
    QString uuid = server_->getSessionUuid(this).toString();

    emit infoMessage(tr("Sending status 'Connection OK' and session uuid to client"));

    QByteArray block;
    BlockStream out(&block);
    out << PacketType::ConnectionOK << uuid << endp;

    allowedReplies_.clear();
    allowedReplies_ << PacketType::HandShake;

    write(block);
}

//-----------------------------------------------------------------------------

void
ServerSideSocket::sendMetaSignals_()
{
    emit infoMessage(tr("Sending meta signals to client..."));

    const Drivers::MetaSignals& metaSignals = driverImpl_->metaSignals();

    emit infoMessage(tr("Driver provides %1 signal(s)").arg(metaSignals.count()));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::MetaSignals;
    out << PacketType::MetaSignals << metaSignals << endp;

    write(block);

    emit infoMessage(tr("Total %1 metasignal(s) sended").arg(metaSignals.count()));

    allowedReplies_.clear();
    allowedReplies_
            << PacketType::ConnectionOK
            << PacketType::RefuseConnection;

}

//-----------------------------------------------------------------------------

void
ServerSideSocket::sendOpenMessageWindow_(const QUuid& uuid, const QString& title)
{
    emit infoMessage(tr("Sending command to open message window \'%1\' with id=%2").arg(title).arg(uuid.toString()));

    QByteArray block;
    BlockStream out(&block);

    out << PacketType::OpenMessageWindow;
    out << uuid << title << endp;

    write(block);
    flush();
}

//-----------------------------------------------------------------------------

void
ServerSideSocket::sendCloseMessageWindow_(const QUuid &uuid, quint16 delay, bool autoClose)
{
    emit infoMessage(tr("Sending close message window command with id=%1").arg(uuid.toString()));

    QByteArray block;
    BlockStream out(&block);

    out << PacketType::CloseMessageWindow;
    out << uuid << delay << autoClose << endp;

    write(block);
    flush();
}

//-----------------------------------------------------------------------------

void
ServerSideSocket::sendTextMessage2Window_(const QUuid &uuid, quint16 type, const QString &message)
{
    emit infoMessage(tr("Sending message (type %3) \"%1\" to window with id=%2").arg(message).arg(uuid.toString()).arg(type));

    QByteArray block;
    BlockStream out(&block);

    out << PacketType::TextMessage2Window;
    out << uuid << type << message << endp;

    write(block);
    flush();
}

//-----------------------------------------------------------------------------

void
ServerSideSocket::sendRefuseConnection_(const QString& reason)
{
    emit infoMessage(tr("Refusing connection..."));
    emit infoMessage(tr("Reason: %1").arg(reason));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::RefuseConnection;
    out << PacketType::RefuseConnection;
    out << reason << endp;

    write(block);

    close();
}


//-----------------------------------------------------------------------------

void
ServerSideSocket::sendSignalValue_(Drivers::SignalID signalID, const QString& message)
{
    emit infoMessage(tr("Sending signal value to client..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::SignalValue;
    out << PacketType::SignalValue;


    QVariant value;
    if (signalID < Drivers::VS_Begin)
        // Сигнал управления
        value = driverImpl_->controlValues()[signalID];
    else
        // Сигнал наблюдения
        value = driverImpl_->viewValues()[signalID];

    out
            << signalID
            << value
            << message
            << endp;

    write(block);
}


//-----------------------------------------------------------------------------

//void
//ServerSideSocket::sendTextMessage_(Drivers::SignalID channel, QString message)
//{
//    emit infoMessage(tr("Sending text message to client at channel #%1").arg(channel));

//    QByteArray block;
//    BlockStream out(&block);

//    lastRequest_ = PacketType::TextMessage;
//    out << PacketType::TextMessage;
//    out << channel << message << endp;

//    write(block);
//}

//-----------------------------------------------------------------------------

void
ServerSideSocket::sendUiDescription_()
{
    emit infoMessage(tr("Sending ui description to client..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::UiDescription;
    out << PacketType::UiDescription;

    out << driverImpl_->userInterface() << endp;

    write(block);

    allowedReplies_.clear();
    allowedReplies_ << PacketType::ConnectionOK << PacketType::RefuseConnection;

}


//-----------------------------------------------------------------------------

void
ServerSideSocket::sendViewSignals_()
{
    emit debugMessage(tr("Sending view signals to client..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::ViewArray;
    out << PacketType::ViewArray << driverImpl_->viewValues();

    out << endp;

    write(block);
}

void
ServerSideSocket::sendSignals_()
{
    //sendViewSignals_();
    sendControlSignals_();
}

}
