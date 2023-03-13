#include "lcd_ui_cell.h"

#include <QLCDNumber>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QFontMetrics>

#include <QDebug>

const int MarginLeft    = 9;
const int MarginTop     = 0;
const int MarginRight   = 9;
const int MarginBottom  = 9;

namespace udav_gui
{

//-----------------------------------------------------------------------------

LcdUiCell::LcdUiCell(QWidget *parent, const QString& title, bool showTitle, const QString& unit, const QString& style) :
    QWidget(parent), AbstractUiCell(),
    title_(title),
    showTitle_(showTitle),
    unit_(unit),
    style_(style)
{
    if (!title.isEmpty())
        showTitle_ = true;
    else
        showTitle_ = false;

    widget_ = this;

    valueText_ = new QLabel("No data!");
    valueText_->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    valueText_->setStyleSheet(style);

    QGroupBox* groupBox;

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(MarginLeft, MarginTop, MarginRight, 0);

    QHBoxLayout* boxLayout = new QHBoxLayout();
    boxLayout->setContentsMargins(MarginLeft, MarginTop, MarginRight, MarginBottom);

    if (showTitle_)
    {
        groupBox = new QGroupBox;
        groupBox->setTitle(title_);
        //groupBox->setFlat(true);

        groupBox->setLayout(boxLayout);

        mainLayout->addWidget(groupBox);
    }

    if (showTitle_)
    {
        boxLayout->addWidget(valueText_);
    }
    else
    {
        mainLayout->addWidget(valueText_);
    }

    setLayout(mainLayout);
}

//-----------------------------------------------------------------------------

void
LcdUiCell::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);


    QFont font;
    font.setFamily("Droup Sans Mono");
    font.setStyleHint(QFont::Monospace);

    //double scale = 5.0;

//    font.setPointSize(ySize_ * gridStep_ / scale);

//    QFontMetrics fontMetrics(font);

//    if (valueText_->text().isEmpty())
//    {
//        valueText_->setFont(font);
//        return;
//    }

//    int pixelsWide = fontMetrics.width(valueText_->text());
//    double ratio = double(pixelsWide) / double(valueText_->width());

//    if (ratio < 0.9)
//    {
//        do
//        {
//            scale += 0.1;
//            font.setPointSize(ySize_ * gridStep_ / scale);

//            pixelsWide = fontMetrics.width(valueText_->text());
//            ratio = double(pixelsWide) / double(valueText_->width());
//        } while (ratio < 0.9);
//    }
//    else if (ratio > 0.9)
//    {
//        do
//        {
//            scale -= 0.1;
//            qDebug() << "decrease, new scale:" << scale;
//            font.setPointSize(ySize_ * gridStep_ / scale);

//            pixelsWide = fontMetrics.width(valueText_->text());
//            ratio = double(pixelsWide) / double(valueText_->width());
//            qDebug() << "text:" << valueText_->text() << "pixelsWide:" << pixelsWide << "valueText_->width()" << valueText_->width() << "new ratio: " << ratio;
//        } while (ratio > 0.9);
//    }

    valueText_->setFont(font);
}

//-----------------------------------------------------------------------------

void
LcdUiCell::setValue(double value)
{
    QString text = QString("%1 %2").arg(value, 0, 'e', 2).arg(unit_);

    valueText_->setText(text);
}

}
