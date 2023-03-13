#ifndef STUB_IMPL_H
#define STUB_IMPL_H
//      ^^^^^^
//
// Step 1. Select name for a new driver
//
// Do not foget rename define for real driver name!
// If not renamed it can provide very strange results!
//

#include <QObject>
#include "driver_impl.h"

// Rename classes and files, replace Stub with selected name.

namespace udav_drivers {

/**
 * @brief The StubImpl class - implementation for stub driver
 */
class StubImpl: public DriverImpl
{
    Q_OBJECT

    friend class Stub;

    public :

        /**
         * @brief userInterface
         * @return
         */
        virtual QString userInterface() const override;


        /**
         * @brief StubImpl
         * @param metaSignals
         * @param settings
         * @param parent
         * @param simulate
         */
        explicit StubImpl(
                const MetaSignals& metaSignals,
                const DriverSettings& settings,
                DriverInterface* driverInterface,
                bool simulate,
                elvactools::Logger* logger
        );

        /**
         * @brief ~StubImpl
         */
        virtual ~StubImpl();

        //! @todo Why it is in public section?
        /**
         * @brief initSettings_
         * @param messageWindow
         * @return
         */
        virtual bool initSettings_(QUuid messageWindow) override;


        /**
         * @brief initDevices
         */
        virtual void initDevices_() override
        {

        }

//        /**
//         * @brief initProcesses
//         */
//        virtual void initProcesses() override
//        {

//        }

        /**
         * @brief initEquipment
         * @return
         */
        virtual bool initEquipment_() override;


    public slots: // process control signals from client

        //! @todo Check which version of methods we are really need

        /**
         * @brief setControlSignal
         * @param signalID
         * @param value
         * @return
         */
        virtual int setControlSignal(udav_drivers::SignalID signalID, bool value);

        /**
         * @brief setControlSignal
         * @return
         */
        virtual int setControlSignalBoolean(SignalID signalID, bool) override
        {
        }

        /**
         * @brief setControlSignalDouble
         * @param signalID
         * @param value
         * @return
         */
        virtual int setControlSignalDouble(SignalID signalID, double value) override
        {
        }

        /**
         * @brief setControlSignalInteger
         * @param signalID
         * @param value
         * @return
         */
        virtual int setControlSignalInteger(SignalID signalID, int value) override
        {
        }


//    /**
//     * @brief The Anonymous:1 struct for signal values internal cache
//     *
//     * We use it as work with signal values realized with QVariant.
//     * QVariant is very usefull for data exchange but slow.
//     */
//    struct
//    {

//        //
//        // Step 2. Declare vars for storing signal values according signals table.
//        //

////        bool isVM1Opened;

////        bool isVM1Closed;
//    } signalValues_;

    private :

        //
        // Step 3. Declare internal fields needed for class functions.
        //

//         modbus4qt::RtuClient* apelDCBus_;

//         bool isPumping_;


        //
        // Step 4. Declare driver settings
        //

        struct stubSettings
        {
//            QString apelBusPort;
//            QSerialPort::BaudRate apelBusBaudRate;
//            QSerialPort::DataBits apelBusDataBits;
//            QSerialPort::StopBits apelBusStopBits;
//            QSerialPort::Parity apelBusParity;
//            int apelBusTimeOut;
        } settings_;


    private slots: // read signal values from equipment

        /**
         * @brief readControlValues_
         */
        virtual void readControlValues_();

        /**
         * @brief readViewValues_
         */
        virtual void readViewValues_();

    private slots: // send real commands to equipment

        //
        // Step 5. Declare and realize methods for real equipment control.
        //

        //bool openTurboValve();
};

}

#endif // STUB_IMPL_H
