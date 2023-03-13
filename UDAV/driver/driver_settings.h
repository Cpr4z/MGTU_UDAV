/**
    $Id: driver_settings.h 920 2016-04-19 06:46:27Z l.kolesnik $
    $URL: https://office.m-i.ru/svn/udav/trunk/driver/driver_settings.h $
*/

#ifndef DRIVER_SETTINGS_H
#define DRIVER_SETTINGS_H

#include <QMap>
#include <QVariant>
#include <QUuid>
#include <QStringList>

#include "drivers_global.h"

#include "elvactools/settings.h"

/*
 * Настройки драйверов группируются по следующему принципу:
 * 1. Общие настройки драйвера
 * 2. Настройки конкретной подсистемы
 */

namespace udav_drivers
{

const QString Simulate = "Simulate";

/**
 * @brief Класс для работы с настройками драйвера оборудования
 *
 * Прочитанные настройки хранятся
 *
 * Место хранения настроек определяется архитектурой платфоры:
 * <ul>
 * <li>для *nix систем - текстовый файл
 * <li>для Windows систем - реестр
 * </ul>
 *
 * Считанные настройки хранятся в карте соответствия Имя(QString)/Значение(QVariant)
 */
class UDAV_DRIVERS_EXPORT DriverSettings : public Settings
{
    Q_OBJECT
    private:
        /**
         * @brief Имя драйвера оборудования
         */
        QString driverName_;

        /**
         * @brief Значения настроек
         */
        QMap<QString, QVariant> values;

    public:

        /**
         * @brief Конструктор по умолчанию
         * @param organization - название организации-разработчика; используется для определения имени места хранения настроек
         * @param application - название програмы; будет используется для определения имени файла хранения настроек
         * @param driverName - название драйвера; будет используется для определения имени секции в файле настроек
         * @param parent - объект-родитель
         *
         * Инициализирует внутренние переменные класса и считывает настройки для драйвера с указанным именем.
         */
        explicit DriverSettings(const QString& organization, const QString& application, const QString& driverName, QObject * parent = 0);

        /**
         * @brief Добавить значение параметра в набор настроек
         * @param name - имя параметра
         * @param value - значение параметра
         */
        inline void addSetting(const QString& name, const QVariant& value);

        /**
         * @brief Определить есть ли в наборе настроек параметр с запрошенным именем
         * @param name - имя параметра
         * @return true, если параметр с запрошенным именем есть в наборе настроек; false в противном случае
         */
        inline bool contains(const QString& name);

        /**
         * @brief Прочитать и вернуть список имен всех параметров
         * @return Список имен параметров
         */
        QStringList names() const;

        /**
         * @brief Прочитать все настройки
         */
        void read();

        /**
         * @brief Записать все настройки
         */
        virtual void save() const;

        /**
         * @brief Вернуть значение параметра для указанного имени
         * @param name - имя параметра
         * @return Значение параметра для указанного имени
         */
        inline QVariant setting(const QString& name) const;

        /**
         * @brief Получить количество параметров настройки
         * @return Количество параметров настройки
         */
        inline int size() const;

        /**
         * @brief Получить значение параметра по его имени
         * @param name - имя параметра
         * @return Значение запрошенного параметра
         */
        inline QVariant operator[](const QString& name) const;
};

//-----------------------------------------------------------------------------

void
DriverSettings::addSetting(const QString& name, const QVariant& value)
{
    values.insert(name, value);
}

//-----------------------------------------------------------------------------

bool
DriverSettings::contains(const QString& name)
{
    return values.contains(name);
}

//-----------------------------------------------------------------------------

QVariant
DriverSettings::setting(const QString& name) const
{
    return values[name];
}

//-----------------------------------------------------------------------------

int
DriverSettings::size() const
{
    return values.size();
}

//-----------------------------------------------------------------------------

QVariant
DriverSettings::operator[](const QString& name) const
{
    return values[name];
}

}

#endif // DRIVER_SETTINGS_H
