#include "server_settings.h"
#include <QSettings>

namespace Core
{

//==========================================================
// Строковые константы для имен сохраняемых параметров (BOF)

const char* LogPath                     = "LogPath";
const char* Port                        = "Port";
const char* ConnectCheckingIterval      = "ConnectCheckingIterval";
const char* ConnectLostAfterAttempts    = "ConnectLostAfterAttempts";
const char* SendDataInterval            = "SendDataInterval";
const char* PathToDrivers               = "PathToDrivers";
const char* ClearLogAtStart             = "ClearLogAtStart";

// Строковые константы для имен сохраняемых параметров (EOF)
//==========================================================

ServerSettings::ServerSettings(const QString& organization, const QString& application, QObject * parent)
    : Settings(organization, application, parent)
{

    //=====================================
    // Значения настроек по умолчанию (BOF)

    logPath_ = "/var/log/udav"; //! @todo Платформонезависимые значения! Issue #95
    port_ = 6178;
    connectCheckingIterval_ = 10;
    connectLostAfterAttempts_ = 3;
    sendDataInterval_ = 15;
    pathToDrivers_ = "/usr/local/lib/udav";

    // Значения настроек по умолчанию (EOF)
    //=====================================

    QSettings s(getOrgName(), getAppName());

    //======================
    // Чтение настроек (BOF)

    QString logPath = s.value(LogPath).toString();
    if (!logPath.isEmpty()) logPath_ = logPath;

    bool ok = true;
    quint16 iPort = s.value(Port).toInt(&ok);
    if (ok && iPort) port_ = iPort;

    quint16 iConnectCheckingIterval = s.value(ConnectCheckingIterval).toInt(&ok);
    if (ok) connectCheckingIterval_ = iConnectCheckingIterval;

    quint8 iConnectLostAfterAttempts = s.value(ConnectLostAfterAttempts).toInt(&ok);
    if (ok) connectLostAfterAttempts_ = iConnectLostAfterAttempts;

    quint16 iSendDataInterval = s.value(SendDataInterval).toInt(&ok);
    if (ok) sendDataInterval_ = iSendDataInterval;

    QString sPathToDrivers = s.value(PathToDrivers).toString();
    if (ok) pathToDrivers_ = sPathToDrivers;

    clearLogAtStart_ = s.value(ClearLogAtStart).toBool();

    // Чтение настроек (EOF)
    //======================

    // Сохранение настроек гарантирует правильную
    // запись значений по умолчанию для добавленных элементов
    save();
}

//-----------------------------------------------------------------------------

void
ServerSettings::save() const
{
    QSettings s(getOrgName(), getAppName());

    //==========================
    // Сохранение настроек (BOF)

    s.setValue(LogPath, logPath_);
    s.setValue(Port, port_);
    s.setValue(ConnectCheckingIterval, connectCheckingIterval_);
    s.setValue(ConnectLostAfterAttempts, connectLostAfterAttempts_);
    s.setValue(SendDataInterval, sendDataInterval_);
    s.setValue(PathToDrivers, pathToDrivers_);
    s.setValue(ClearLogAtStart, clearLogAtStart_);

    // Сохранение настроек (EOF)
    //==========================
}

//-----------------------------------------------------------------------------

}
