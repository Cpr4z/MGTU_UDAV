#include "svg_image_ui_item.h"

#include <QPainter>

#include <QHBoxLayout>
#include <QFile>

namespace udav_gui
{

SvgImageUiItem::SvgImageUiItem(QWidget *parent) :
    QWidget(parent), AbstractUiCell()
{
    widget_ = this;
    fileName_ = "";

    svgWidget_ = new QSvgWidget(this);
    svgWidget_->setVisible(true);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addWidget(svgWidget_);

    setLayout(mainLayout);
}

//-----------------------------------------------------------------------------

void
SvgImageUiItem::paintEvent(QPaintEvent* /*e*/)
{
    bool ok = true;
    if (fileName_.isEmpty())
    {
        emit criticalMessage(tr("SvgImageUiItem::paintEvent(): Image file name is empty!"));
        ok = false;
    }
    else if (!QFile::exists(fileName_))
    {
        emit criticalMessage(tr("File %1 is not exists!").arg(fileName_));
        ok = false;
    }

    if (ok)
    {
        svgWidget_->load(fileName_);
    }
    else
    {
        QPainter painter(this);
        painter.drawRect(1, 1, width()-1, height()-1);

        painter.drawLine(1, 1, width()-1, height()-1);
        painter.drawLine(height()-1, 1, 1, width()-1);
    }
}

//-----------------------------------------------------------------------------

void
SvgImageUiItem::setFileName(const QString& fileName)
{
    fileName_ = fileName;

    repaint();
}

}
