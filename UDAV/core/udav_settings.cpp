#include "udav_settings.h"
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

UdavSettings::UdavSettings(const QString& organization, const QString& application, QObject * parent)
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

    autoConnect_ = s.value(AutoConnect).toBool();
    defaultDriverName_ = s.value(DefaultDriverName).toString();
    defaultDriverUuid_ = s.value(DefaultDriverUuid).toString();

    //------------------------------------
    // Раздел "Protocol"/"Протокол работы"

    writeProtocol_ = s.value(WriteProtocol).toBool();

    QString protocolPath = s.value(ProtocolPath).toString();
    if (!protocolPath.isEmpty()) protocolPath_ = protocolPath;

    bool ok;

    quint16 iProtocolTimer = s.value(ProtocolTimer).toInt(&ok);
    if (ok && iProtocolTimer)
    {
        protocolTimer_ = iProtocolTimer;
    }

    quint16 iDeleteProtocolFilesAfterDays = s.value(DeleteProtocolFilesAfterDays).toInt(&ok);
    if (ok)
    {
        deleteProtocolFilesAfterDays_ = iDeleteProtocolFilesAfterDays;
    }

    QString protocolSaveToPath = s.value(ProtocolSaveToPath).toString();
    if (!protocolPath.isEmpty())
    {
        protocolSaveToPath_ = protocolSaveToPath;
    }

    //----------------------------------
    // Раздел "Interface"/"Внешний вид"

    startInFullScreenMode_ = s.value(StartInFullScreenMode).toBool();
    saveWindowsPos_ = s.value(SaveWindowsPosition).toBool();
    restoreWindowsPos_ = s.value(RestoreWindowsPosition).toBool();
    showGrid_ = s.value(ShowGrid).toBool();

    int bs = s.value(ToolBarButtonSize).toInt(&ok);
    if (ok)
    {
        toolBarButtonSize_ = bs;
    }

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
UdavSettings::save() const
{
    QSettings s(getOrgName(), getAppName());

    //==========================
    // Сохранение настроек (BOF)

    //----------------------------------
    // Раздел "General"/"Общие настройки"

    s.setValue(ClientLogPath, logPath_);

    //----------------------------------
    // Раздел "Connection"/"Подключение"

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
UdavSettings::readWindowGeometry(const QString &windowName)
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
UdavSettings::writeWindowGeometry(const QString &windowName, const QRect &geometry)
{
    QString geometryStr = QString("%1;%2;%3;%4").arg(geometry.x()).arg(geometry.y()).arg(geometry.width()).arg(geometry.height());
    QSettings s(getOrgName(), getAppName());
    s.setValue(QString(Geometry).arg(windowName), geometryStr);
}

const QString&
UdavSettings::logPath() const
{
    return logPath_;
}

void Core::UdavSettings::setLogPath(const QString& logPath)
{
    logPath_ = logPath;
}

//-----------------------------------------------------------------------------

bool
UdavSettings::autoConnect() const
{
    return autoConnect_;
}

//-----------------------------------------------------------------------------

void
UdavSettings::setAutoConnect(bool autoConnect)
{
    autoConnect_ = autoConnect;
}

//-----------------------------------------------------------------------------

QString
UdavSettings::defaultDriverName() const
{
    return defaultDriverName_;
}

//-----------------------------------------------------------------------------

QString
UdavSettings::defaultDriverUuid() const
{
    return defaultDriverUuid_;
}

//-----------------------------------------------------------------------------

void
UdavSettings::setDefaultDriver(const QString& driverName, const QString& driverUuid)
{
    defaultDriverName_ = driverName;
    defaultDriverUuid_ = driverUuid;
}

//-----------------------------------------------------------------------------

bool UdavSettings::writeProtocol() const
{
    return writeProtocol_;
}

//-----------------------------------------------------------------------------

void UdavSettings::setWriteProtocol(bool writeProtocol)
{
    writeProtocol_ = writeProtocol;
}

//-----------------------------------------------------------------------------

QString
UdavSettings::protocolPath() const
{
    return protocolPath_;
}

//-----------------------------------------------------------------------------

void
UdavSettings::setProtocolPath(const QString& protocolPath)
{
    protocolPath_ = protocolPath;
}

//-----------------------------------------------------------------------------

int
UdavSettings::protocolTimer() const
{
    return protocolTimer_;
}

//-----------------------------------------------------------------------------

void
UdavSettings::setProtocolTimer(int protocolTimer)
{
    protocolTimer_ = protocolTimer;
}

//-----------------------------------------------------------------------------

int
UdavSettings::deleteProtocolFilesAfterDays() const
{
    return deleteProtocolFilesAfterDays_;
}

//-----------------------------------------------------------------------------

void
UdavSettings::setDeleteProtocolFilesAfterDays(int deleteProtocolFilesAfterDays)
{
    deleteProtocolFilesAfterDays_ = deleteProtocolFilesAfterDays;
}

//-----------------------------------------------------------------------------

QString
UdavSettings::protocolSaveToPath() const
{
    return protocolSaveToPath_;

}

//-----------------------------------------------------------------------------

void
UdavSettings::setProtocolSaveToPath(const QString& protocolSaveToPath)
{
    protocolSaveToPath_ = protocolSaveToPath;
}

//-----------------------------------------------------------------------------

bool
UdavSettings::startInFullScreenMode() const
{
    return startInFullScreenMode_;
}

//-----------------------------------------------------------------------------

void
UdavSettings::setStartInFullScreenMode(bool startInFullScreenMode)
{
    startInFullScreenMode_ = startInFullScreenMode;
}

//-----------------------------------------------------------------------------

bool
UdavSettings::saveWindowsPos() const
{
    return saveWindowsPos_;
}

//-----------------------------------------------------------------------------

void
UdavSettings::setSaveWindowsPos(bool saveWindowsPos)
{
    saveWindowsPos_ = saveWindowsPos;
}

//-----------------------------------------------------------------------------

bool
UdavSettings::restoreWindowsPos() const
{
    return restoreWindowsPos_;
}

//-----------------------------------------------------------------------------

void
UdavSettings::setRestoreWindowsPos(bool restoreWindowsPos)
{
    restoreWindowsPos_ = restoreWindowsPos;
}

//-----------------------------------------------------------------------------

bool
UdavSettings::showGrid() const
{
    return showGrid_;
}

//-----------------------------------------------------------------------------

void
UdavSettings::setShowGrid(bool showGrid)
{
    showGrid_ = showGrid;
}

//-----------------------------------------------------------------------------

int
UdavSettings::toolBarButtonSize() const
{
    return toolBarButtonSize_;
}

//-----------------------------------------------------------------------------

void
UdavSettings::setToolBarButtonSize(int toolBarButtonSize)
{
    toolBarButtonSize_ = toolBarButtonSize;
}

//-----------------------------------------------------------------------------

bool
UdavSettings::showToolBarText() const
{
    return showToolBarText_;
}

//-----------------------------------------------------------------------------

void
UdavSettings::setShowToolBarText(bool showToolBarText)
{
    showToolBarText_ = showToolBarText;
}

//-----------------------------------------------------------------------------

bool
UdavSettings::showDebugPage() const
{
    return showDebugPage_;
}

//-----------------------------------------------------------------------------

void
UdavSettings::setShowDebugPage(bool showDebugPage)
{
    showDebugPage_ = showDebugPage;
}

//-----------------------------------------------------------------------------

bool
UdavSettings::showLogMessagesPage() const
{
    return showLogMessagesPage_;
}

//-----------------------------------------------------------------------------

void
UdavSettings::setShowLogMessagesPage(bool showLogMessagesPage)
{
    showLogMessagesPage_ = showLogMessagesPage;
}

//-----------------------------------------------------------------------------

bool
UdavSettings::clearLogAtStart() const
{
    return clearLogAtStart_;
}

//-----------------------------------------------------------------------------

void
UdavSettings::setClearLogAtStart(bool clearLogAtStart)
{
    clearLogAtStart_ = clearLogAtStart;
}

//-----------------------------------------------------------------------------

}
