#include <QHostAddress>
#include <QByteArray>
#include <QDataStream>
#include <QList>
#include <QMessageBox>

#include "core.h"
#include "udavc.h"
#include "client.h"
#include "elvactools/utils.h"

#include "elvactools/block_stream.h"

#include "gui/client_select_driver_dialog.h"
//#include "ui/equipment_control_window.h"
//#include "ui/equipment_visualisation_window.h"

namespace Core
{

Client::Client(QObject *parent) :
    QObject(parent),
    allowedReplies_(),
        connectionMode_(ConnectionType::WithoutTicket),
        connectionState_({false, QString(), QString(), QDateTime()}),
        connectionUuid_(),
        controlValues_(),
        driverSettings_(),
        lastRequest_(0),
        metaSignals_(),
        nextBlockSize_(0),
        protocolFileName_(),
        protocolTimer_(),
        tcpSocket_(this),
        viewValues_()
{
    connect(&tcpSocket_, SIGNAL(connected()), this, SLOT(requestConnection_()));
    connect(&tcpSocket_, SIGNAL(disconnected()), this, SLOT(connectionClosed_()));
    connect(&tcpSocket_, SIGNAL(readyRead()), this, SLOT(readDataFromServer_()));
    connect(&tcpSocket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error_()));
    connect(&protocolTimer_, SIGNAL(timeout()), this, SLOT(writeProtocolString_()));
}

//-----------------------------------------------------------------------------

bool
Client::checkHandShake_(const QString& serverName, const QString& serverVersion, QString& message)
{
    bool retVal = true;
    emit infoMessage(tr("Checking handshake packet recieved from server... "));
    if (serverName != ServerName)
    {
        message = tr("Incorrect server name");
        retVal = false;
    }
    if (serverVersion != RevisionNumber)
    {
        message = tr("Incorrect server version");
        retVal = false;
    }

    if (retVal)
        emit infoMessage(tr("Handshake OK!"));
    else
    {
        emit criticalMessage(tr("Handshake FAILED! %1").arg(message));
    }

    return retVal;
}


//-----------------------------------------------------------------------------

void
Client::closeConnection()
{
    protocolTimer_.stop();

    emit infoMessage(tr("Closing connection to server..."));

    tcpSocket_.close();
}

//-----------------------------------------------------------------------------

void
Client::connectionClosed_()
{
    if (nextBlockSize_ != 0xFFFF)
    {
        emit infoMessage(tr("Connection closed!"));
    }

    emit connectionClosed();
}

//-----------------------------------------------------------------------------

void
Client::connectToServer()
{
    QHostAddress serverAddress = clientSettings->serverAddress();

    emit infoMessage(tr("Trying to connect to server %1").arg(serverAddress.toString()));

    tcpSocket_.connectToHost(serverAddress, clientSettings->serverPort());
    nextBlockSize_ = 0;
}

//-----------------------------------------------------------------------------

void
Client::controlSignalChanged(Drivers::SignalID signalID, const QVariant& value)
{
    emit infoMessage(tr("Client processed signal from UI! Sending control data to server..."));

    Drivers::MetaSignal::ValueType valueType = metaSignals_[signalID].valueType;
    emit infoMessage(tr("Signal ID: %1, value: %2").arg(signalID).arg(value.toString()));
    emit infoMessage(tr("Signal type: %2").arg(Drivers::MetaSignal::valueType2String(valueType)));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::Control;
    out
            << PacketType::Control
            << signalID
            << value
            << endp;

    allowedReplies_.clear();

    allowedReplies_
            << PacketType::ViewArray
            << PacketType::SignalValue
            << PacketType::CommandOk
            << PacketType::CommandError
            << PacketType::RefuseConnection;

    tcpSocket_.write(block);
}

//-----------------------------------------------------------------------------

void
Client::createNewProtocol()
{
    // Задаем имя файла протокола и создаем его
    protocolFileName_ = QString("%1-%2.protocol").
                        arg(connectionState_.driverName).
                        arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"));

    QString fileName = checkBackSlash(clientSettings->protocolPath()) + protocolFileName_;

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly))
    {
        emit criticalMessage(tr("Cannot create protocol file %1!").arg(fileName));
    }
    else
    {
        QTextStream out(&file);
        out << tr("# Protocol started at: %1").arg(connectionState_.conTime.toString("yyyy-MM-dd-hh-mm-ss")) << "\n";
        out << tr("# Delimeter is: %1").arg(";") << "\n";

        out << "# ";
        foreach(Drivers::MetaSignal item, metaSignals_)
        {
            out << item.fieldName << "; ";
        }
        out << "\n";

        file.close();

        emit infoMessage(tr("New protocol %1 created").arg(fileName));
    }
    protocolTimer_.start(clientSettings->protocolTimer() * 1000);
}

//-----------------------------------------------------------------------------

void
Client::error_()
{
    QString message = tcpSocket_.errorString();

    QMessageBox::critical(0, tr("Error"), message);

    emit criticalMessage(message);
    closeConnection();
}

//-----------------------------------------------------------------------------

void
Client::readDataFromServer_()
{
    QDataStream in(&tcpSocket_);
    in.setVersion(StreamVersion);

    forever
    {
        if (!nextBlockSize_) // Читаем размер следующего блока данных.
        {
            if (tcpSocket_.bytesAvailable() < int(sizeof(quint32)))
                break;

            in >> nextBlockSize_;

            emit debugMessage(tr("Incoming block size: %1 byte(s)").arg(nextBlockSize_));
        }

        if (!nextBlockSize_)
        {
            QString message = tr("Empty data block!");

            emit criticalMessage(message);
            sendRefuseConnection_(message);

            return;
        }

        qint64 bytesAvailable = tcpSocket_.bytesAvailable();

        if (bytesAvailable < nextBlockSize_)
        {
            QString message;
            QTextStream messageStream(&message);

            messageStream << "Data error! Data size less than expected,"
                          << "Expected: " << nextBlockSize_
                          << "; available: " << bytesAvailable;

            emit debugMessage(message);

            break;
        }

        quint8 reply = PacketType::Empty;
        in >> reply;

        emit debugMessage("Processing packet recieved from server...");
        emit debugMessage(tr("Recieved packet: %1 (%2)").arg(reply).arg(packetNames[reply]));
        emit debugMessage(tr("Last sended packet: %1 (%2)").arg(lastRequest_).arg(packetNames[lastRequest_]));

        // Есть пакеты, которые могут приходить вне зависимости от режима установки соединения
        // К таким пакетам относятся:
        //  OpenMessageWindow - сервер дал команду открыть окно вывода сообщений, требующих внимания пользователя
        //  CloseMessageWindow - сервер дал команду закрыть окно вывода сообщений, требующих внимания пользователя
        //  TextMessage2Window - сервер прислал сообщение, требующее внимания пользователя
        //  RefuseConnection - сервер разорвал соединение
        // Эти пакеты обрабатываются вне очереди, чтобы не нарушать логику работы

        if (reply == PacketType::RefuseConnection)
        {
            QString reason;
            in >> reason;

            emit infoMessage(tr("Connection refused. Reason: %1").arg(reason));
        }
        else if (reply == PacketType::OpenMessageWindow)
        {
            QUuid uuid;
            QString title;
            in >> uuid >> title;
            emit debugMessage(tr("Widnow id=%1, window title is \'%2\'").arg(uuid.toString()).arg(title));
            emit openMessageWindow(uuid, title);
        }
        else if (reply == PacketType::TextMessage2Window)
        {
            QUuid uuid;
            QString message;
            quint16 type;
            in >> uuid >> type >> message;
            emit debugMessage(tr("Message (type %3) \'%1\' for window with id=%2").arg(message).arg(uuid.toString()).arg(type));
            emit writeTextMessage2Window(uuid, type, message);
        }
        else if (reply == PacketType::CloseMessageWindow)
        {
            QUuid uuid;
            qint16 delay;
            bool autoClose;
            in >> uuid >> delay >> autoClose;
            emit debugMessage(tr("Window id=%1").arg(uuid.toString()));
            emit closeMessageWindow(uuid, delay, autoClose);
        }
        else if (!allowedReplies_.contains(reply))
        {
            // На стороне клиента всегда надо проверять корректность полученного пакета данных
            // Во время установки соединения это важно для правильности отработки алгоритма подключения
            // В режиме установленного соединения неверные ответы говорят об ошибках на стороне сервера

            emit infoMessage(tr("Wrong reply recieved"));
            emit infoMessage(tr("Reply is: %1 (%2)").arg(reply).arg(packetNames[reply]));
            emit infoMessage(tr("Expected: "));

            foreach(quint8 curItem, allowedReplies_)
                emit infoMessage(tr("\t%1 (%2);").arg(curItem).arg(packetNames[curItem]));
        }

        if (!connectionState_.isConnected)
        {
            // Режим установки соединения. Здесь имеет значение порядок обмена пакетами
            // Метки расположены по порядку обмена пакетами согласно спецификации протокола

            switch (reply)
            {
                // Подтверждение возможности подключения (только во время установки соединения)
                case PacketType::ConnectionOK :
                {
                    if (lastRequest_ == PacketType::RequestConnection)
                    {
                        emit infoMessage(tr("Connection OK!"));

                        QString uuidString;
                        in >> uuidString;
                        emit infoMessage(tr("Connection uid: %1").arg(uuidString));
                        connectionUuid_ = uuidString;

                        sendHandShake_();
                    }
                }
                break;

                // Рукопожатие (только во время установки соединения)
                case PacketType::HandShake :
                {
                    if (lastRequest_ == PacketType::HandShake)
                    {
                        QString message;
                        QString srvName, srvVer;
                        in >> srvName >> srvVer;

                        if (!checkHandShake_(srvName, srvVer, message))
                        {
                            sendRefuseConnection_(message);
                        }
                        else
                        {
                            sendConnectionOK_();

                            // Отправляем тип запрашиваемого подключения
                            if (connectionMode_ == ConnectionType::DriversListRequest)
                                sendDriversListRequest_();
                            else if (connectionMode_ == ConnectionType::WithoutTicket)
                                sendWithoutTicketConnectionRequest_();
                        }
                    }
                }
                break;

                // Список доступных драйверов (только во время установки соединения)
                case PacketType::DriversList :
                {
                    emit infoMessage(tr("Reading available equipment list from server..."));

                    UniversalNamedMap driverItem;
                    QList<UniversalNamedMap> driversList;

                    int count = 0;
                    in >> count;
                    emit infoMessage(tr("Total %1 item(s) available").arg(count));

                    for (int i = 0; i < count; ++i)
                    {
                        QUuid uuid;
                        QString name, desc;
                        in >> uuid >> name >> desc;
                        emit infoMessage(tr("\t %1 : %2 : %3").arg(uuid.toString()).arg(name).arg(desc));

                        driverItem["uuid"] = uuid.toString();
                        driverItem["name"] = name;
                        driverItem["desc"] = desc;

                        driversList.append(driverItem);
                    }

                    // Обработка списка доступных драйверов зависит от того, что мы запрашивали.
                    // Если мы запрашивали подключение без билета, то пользователь должен выбрать драйвер
                    // Если мы ... Пока не прозрачен механизм выбора... Надо упрощать...
                    if (lastRequest_ == ConnectionType::WithoutTicket)
                    {
                        bool autoConnect = false;
                        if (clientSettings->autoConnect())
                        {
                            Core::UniversalNamedMap item;
                            foreach(item, driversList)
                            {
                                if (item["uuid"] == clientSettings->defaultDriverUuid())
                                {
                                    connectionState_.driverName = item["name"].toString();
                                    connectionState_.driverUuid = QUuid(item["uuid"].toString()).toString();
                                    sendSelectedDriverUuid_(QUuid(item["uuid"].toString()));
                                    autoConnect = true;
                                    break;
                                }
                            }
                        }

                        if (!clientSettings->autoConnect() || !autoConnect)
                        {
                            //! @todo TODO разделить код интерфейсных элементов и элементов ядра системы

                            udav_gui::ClientSelectDriverDialog dlg(driversList);
                            int ret = dlg.exec();

                            if (ret == QDialog::Accepted)
                            {
                                QUuid selUuid = dlg.getSelection();
                                foreach(Core::UniversalNamedMap item, driversList)
                                {
                                    if (item["uuid"].toString() == selUuid.toString())
                                    {
                                        connectionState_.driverName = item["name"].toString();
                                        connectionState_.driverUuid = QUuid(item["uuid"].toString()).toString();
                                        break;
                                    }
                                }
                                sendSelectedDriverUuid_(selUuid);
                            }
                            else
                                sendRefuseConnection_(tr("User closed selection without decision"));
                        }
                    }
                    else if (lastRequest_ == ConnectionType::DriversListRequest)
                    {
                        emit debugMessage(tr("Sending signal drivers list readed to ui elements"));

                        // Сообщаем интерфейсным элементам, что прочитан список драйверов

                        emit driversListReaded(driversList);

                        closeConnection();
                    }
                } break;

                // Массив метасигналов (только во время установки соединения)
                case PacketType::MetaSignals :
                {
                    if (lastRequest_ == PacketType::SelectedDriver)
                    {
                        emit infoMessage(tr("Reading metasignals from server..."));

                        metaSignals_.clear();
                        in >> metaSignals_;

                        emit infoMessage(tr("Total %1 metasignal(s) readed").arg(metaSignals_.count()));
                        emit infoMessage(tr("\t SignalID : Name : Desc : Type : MinValue : MaxValue : DefaultValue"));

                        Drivers::MetaSignalsJavaStyleIterator i(metaSignals_);
                        while (i.hasNext())
                        {
                            i.next();
                            Drivers::MetaSignal item = i.value();
                            emit infoMessage(tr("\t %1 : %2 : %3 : %4 : %5 : %6 : %7").
                                             arg(i.key()).
                                             arg(item.fieldName).
                                             arg(item.fieldDesc).
                                             arg(item.valueType2String(item.valueType)).
                                             arg(item.minValue).
                                             arg(item.maxValue).
                                             arg(item.defaultValue));
                        }

                        allowedReplies_.clear();
                        allowedReplies_ << PacketType::UiDescription;
                    }
                } break;

                // Описание интерфейса пользователя  (только во время установки соединения)
                case PacketType::UiDescription :
                {
                    if (lastRequest_ == PacketType::SelectedDriver)
                    {
                        emit infoMessage(tr("Reading ui description from server..."));

                        QString uiDesc;
                        in >> uiDesc;

                        emit uiDescriptionReaded(uiDesc);

                        // Ожидаем карту настройки драйвера оборудования...

                        allowedReplies_.clear();
                        allowedReplies_ << PacketType::DriverSettings;
                    }
                } break;

                // Настройки драйвера оборудования (только во время установки соединения)
                case PacketType::DriverSettings :
                {
                    emit infoMessage(tr("Reading driver settings from server..."));

                    driverSettings_.clear();
                    in >> driverSettings_;

                    emit infoMessage(tr("Total %1 value(s) readed").arg(driverSettings_.count()));

    //				for(Drivers::SignalValuesConstIterator i = viewValues.begin(); i != viewValues.end(); ++i)
    //				{
    //					logger << "\tid: " << i.key() << ", value: " << i.value().toString() << endl;
    //					emit signalValueChanged(i.key(), i.value());
    //				}

                    // Это последний из служебных пакетов в процессе установки соединения. Cоединение успешно установлено

                    sendConnectionEstablished_();

                    connectionState_.isConnected = true;
                    connectionState_.conTime = QDateTime::currentDateTime();

                    if (clientSettings->writeProtocol()) createNewProtocol();

                    allowedReplies_.clear();
                    allowedReplies_
                            << PacketType::ViewArray
                            << PacketType::CheckConnection;
                } break;
            }
        }
        else
        {
            // Режим работы при установленном соединении
            /*
             * В режиме установленного соединения сервер может прислать:
             * - запрос наличия связи
             * - подтверждение или отказ выполнения команды
             * - файл
             * - массив значений сигналов наблюдения
             * - значение единичного сигнала
             * - описание интерфейса пользователя (после запроса на его повторную загрузку)
            */
            switch (reply)
            {
                case PacketType::CheckConnection : // Сервер запросил проверку соединения
                {
                    sendConnectionOK_();
                } break;

                case PacketType::CommandOk : // Сервер принял команду сигнала управления к выполнению
                {
                    emit infoMessage(tr("Server processed the command successfully"));
                } break;

                case PacketType::CommandError : // Сервер отказал команде сигнала управления в выполнении
                {
                    // Надо вернуть кнопки управления в исходное состояние
                    // И прочитать причину отказа
                    Drivers::SignalID signalID;
                    QVariant value;
                    QString message;

                    in >> signalID >> value >> message;

                    emit infoMessage(tr("Server cannot process control signal! Reason: %1").arg(message));
                    emit signalValueChanged(signalID, value);
                } break;

                case PacketType::File : // Сервер передал файл по запросу. Используется для передачи файлов SVG в мнемосхемах
                {
                    // Полученный файл сохраняем во временной папке...
                    //! @todo Сохранять во временном файле, а потом копировать на постоянное место. Иначе может зависнуть при чтении неполностью принятого файла
                    QString fname;
                    QByteArray buf;
                    in >> fname >> buf;

                    QString tempPath = getTempPath();
                    checkFilePath(fname, tempPath);

                    QFile f(buildFullFileName(fname, tempPath));
                    if (f.open(QIODevice::WriteOnly))
                    {
                        QDataStream os(&f);
                        os.writeRawData(buf.data(), buf.size());
                    }
                    else
                    {
                        emit criticalMessage(tr("Cannot open file for writing! Error: %1").arg(qPrintable(f.errorString())));
                        break;
                    }

                    emit fileRecieved(fname);
                } break;

                case PacketType::SignalValue : // Сервер сообщил значение единичного сигнала
                {
                    emit debugMessage(tr("Signal value recieved"));

                    // Получен единичный сигнал

                    Drivers::SignalID signalId;
                    QVariant value;
                    QString message;

                    in >> signalId >> value >> message;
                    emit debugMessage(tr("Id: %1; value: %2; message: %3").arg(signalId).arg(value.toString()).arg(message));

                    emit signalValueChanged(signalId, value, message);
                } break;

                case PacketType::UiDescription : // Сервер по запросу перечитал и передал описание интерфейса пользователя
                {
                    emit infoMessage(tr("Reloading ui description from server..."));

                    QString uiDesc;
                    in >> uiDesc;

                    emit uiDescriptionReaded(uiDesc);
                } break;

                case PacketType::ViewArray : // Массив сигналов наблюдения с сервера
                {
                    emit infoMessage(tr("Reading view values from server..."));

                    viewValues_.clear();
                    in >> viewValues_;

                    emit infoMessage(tr("Total %1 value(s) readed").arg(viewValues_.count()));

                    for(Drivers::SignalValuesConstIterator i = viewValues_.begin(); i != viewValues_.end(); ++i)
                    {
                        emit infoMessage(tr("\tid: %1, value: %2").arg(i.key()).arg(i.value().toString()));
                        emit signalValueChanged(i.key(), i.value());
                    }
                } break;

                case PacketType::ControlArray : // Массив сигналов управления с сервера
                {
                    emit infoMessage(tr("Reading control values from server..."));

                    controlValues_.clear();
                    in >> controlValues_;

                    emit infoMessage(tr("Total %1 value(s) readed").arg(controlValues_.count()));

                    for(Drivers::SignalValuesConstIterator i = controlValues_.begin(); i != controlValues_.end(); ++i)
                    {
                        emit infoMessage(tr("\tid: %1, value: %2").arg(i.key()).arg(i.value().toString()));
                        emit signalValueChanged(i.key(), i.value());
                    }
                } break;

                default:
                {
                    emit criticalMessage(tr("In connected mode: Unknown reply recieved"));
                    emit criticalMessage(tr("Reply is: %1 (%2)").arg(reply).arg(packetNames[reply]));
                    sendRefuseConnection_("Unknown reply recieved");
                }

            }

        }

        //-----------------------------------------------------------------------------------------------------
        // Неиспользуемые типы сигналов. Удалить?

//        // Сервер прислал текстовое сообщение.  Это экстренная ситуация?
//        else if (reply == PacketType::TextMessage)
//        {
//            qDebug() <<  "Text message recieved";

//            Drivers::SignalID channel;
//            QString message;

//            in >> channel >> message;

//            qDebug() << "Channel:" << channel << ":" << message;

//            emit textMessageRecieved(channel, message);
//        }

//        switch (reply)
//        {
//            // Массив сигналов управления с сервера
//            case PacketType::ControlArray :
//                *logger << tr("Reading control values from server...") << endl;

//                controlValues_.clear();
//                in >> controlValues_;

//                *logger << tr("Total ") << controlValues_.count() << " value(s) readed" << endl;
//            break;


//        }

        nextBlockSize_ = 0;
    }
}

//-----------------------------------------------------------------------------

void
Client::requestConnection_()
{
    emit infoMessage(tr("Sending connection request to server..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::RequestConnection;	// Сохраняем код запроса
    out << PacketType::RequestConnection;			// Записываем код запроса на подключение
    out << endp;									// Записываем в поток признак конца данных

    allowedReplies_.clear();
    allowedReplies_ << PacketType::ConnectionOK << PacketType::RefuseConnection;

    tcpSocket_.write(block);
}

//-----------------------------------------------------------------------------

void
Client::requestFileFromServer(const QString& fname)
{
    if (!isConnected())
    {
        emit infoMessage(tr("Connection is not established! Cannot request file from server"));
        return;
    }

    emit infoMessage(tr("Sending file request to server..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::FileRequest;
    out << PacketType::FileRequest << fname << endp;

    allowedReplies_.clear();
    allowedReplies_
            << PacketType::ConnectionOK
            << PacketType::CheckConnection
            << PacketType::File
            << PacketType::ViewArray
            << PacketType::ControlArray
            << PacketType::RefuseConnection;

    tcpSocket_.write(block);
}

//-----------------------------------------------------------------------------

void
Client::sendConnectionEstablished_()
{
    emit infoMessage(tr("Sending 'Connection established' to server..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::ConnectionEstablsihed;
    out << PacketType::ConnectionEstablsihed;
    out << endp;

    tcpSocket_.write(block);

    emit connectionEstablished(clientSettings->serverAddress());
}

//-----------------------------------------------------------------------------

void
Client::sendConnectionOK_()
{
    emit infoMessage(tr("Sending 'Connection OK' to server..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::ConnectionOK;
    out << PacketType::ConnectionOK;
    out << endp;

    tcpSocket_.write(block);
}

//-----------------------------------------------------------------------------

void
Client::sendDriversListRequest_()
{
    emit infoMessage(tr("Requesting drivers list from server..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = ConnectionType::DriversListRequest;
    out << ConnectionType::DriversListRequest << endp;


    allowedReplies_.clear();
    allowedReplies_ << PacketType::DriversList << PacketType::RefuseConnection;

    tcpSocket_.write(block);
}

//-----------------------------------------------------------------------------

void
Client::sendDriverSettings()
{
    emit infoMessage(tr("Sending driver settings to server..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::DriverSettings;
    out << PacketType::DriverSettings;

    out << driverSettings_ << endp;

    allowedReplies_.clear();
    allowedReplies_ << PacketType::ConnectionOK << PacketType::RefuseConnection;

    tcpSocket_.write(block);
}

//-----------------------------------------------------------------------------

void
Client::sendHandShake_()
{
    emit infoMessage(tr("Sending handshake packet to server..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::HandShake;
    out << PacketType::HandShake;
    out << ClientName << RevisionNumber << endp;

    allowedReplies_.clear();
    allowedReplies_ << PacketType::HandShake << PacketType::RefuseConnection;

    tcpSocket_.write(block);
}

//-----------------------------------------------------------------------------

void
Client::sendInterfaceReloadRequest()
{
    if (!isConnected())
    {
        emit infoMessage(tr("Connection is not established! Cannot request user interface from server"));
        return;
    }

    emit infoMessage(tr("Sending interface reload request packet to server..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::ReloadInterface;
    out << PacketType::ReloadInterface << endp;

    allowedReplies_.clear();
    allowedReplies_ << PacketType::UiDescription;

    tcpSocket_.write(block);
}

//-----------------------------------------------------------------------------

void
Client::sendRefuseConnection_(const QString& reason)
{
    emit criticalMessage(tr("Refusing connection with server..."));
    emit criticalMessage(tr("Reason: %1").arg(reason));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::RefuseConnection;
    out << PacketType::RefuseConnection;
    out << reason << endp;

    tcpSocket_.write(block);
}

//-----------------------------------------------------------------------------

void
Client::sendRequest_()
{
    emit infoMessage(tr("[Deprecated] Sending request to server..."));

    QByteArray block;
    BlockStream out(&block);

    // ЭТОТ МЕТОД ПОКА НЕ ИСПОЛЬЗУЕТСЯ!!!

//	out << NetworkPackets::requestConnection << NetworkPackets::withoutTicket << NetworkPackets::endp;

//	allowedReplies.clear();
//	allowedReplies << NetworkPackets::driversList << NetworkPackets::refuseConnection;

    tcpSocket_.write(block);
}

//-----------------------------------------------------------------------------
/**
  После отправки uuid выбранного драйвера ожидаются ответы от сервера:
  NetworkPackets::metaSignals
  NetworkPackets::refuseConnection
*/
void
Client::sendSelectedDriverUuid_(const QUuid& uuid)
{
    emit infoMessage(tr("Driver with uuid %1 selected").arg(uuid.toString()));
    emit infoMessage(tr("Sending selection to server..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = PacketType::SelectedDriver;
    out << PacketType::SelectedDriver << uuid << endp;

    allowedReplies_.clear();
    allowedReplies_ << PacketType::MetaSignals << PacketType::RefuseConnection;

    tcpSocket_.write(block);
}

//-----------------------------------------------------------------------------

void
Client::sendWithoutTicketConnectionRequest_()
{
    emit infoMessage(tr("Sending without ticket connection request..."));

    QByteArray block;
    BlockStream out(&block);

    lastRequest_ = ConnectionType::WithoutTicket;
    out << ConnectionType::WithoutTicket << endp;


    allowedReplies_.clear();
    allowedReplies_ << PacketType::DriversList << PacketType::RefuseConnection;

    tcpSocket_.write(block);
}

//-----------------------------------------------------------------------------

void
Client::writeProtocolString_()
{
    QString fileName = checkBackSlash(clientSettings->protocolPath()) + protocolFileName_;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Append))
    {
        emit criticalMessage(tr("Cannot open protocol file %1 for adding data!").arg(fileName));
    }
    else
    {
        QTextStream out(&file);
        foreach(Drivers::MetaSignal item, metaSignals_)
        {
            Drivers::MetaSignal::ValueType valueType = item.valueType;

            if (item.signalType == Drivers::MetaSignal::Observation)
            {
                bool ok;
                if (valueType == Drivers::MetaSignal::Boolean)
                    out << viewValues_[metaSignals_.key(item)].toBool() << "; ";
                else if (valueType == Drivers::MetaSignal::Integer)
                    out << viewValues_[metaSignals_.key(item)].toInt(&ok) << "; ";
                else if (valueType == Drivers::MetaSignal::Real)
                    out << viewValues_[metaSignals_.key(item)].toDouble(&ok) << "; ";
                else if (valueType == Drivers::MetaSignal::Time)
                    out << viewValues_[metaSignals_.key(item)].toDateTime().toString("hh:mm:ss dd-MM-yyyy") << "; ";
            }
            else
            {
                bool ok;
                if (valueType == Drivers::MetaSignal::Boolean)
                    out << viewValues_[metaSignals_.key(item)].toBool() << "; ";
                else if (valueType == Drivers::MetaSignal::Integer)
                    out << viewValues_[metaSignals_.key(item)].toInt(&ok) << "; ";
                else if (valueType == Drivers::MetaSignal::Real)
                    out << viewValues_[metaSignals_.key(item)].toDouble(&ok) << "; ";
                else if (valueType == Drivers::MetaSignal::Time)
                    out << viewValues_[metaSignals_.key(item)].toDateTime().toString("hh:mm:ss dd-MM-yyyy") << "; ";
            }
        }
        out << "\n";

        file.close();
    }
}


}
