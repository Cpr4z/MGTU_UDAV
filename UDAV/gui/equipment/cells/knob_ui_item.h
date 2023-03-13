#ifndef KNOB_UI_ITEM_H
#define KNOB_UI_ITEM_H

#include <QWidget>
#include <QLabel>

#include <qwt_knob.h>

#include "abstract_ui_cell.h"

class QwtKnob;
class QLabel;

namespace udav_gui
{

class KnobUiItem : public QWidget, public AbstractUiCell
{
    Q_OBJECT
    public:
        explicit KnobUiItem(const QString& title = QString(), QWidget *parent = 0);

        virtual void setGridStep(int newStep)
        {
            AbstractUiCell::setGridStep(newStep);
            knob_->setKnobWidth(xSize_ * gridStep_ * ScaleFactor_);
        }

        virtual void setGridGeometry(int x, int y, int xsize, int ysize, int step)
        {
            AbstractUiCell::setGridGeometry(x, y, xsize, ysize, step);
            knob_->setKnobWidth(xSize_ * gridStep_ * ScaleFactor_);
        }

        void setTitle(const QString& title)
        {
            label_->setText(title);
        }

        void setRange(double minValue, double maxValue, double step)
        {
            //knob_->setRange(minValue, maxValue, step);
        }

    protected:
//		virtual void resizeEvent(QResizeEvent *e);

    signals:
        void valueChanged(KnobUiItem* knobUiItem, double value);

    public slots:
        void valueChanged(double value);

    private:
        QwtKnob* knob_;
        QLabel* label_;

        static const double ScaleFactor_;
};

}

#endif // KNOB_UI_ITEM_H
