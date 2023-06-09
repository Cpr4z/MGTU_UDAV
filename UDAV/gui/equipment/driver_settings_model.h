#ifndef DRIVERSETTINGSMODEL_H
#define DRIVERSETTINGSMODEL_H

#include <QAbstractTableModel>

class DriverSettingsModel : public QAbstractTableModel
{
    Q_OBJECT

    private:
        virtual int rowCount(const QModelIndex &parent) const;
        virtual int columnCount(const QModelIndex &parent) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    public:
        DriverSettingsModel();
};

#endif // DRIVERSETTINGSMODEL_H
