#include "image_ui_item.h"
#include "elvactools/utils.h"
//#include "udavc.h"

#include <QPainter>
#include <QImage>
#include <QFile>

namespace udav_gui
{

ImageUiItem::ImageUiItem(QWidget *parent) :
    QWidget(parent), AbstractUiCell()
{
    widget_ = this;
    fileName_ = "";
}

//-----------------------------------------------------------------------------

void
ImageUiItem::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    bool ok = true;
    if (fileName_.isEmpty())
    {
        emit criticalMessage(tr("ImageUiItem::paintEvent(): Image file name is empty!"));
        ok = false;
    }
    else if (!QFile::exists(fileName_))
    {
        emit criticalMessage(tr("File %1 is not exists!").arg(fileName_));
        ok = false;
    }

    if (ok)
    {
        QImage image;
        image.load(fileName_);

        if (image.isNull())
            emit criticalMessage(tr("Null image file: %1").arg(fileName_));

        painter.drawImage(0, 0, image);
    }
    else
    {
        painter.drawRect(1, 1, width()-1, height()-1);

        painter.drawLine(1, 1, width()-1, height()-1);
        painter.drawLine(height()-1, 1, 1, width()-1);
    }
}

//-----------------------------------------------------------------------------

void
ImageUiItem::setFileName(const QString& fileName)
{
    fileName_ = fileName;

    repaint();
}

}
