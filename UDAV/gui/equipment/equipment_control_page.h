#ifndef EQUIPMENT_CONTROL_PAGE_H
#define EQUIPMENT_CONTROL_PAGE_H

#include <QWidget>

#include "driver/drivers.h"

class QIcon;

namespace Core
{
    class UdavSettings;
}

namespace udav_gui
{

class AbstractUiCell;

/**
 * @brief Класс, отвечающий за отображение страницы панели управления оборудованием
 */
class EquipmentControlPage : public QWidget
{
    Q_OBJECT
    friend class EquipmentUiReader;

    private :

        /**
         * @brief Размер страницы панели в ячейках по горизонтали
         */
        int xSize_;

        /**
         * @brief Размер страницы панели в ячейках по вертикали
         */
        int ySize_;

        /**
         * @brief Шаг сетки для размещения элементов панели
         */
        int gridStep_;

        /**
         * @brief Массив интерфейсных элементов, расположенных на странице
         */
        QVector<AbstractUiCell*> uiItems_;

        /**
         * @brief Иконка страницы. Используется для отображения в левой части панели управления для переключения между страницами.
         */
        QIcon* icon_;

        /**
         * @brief Отображение страницы в режиме отдельного окна
         */
        bool windowMode_;

        Core::UdavSettings* settings_;

    protected:
        /**
         * @brief Обработать событие отрисовки страницы
         */
        virtual void paintEvent(QPaintEvent *);

        /**
         * @brief Обработать событие изменения размера страницы
         * @param e
         *
         * При изменении размера страницы происходит пересчет размера ячейки сетки,
         * используемой для размещения кнопок управления оборудованием. После этого
         * производится пересчет координат интерфейсных элементов, размещенных на сетке.
         */
        virtual void resizeEvent(QResizeEvent *e);

    public:
        /**
         * @brief Конструктор по умолчанию
         * @param parent - объект владелец/родитель
         */
        explicit EquipmentControlPage(Core::UdavSettings* settings, QWidget *parent = 0)
            : QWidget(parent), settings_(settings) {}

        QIcon* icon() const
        {
            return icon_;
        }

        void setIcon(QIcon* icon)
        {
            icon_ = icon;
        }

    signals:

        /**
         * @brief Сообщает об изменении пользователем значения сигнала типа boolean
         */
        void controlSignalChanged(udav_drivers::SignalID, bool);

        /**
         * @brief Сообщает об изменении  пользователем значения сигнала типа double
         */
        void controlSignalChanged(udav_drivers::SignalID, double);

        /**
         * @brief Сообщает об изменении  пользователем значения сигнала типа int
         */
        void controlSignalChanged(udav_drivers::SignalID, qint16);


};

} //namespace Gui


#endif // EQUIPMENT_CONTROL_PAGE_H
