#include "text_message_ui_item.h"

namespace udav_gui
{

TextMessageUiItem::TextMessageUiItem(QWidget *parent) :
    QLabel(parent), AbstractUiCell()
{
    widget_ = this;
}

}
