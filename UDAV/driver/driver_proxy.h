#ifndef UDAV_DRIVER_PROXY_H
#define UDAV_DRIVER_PROXY_H

#include <QObject>

#include "elvactools/logger.h"

#include "drivers.h"
#include "meta_signal.h"

namespace udav_drivers
{

/**
 * @brief The DriverProxy class - a middle layer between ui and driver.
 *
 * This class inherited by classes DriverLocalProxy to connect with local
 * equipment and DriverNetworkProxy to connect with remote equipment.
 */
class DriverProxy : public QObject
{
    Q_OBJECT
    ELVACTOOLS_LOGGER_SYSTEM_DECLARE

    public:
        explicit DriverProxy(elvactools::Logger* logger, QObject *parent = nullptr);

    public slots:
        /**
         * @brief Обработать изменение значения сигнала управления от интерфейса пользователя
         * @param signalID - номер сигнала
         * @param value - значение сигнала
         */
        void onControlSignalChanged(SignalID signalID, const QVariant& value);


    signals:
        /**
         * @brief Сообщает о получении с сервера измененного значения сигнала
         * @param id - номер сигнала
         * @param value - значение сигнала
         * @param message - текстовое сообщение
         */
        void signalValueChanged(SignalID signalID, const QVariant& value, const QString& message = QString());

    protected:

        MetaSignals metaSignals_;

};

} // namespace udav_drivers


#endif // UDAV_DRIVER_PROXY_H
