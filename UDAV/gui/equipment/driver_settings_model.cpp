#include "driver_settings_model.h"


DriverSettingsModel::DriverSettingsModel()
{

}

int
DriverSettingsModel::rowCount(const QModelIndex& parent) const
{
    return 5;
}

int
DriverSettingsModel::columnCount(const QModelIndex& parent) const
{
    return 3;
}

QVariant
DriverSettingsModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole) {
        QString unswer = QString("row = ") + QString::number(index.row()) + "  col = " + QString::number(index.column());
        return QVariant(unswer);
    }
    return QVariant();
}

QVariant
DriverSettingsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0 : return QVariant(tr("Name")); break;
            case 1 : return QVariant(tr("Type")); break;
            case 2 : return QVariant(tr("Value")); break;
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
