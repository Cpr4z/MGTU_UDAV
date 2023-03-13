#ifndef MODBUS4QT_SERVER_INTERNAL_DATA_H
#define MODBUS4QT_SERVER_INTERNAL_DATA_H

#include <QObject>

#include <QMap>

namespace modbus4qt {

//!
//! \brief The ServerInternalData class is a data storage for modbus server object.
//!
//! Several server objects can share internal data and allow access to then via
//! different protocols: Modbus-RTU, Modbus-TCP or Modbus-ASCII.
//!
class ServerInternalData : public QObject
{
        Q_OBJECT
    public:

        //!
        //! \brief ServerInternalData - default constructor
        //! \param parent - owner of the object
        //!
        explicit ServerInternalData(QObject *parent = nullptr);

        //!
        //! \brief Add coil into server memory structure and init it with false value
        //! \param index - new coil index
        //!
        //! If coil with index is already exist do nothing.
        //!
        void addCoil(quint16 index);

        //!
        //! \brief Add coils into server memory structure and init them with false value
        //! \param startIndex - start index for adding coils
        //! \param endIndex - and index for addign coils
        //!
        //! If some of adding coils with index is already exist skip.
        //!
        void addCoils(quint16 startIndex, quint16 endIndex);

        //!
        //! \brief Add descrete input into server memory structure and init it with false value
        //! \param index - new descerte input index
        //!
        //! If descrete input with index is already exist do nothing.
        //!
        void addDescreteInput(quint16 index);

        //!
        //! \brief Add descrete inputs into server memory structure and init them with false value
        //! \param startIndex - start index for adding descrete inputs
        //! \param endIndex - and index for addign descrete inputs
        //!
        //! If some of adding descrete input with index is already exist skip.
        //!
        void addDescreteInputs(quint16 startIndex, quint16 endIndex);

        //!
        //! \brief Add holding register into server memory structure and init it with zero
        //! \param index - new holding register index
        //!
        //! If holding register with index is already exist do nothing.
        //!
        void addHoldingRegister(quint16 index);

        //!
        //! \brief Add holding registers into server memory structure and init them with zero
        //! \param startIndex - start index for adding holding registers
        //! \param endIndex - and index for addign holding registers
        //!
        //! If some of adding holding registers with index is already exist skip.
        //!
        void addHoldingRegisters(quint16 startIndex, quint16 endIndex);

        //!
        //! \brief Add input register into server memory structure and init it with zero
        //! \param index - new input register index
        //!
        //! If input register with index is already exist do nothing.
        //!
        void addInputRegister(quint16 index);

        //!
        //! \brief Add input registers into server memory structure and init them with zero
        //! \param startIndex - start index for adding input registers
        //! \param endIndex - and index for addign input registers
        //!
        //! If some of adding input registers with index is already exist skip.
        //!
        void addInputRegisters(quint16 startIndex, quint16 endIndex);

        QMap<quint16, bool>* coils();

    signals:

        void coilAdded(quint16 coilAddress);

    protected:

        //! \brief Values of coils in server memory
        //!
        QMap<quint16, bool> coils_;

        //!
        //! \brief Values of descrete inputs in server memory
        //!
        QMap<quint16, bool> descreteInputs_;

        //!
        //! \brief Values of holding registers in server memory
        //!
        QMap<quint16, quint16> holdingRegisters_;

        //!
        //! \brief Values of input registers in server memory
        //!
        QMap<quint16, quint16> inputRegisters_;
};

} // namespace modbus4qt

#endif // MODBUS4QT_SERVER_INTERNAL_DATA_H
