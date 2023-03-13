#ifndef SVG_IMAGE_UI_ITEM_H
#define SVG_IMAGE_UI_ITEM_H

#include <QWidget>
#include <QSvgWidget>
#include "abstract_ui_cell.h"

namespace udav_gui
{

/**
  Элемент отображения картинки привязывается к номеру сигнала. Номер картинки передается в сигнале.
  Имя файла для отображения формируется как signal-NNN-XXX, где NNN - номер сигнала, XXX - номер картинки.
*/


class SvgImageUiItem : public QWidget, public AbstractUiCell
{
    Q_OBJECT
//    LOG_MESSENGER

    public:
        explicit SvgImageUiItem(QWidget *parent = 0);

        QString getFileName() const
        {
            return fileName_;
        }

        void setFileName(const QString& fileName);

    protected:
        void paintEvent(QPaintEvent* e);

    signals:

    public slots:

    private:
        QString fileName_;
        QSvgWidget* svgWidget_;

    signals: // Система записи лог-файлов

        /**
         * @brief Отправляет сообщение о критической ошибке
         * @param message - текст сообщения
         */
        void criticalMessage(const QString& message) const;

        /**
         * @brief Отправляет отладочное сообщение
         * @param message - текст сообщения
         */
        void debugMessage(const QString& message) const;

        /**
         * @brief Отправляет информационное сообщение
         * @param message - текст сообщения
         */
        void infoMessage(const QString& message) const;

};

}
#endif // SVG_IMAGE_UI_ITEM_H
