#include "coils_table_model.h"

//------------------------------------------------------------------------------

CoilsTableModel::CoilsTableModel(QMap<quint16, bool>* coilsData, QObject *parent)
    : QAbstractTableModel{parent},
      coilsData_(coilsData)
{
}

//------------------------------------------------------------------------------

int
CoilsTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 2;
};

//------------------------------------------------------------------------------

QVariant
CoilsTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    switch (role)
    {
        case Qt::TextAlignmentRole :
            return int(Qt::AlignRight | Qt::AlignVCenter);
        break;
        case Qt::DisplayRole :
            //if coilsData_.contains()
        break;
    }

    return QVariant();
}

//------------------------------------------------------------------------------

QVariant
CoilsTableModel::headerData(int section, Qt::Orientation /*orientation*/, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (section)
        {
            case 0 :
                return QVariant(tr("Address"));
            break;
            case 1 :
                return QVariant(tr("Value"));
            break;
        }
    }

    return QVariant();
}

//------------------------------------------------------------------------------

Qt::ItemFlags
CoilsTableModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.column() > 0)
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

//------------------------------------------------------------------------------

int CoilsTableModel::rowCount(const QModelIndex& parent) const
{
    return coilsData_->count();
}

//------------------------------------------------------------------------------

bool
CoilsTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{

}

//------------------------------------------------------------------------------
// EOF
