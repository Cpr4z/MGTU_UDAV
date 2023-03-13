#ifndef EQUIPMENT_UI_ITEM_H
#define EQUIPMENT_UI_ITEM_H

#include <QWidget>
#include "driver/drivers.h"

namespace udav_gui
{

/**
 * @brief Базовый класс для интерфейсной ячейки элемента панели управления
 */
class AbstractUiCell
{
    protected:
        /**
         * @brief Координата x в координатах сетки
         *
         * Внутри программы координаты отсчитываются от нуля. Пользователь при описании отсчитывает от единицы.
         */
        int xCoord_;

        /**
         * @brief Координата у в координатах сетки
         *
         * Внутри программы координаты отсчитываются от нуля. Пользователь при описании отсчитывает от единицы.
         */
        int yCoord_;

        /**
         * @brief Размер по горизонтали в координатах сетки
         */
        int xSize_;

        /**
         * @brief ySize_ Размер по вертикали в координатах сетки
         */
        int ySize_;

        /**
         * @brief Шаг сетки в пикселях
         */
        int gridStep_;

        /**
         * @brief Связанный элемент интерфейса, с которым взаимодействует пользователь
         */
        QWidget* widget_;

        /**
         * @brief Имя класса интерфейсного элемента для идентификации во время отладки
         */
        QString className_;

        /**
         * @brief Код сигнала, привязанного к интерфейсному элементу
         */
        udav_drivers::SignalID signalId_;

    public:
        explicit AbstractUiCell()
        {
            className_ = QString("Unknown");
        }

        QString getClassName() const
        {
            return className_;
        }

        int getGridStep() const
        {
            return gridStep_;
        }

        udav_drivers::SignalID getSignalId() const
        {
            return signalId_;
        }

        int getXCoord() const
        {
            return xCoord_;
        }

        int getYCoord() const
        {
            return yCoord_;
        }

        int getXSize() const
        {
            return xSize_;
        }

        int getYSize() const
        {
            return xSize_;
        }

        void setClassName(const QString& className)
        {
            className_ = className;
        }

        virtual void setGridGeometry(int x, int y, int xsize, int ysize, int step)
        {
            xCoord_ = x - 1;
            yCoord_ = y - 1;
            xSize_ = xsize;
            ySize_ = ysize;
            gridStep_ = step;
            widget_->setGeometry(xCoord_ * gridStep_, yCoord_ * gridStep_, xSize_ * gridStep_, ySize_ * gridStep_);
        }

        virtual void setGridStep(int newStep)
        {
            gridStep_ = newStep;
            widget_->setGeometry(xCoord_ * gridStep_, yCoord_ * gridStep_, xSize_ * gridStep_, ySize_ * gridStep_);
        }

        void setXCoord(int x)
        {
            xCoord_ = x - 1;
            widget_->setGeometry(xCoord_ * gridStep_, yCoord_ * gridStep_, xSize_ * gridStep_, ySize_ * gridStep_);
        }

        void setYCoord(int y)
        {
            yCoord_ = y - 1;
            widget_->setGeometry(xCoord_ * gridStep_, yCoord_ * gridStep_, xSize_ * gridStep_, ySize_ * gridStep_);
        }

        void setXSize(int size)
        {
            xSize_ = size;
            widget_->setGeometry(xCoord_ * gridStep_, yCoord_ * gridStep_, xSize_ * gridStep_, ySize_ * gridStep_);
        }

        void setYSize(int size)
        {
            ySize_ = size;
            widget_->setGeometry(xCoord_ * gridStep_, yCoord_ * gridStep_, xSize_ * gridStep_, ySize_ * gridStep_);
        }

        inline virtual void setSignalId(udav_drivers::SignalID signalId);

};

void AbstractUiCell::setSignalId(udav_drivers::SignalID signalId)
{
    signalId_ = signalId;
}

}

#endif // EQUIPMENT_UI_ITEM_H
