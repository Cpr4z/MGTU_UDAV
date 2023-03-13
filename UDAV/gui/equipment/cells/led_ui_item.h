#ifndef LED_UI_ITEM_H
#define LED_UI_ITEM_H

#include <QWidget>
#include "abstract_ui_cell.h"

namespace udav_gui
{

class LedUiItem : public QWidget, public AbstractUiCell
{
    Q_OBJECT
    public:
        explicit LedUiItem(QWidget *parent = 0);

        bool getValue() const
        {
            return value_;
        }

        void setValue(bool value)
        {
            value_ = value;
            repaint();
        }

    protected:
        void paintEvent(QPaintEvent* e);


    signals:

    public slots:

    private:
        bool value_;

};

}

#endif // LED_UI_ITEM_H
