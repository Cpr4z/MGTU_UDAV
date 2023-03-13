#include "client_settings.h"
#include <QSettings>
#include <QRect>
#include <QString>
#include <QStringList>

namespace Core
{

//==========================================================
// Строковые константы для имен сохраняемых параметров (BOF)

//----------------------------------
// Раздел "General"/"Общие настройки"

const char* ClientLogPath = "LogPath";

//----------------------------------
// Раздел "Connection"/"Подключение"

const char* ServerAddress     = "Connection/Server";
const char* ServerPort        = "Connection/Port";
const char* CacheFiles        = "Connection/CacheFiles";
const char* AutoConnect       = "Connection/AutoConnect";
const char* DefaultDriverName = "Connection/DefaultDriverName";
const char* DefaultDriverUuid = "Connection/DefaultDriverUuid";

//------------------------------------
// Раздел "Protocol"/"Протокол работы"

const char* WriteProtocol                   = "Protocol/WriteProtocol";
const char* ProtocolPath                    = "Protocol/Path";
const char* ProtocolTimer                   = "Protocol/Timer";
const char* DeleteProtocolFilesAfterDays    = "Protocol/DeleteFilesAfterDays";
const char* ProtocolSaveToPath              = "Protocol/SaveToPath";

//----------------------------------
// Раздел "Interface"/"Внешний вид"

const char* StartInFullScreenMode   = "Interface/StartInFullScreenMode";
const char* SaveWindowsPosition     = "Interface/SaveWindowsPosition";
const char* RestoreWindowsPosition  = "Interface/RestoreWindowsPosition";
const char* ShowGrid                = "Interface/ShowGrid";
const char* ToolBarButtonSize       = "Interface/ToolBarButtonSize";
const char* ShowToolBarText         = "Interface/ShowToolBarText";
const char* ShowDebugPage           = "Interface/ShowDebugPage";
const char* ShowLogMessagesPage     = "Interface/ShowLogMessagesPage";

//------------------------------------
// Раздел "Geometry"/"Позиции окон"

const char* Geometry                = "Geometry/%1";

//------------------------------------
// Раздел "Logging"/"Лог-файлы"

const char* ClearClientLogAtStart         = "Logging/ClearLogFiles";

// Строковые константы для имен сохраняемых параметров (EOF)
//==========================================================

ClientSettings::ClientSettings(const QString& organization, const QString& application, QObject * parent)
    : Settings(organization, application, parent)
{
    //=====================================
    // Значения настроек по умолчанию (BOF)

    //----------------------------------
    // Раздел "General"/"Общие настройки"

    //logPath_ = "/var/log/udav";
    logPath_ = "./";

    //----------------------------------
    // Раздел "Connection"/"Подключение"

    serverAddress_ = QHostAddress::LocalHost;
    serverPort_ = 6178;
    cacheFiles_ = true;
    autoConnect_ = false;
    defaultDriverName_ = QString();
    defaultDriverUuid_ = QString();

    //------------------------------------
    // Раздел "Protocol"/"Протокол работы"

    writeProtocol_ = false;
    protocolPath_ = QString("./");
    protocolTimer_ = 0;
    deleteProtocolFilesAfterDays_ = 10;
    protocolSaveToPath_ = QString("/media");

    //----------------------------------
    // Раздел "Interface"/"Внешний вид"

    startInFullScreenMode_ = false;
    saveWindowsPos_ = false;
    restoreWindowsPos_ = false;
    showGrid_ = true;
    toolBarButtonSize_ = 100;
    showToolBarText_ = true;
    showDebugPage_ = true;
    showLogMessagesPage_ = true;

    //------------------------------------
    // Раздел "Logging"/"Лог-файлы"

    clearLogAtStart_ = false;

    // Значения настроек по умолчанию (EOF)
    //=====================================

    QSettings s(getOrgName(), getAppName());

    //======================
    // Чтение настроек (BOF)

    //----------------------------------
    // Раздел "General"/"Общие настройки"

    QString logPath = s.value(ClientLogPath).toString();
    if (!logPath.isEmpty()) logPath_ = logPath;

    //----------------------------------
    // Раздел "Connection"/"Подключение"

    QString serverAddresString = s.value(ServerAddress).toString();
    if (!serverAddresString.isEmpty()) serverAddress_.setAddress(serverAddresString);

    bool ok = true;
    quint16 iPort = s.value(ServerPort).toInt(&ok);
    if (ok && iPort) serverPort_ = iPort;

    cacheFiles_ = s.value(CacheFiles).toBool();

    autoConnect_ = s.value(AutoConnect).toBool();
    defaultDriverName_ = s.value(DefaultDriverName).toString();
    defaultDriverUuid_ = s.value(DefaultDriverUuid).toString();

    //------------------------------------
    // Раздел "Protocol"/"Протокол работы"

    writeProtocol_ = s.value(WriteProtocol).toBool();

    QString protocolPath = s.value(ProtocolPath).toString();
    if (!protocolPath.isEmpty()) protocolPath_ = protocolPath;

    quint16 iProtocolTimer = s.value(ProtocolTimer).toInt(&ok);
    if (ok && iProtocolTimer) protocolTimer_ = iProtocolTimer;

    quint16 iDeleteProtocolFilesAfterDays = s.value(DeleteProtocolFilesAfterDays).toInt(&ok);
    if (ok) deleteProtocolFilesAfterDays_ = iDeleteProtocolFilesAfterDays;

    QString protocolSaveToPath = s.value(ProtocolSaveToPath).toString();
    if (!protocolPath.isEmpty()) protocolSaveToPath_ = protocolSaveToPath;

    //----------------------------------
    // Раздел "Interface"/"Внешний вид"

    startInFullScreenMode_ = s.value(StartInFullScreenMode).toBool();
    saveWindowsPos_ = s.value(SaveWindowsPosition).toBool();
    restoreWindowsPos_ = s.value(RestoreWindowsPosition).toBool();
    showGrid_ = s.value(ShowGrid).toBool();

    int bs = s.value(ToolBarButtonSize).toInt(&ok);
    if (ok) toolBarButtonSize_ = bs;

    showToolBarText_ = s.value(ShowToolBarText).toBool();
    showDebugPage_ = s.value(ShowDebugPage).toBool();
    showLogMessagesPage_ = s.value(ShowLogMessagesPage).toBool();

    //------------------------------------
    // Раздел "Logging"/"Лог-файлы"

    clearLogAtStart_ = s.value(ClearClientLogAtStart).toBool();

    // Чтение настроек (EOF)
    //======================

    // Сохранение настроек гарантирует правильную
    // запись значений по умолчанию для добавленных элементов
    save();
}

//-----------------------------------------------------------------------------

void
ClientSettings::save() const
{
    QSettings s(getOrgName(), getAppName());

    //==========================
    // Сохранение настроек (BOF)

    //----------------------------------
    // Раздел "General"/"Общие настройки"

    s.setValue(ClientLogPath, logPath_);

    //----------------------------------
    // Раздел "Connection"/"Подключение"

    s.setValue(ServerAddress, serverAddress_.toString());
    s.setValue(ServerPort, serverPort_);
    s.setValue(CacheFiles, cacheFiles_);
    s.setValue(AutoConnect, autoConnect_);
    s.setValue(DefaultDriverName, defaultDriverName_);
    s.setValue(DefaultDriverUuid, defaultDriverUuid_);

    //------------------------------------
    // Раздел "Protocol"/"Протокол работы"

    s.setValue(WriteProtocol, writeProtocol_);
    s.setValue(ProtocolPath, protocolPath_);
    s.setValue(ProtocolTimer, protocolTimer_);
    s.setValue(DeleteProtocolFilesAfterDays, deleteProtocolFilesAfterDays_);
    s.setValue(ProtocolSaveToPath, protocolSaveToPath_);

    //----------------------------------
    // Раздел "Interface"/"Внешний вид"

    s.setValue(StartInFullScreenMode, startInFullScreenMode_);
    s.setValue(SaveWindowsPosition, saveWindowsPos_);
    s.setValue(RestoreWindowsPosition, restoreWindowsPos_);
    s.setValue(ShowGrid, showGrid_);

    s.setValue(ToolBarButtonSize, int(toolBarButtonSize_));
    s.setValue(ShowToolBarText, showToolBarText_);
    s.setValue(ShowDebugPage, showDebugPage_);
    s.setValue(ShowLogMessagesPage, showLogMessagesPage_);

    //------------------------------------
    // Раздел "Logging"/"Лог-файлы"

    s.setValue(ClearClientLogAtStart, clearLogAtStart_);

    // Сохранение настроек (EOF)
    //==========================

    s.sync();
}

//-----------------------------------------------------------------------------

QRect
ClientSettings::readWindowGeometry(const QString &windowName)
{
    QRect result;

    QSettings s(getOrgName(), getAppName());

    QString geomStr = s.value(QString(Geometry).arg(windowName)).toString();
    if (!geomStr.isEmpty())
    {
        QStringList geomStrList = geomStr.split(";");

        bool ok;
        result.setX(geomStrList[0].toInt(&ok));
        result.setY(geomStrList[1].toInt(&ok));
        result.setWidth(geomStrList[2].toInt(&ok));
        result.setHeight(geomStrList[3].toInt(&ok));
    }

    return result;
}

//-----------------------------------------------------------------------------

void
ClientSettings::writeWindowGeometry(const QString &windowName, const QRect &geometry)
{
    QString geometryStr = QString("%1;%2;%3;%4").arg(geometry.x()).arg(geometry.y()).arg(geometry.width()).arg(geometry.height());
    QSettings s(getOrgName(), getAppName());
    s.setValue(QString(Geometry).arg(windowName), geometryStr);
}

//-----------------------------------------------------------------------------

}
