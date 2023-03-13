#ifndef TEXT_MESSAGE_UI_ITEM_H
#define TEXT_MESSAGE_UI_ITEM_H

#include <QLabel>
#include "abstract_ui_cell.h"

namespace udav_gui
{

class TextMessageUiItem : public QLabel, public AbstractUiCell
{
    Q_OBJECT
    public:
        explicit TextMessageUiItem(QWidget *parent = 0);

        inline void setText(const QString &text = QString());

        inline QString fontName() const;
        inline void setFontName(const QString &fontName);

        inline int fontSize() const;
        inline void setFontSize(int fontSize);

    signals:

    public slots:

    private:
        QString text_;

        /**
         * @brief Имя шрифта, используемого для вывода текста
         */
        QString fontName_;

        /**
         * @brief Размер шрифта, используемого для вывода текста
         */
        int fontSize_;
};


void
TextMessageUiItem::setText(const QString &text)
{
    text_ = text;
    QLabel::setText(text);
}

QString
TextMessageUiItem::fontName() const
{
    return fontName_;
}

void
TextMessageUiItem::setFontName(const QString &fontName)
{
    fontName_ = fontName;

    QFont font(fontName_, fontSize_);
    setFont(font);
}

int
TextMessageUiItem::fontSize() const
{
    return fontSize_;
}

void
TextMessageUiItem::setFontSize(int fontSize)
{
    fontSize_ = fontSize;

    QFont font(fontName_, fontSize_);
    setFont(font);
}

}

#endif // TEXT_MESSAGE_UI_ITEM_H
