#ifndef COILSTABLEMODEL_H
#define COILSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMap>

class CoilsTableModel : public QAbstractTableModel
{
        Q_OBJECT
    public:
        explicit CoilsTableModel(QMap<quint16, bool>* coilsData, QObject *parent = nullptr);

        int columnCount(const QModelIndex&) const override;

        QVariant data(const QModelIndex& index, int role) const override;

        QVariant headerData(int section, Qt::Orientation, int role) const override;

        Qt::ItemFlags flags(const QModelIndex& index) const override;

        int rowCount(const QModelIndex& parent) const override;

        bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    protected:

        QMap<quint16, bool>* coilsData_;
};

#endif // COILSTABLEMODEL_H
