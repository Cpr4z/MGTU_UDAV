/**
    $Id: driver_settings.cpp 920 2016-04-19 06:46:27Z l.kolesnik $
    $URL: https://office.m-i.ru/svn/udav/trunk/driver/driver_settings.cpp $
*/

#include "driver_settings.h"

#include <QSettings>
#include <QStringList>

namespace udav_drivers
{

//-----------------------------------------------------------------------------

DriverSettings::DriverSettings(const QString& organization, const QString& application, const QString& driverName, QObject * parent)
    :
      Settings(organization, application, parent),
      driverName_(driverName)
{
    values.clear();
    read();
}


//-----------------------------------------------------------------------------

QStringList
DriverSettings::names() const
{
    QSettings s(getOrgName(), getAppName());
    s.beginGroup(driverName_);
    QStringList res = s.allKeys();

    return res;
}

//-----------------------------------------------------------------------------

void
DriverSettings::read()
{
    QSettings s(getOrgName(), getAppName());

    s.beginGroup(driverName_);

    QStringList keys = s.allKeys();

    QString key;
    foreach (key, keys)
        values[key] = s.value(key);

    s.endGroup();
}

//-----------------------------------------------------------------------------

void
DriverSettings::save() const
{
    QSettings s(getOrgName(), getAppName());

    s.beginGroup(driverName_);

    QMapIterator<QString, QVariant> i(values);
    while (i.hasNext())
    {
        i.next();
        s.setValue(i.key(), i.value());
    }

    s.endGroup();
}

//-----------------------------------------------------------------------------

}
