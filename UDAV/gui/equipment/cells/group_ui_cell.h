#ifndef GROUP_UI_CELL_H
#define GROUP_UI_CELL_H

#include <QGroupBox>
#include "abstract_ui_cell.h"

class QHBoxLayout;

namespace udav_gui
{

class EquipmentControlPage;

class GroupUiCell : public QGroupBox, public AbstractUiCell
{
    friend class EquipmentUiReader;
    Q_OBJECT
    private:
        QHBoxLayout* layout_;

        void setLayout_(EquipmentControlPage* layout);

    public:
        explicit GroupUiCell(QWidget *parent = 0);

    signals:

    public slots:

};

}

#endif // GROUP_UI_CELL_H
