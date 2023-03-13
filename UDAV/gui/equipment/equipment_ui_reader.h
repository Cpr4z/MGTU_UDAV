#ifndef EQUIPMENT_UI_READER_H
#define EQUIPMENT_UI_READER_H

#include <QObject>
#include <QString>
#include <QXmlStreamReader>

#include "driver/meta_signal.h"

#include "elvactools/logger.h"

class QTabWidget;
class QMenu;

namespace Core
{
    class UdavSettings;
}

namespace udav_gui
{

class EquipmentControlPanel;
class EquipmentControlPage;
class PushButtonUiCell;

/**
 * @brief Класс обеспечивает чтение и восстановление интерфейса панели управления оборудованием из XML файла.
 */
class EquipmentUiReader : public QObject
{
    Q_OBJECT
    ELVACTOOLS_LOGGER_SYSTEM_DECLARE
    private :

        EquipmentControlPanel* controlPanel_;

        QString xml_;

        const udav_drivers::MetaSignals& metasignals_;

        Core::UdavSettings* settings_;

    public :
        EquipmentUiReader(EquipmentControlPanel* panel,
                          Core::UdavSettings* settings,
                          const udav_drivers::MetaSignals& metasignals,
                          const QString& xml,
                          elvactools::Logger* logger);

        bool restore();
    private :

        void readCurcuitButton_(EquipmentControlPage* widget, QXmlStreamReader* xmlReader, int level);

        void readGroup_(EquipmentControlPage* parent, QXmlStreamReader* xmlReader, int level);

        void readImage_(EquipmentControlPage* widget, QXmlStreamReader* xmlReader, int level);

        void readKnob_(EquipmentControlPage* widget, QXmlStreamReader* xmlReader, int level);

        /**
         * @brief Прочитать из файла описание расположения интерфейсных элементов и создать соответстующий виджет
         * @param xmlReader - класс чтения данных из файла
         * @return Указатель на созданный виджет
         */
        EquipmentControlPage* readLayout_(QXmlStreamReader* xmlReader, bool isPage, int level);

        void readLcd_(EquipmentControlPage* widget, QXmlStreamReader* xmlReader, int level);

        void readLed_(EquipmentControlPage* widget, QXmlStreamReader* xmlReader, int level);

        void readPage_(QXmlStreamReader* xmlReader, int level);

        void readPushButton_(EquipmentControlPage* widget, QXmlStreamReader* xmlReader, int level);

        /**
         * @brief readText_
         * @param parent
         * @param xmlReader
         * @param level
         */
        void readTextLabel_(EquipmentControlPage* parent, QXmlStreamReader* xmlReader, int level);

        void readTextMessage_(EquipmentControlPage* parent, QXmlStreamReader* xmlReader, int level);

        /**
         * @brief Прочитать из файла описание элемента редактирования значения и создать его
         * @param parent - страница панели управления, на которой будет создан элемент
         * @param xmlReader - класс чтения данных из файла
         */
        void readValueEdit_(EquipmentControlPage* parent, QXmlStreamReader* xmlReader, int level);
};

struct ImageFormats
{
    static const QString svg;
};


struct XMLTags
{
    //------------------------------------------------------
    // Типы интерфейсных элементов панели управления

    static const QString circuitButton;
    static const QString group;
    static const QString image;
    static const QString knob;
    static const QString lcd;
    static const QString led;
    static const QString pushButton;
    static const QString svgImage;
    static const QString valueEdit;

    //------------------------------------------------------
    // Вложенный виджет для элемента group

    static const QString layout;

    //------------------------------------------------------
    // Атрибуты для элементов

    static const QString name;
    static const QString mode;
    static const QString title;
    static const QString showTitle;
    static const QString xSize;
    static const QString ySize;

    //------------------------------------------------------
    // Предопределенные значения атрибутов

    static const QString window;

    //------------------------------------------------------

    static const QString axis;
    static const QString bottom;
    static const QString buttonClass;
    static const QString channel;
    static const QString connection;
    static const QString driver;
    static const QString enableIf;
    static const QString format;
    static const QString item;
    static const QString left;
    static const QString log;
    static const QString menu;
    static const QString maxValue;
    static const QString minValue;
    static const QString offQuestion;
    static const QString onQuestion;
    static const QString page;
    static const QString plot;
    static const QString right;
    static const QString signalID;
    static const QString signalValue;
    static const QString step;
    static const QString style;
    static const QString text;
    static const QString textLabel;
    static const QString textPosition;
    static const QString top;
    static const QString unit;
    static const QString value;
    static const QString ventBotton;
    static const QString x;
    static const QString y;
};

} // namespace Gui


#endif // EQUIPMENT_UI_READER_H
