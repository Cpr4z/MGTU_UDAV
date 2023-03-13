#include "equipment_ui_reader.h"
#include "equipment_control_panel.h"
#include "equipment_control_page.h"

#include "cells/image_ui_item.h"
#include "cells/group_ui_cell.h"
#include "cells/knob_ui_item.h"
#include "cells/lcd_ui_cell.h"
#include "cells/led_ui_item.h"
#include "cells/push_button_ui_cell.h"
#include "cells/svg_image_ui_item.h"
#include "cells/text_message_ui_item.h"  // < ?
#include "cells/value_edit_ui_cell.h"

#include "cells/circuit/circuit_push_button_ui_cell.h"
#include "cells/circuit/vent_button_ui_cell.h"
#include "cells/circuit/pipe_button_ui_cell.h"
#include "cells/circuit/pump_button_ui_cell.h"
#include "cells/circuit/turbo_pump_button.h"
#include "cells/circuit/vaccum_chamber_button.h"
#include "cells/circuit/vaccum_sensor_button_ui_cell.h"
#include "cells/circuit/leak_button_ui_cell.h"

#include "elvactools/utils.h"
//#include "udavc.h"
#include "driver/drivers.h"

//#include <auto_ptr.h>

#include <QtCore>
#include <QtGui>

namespace udav_gui
{

//------------------------------------------------------
// Типы интерфейсных элементов панели управления

const QString XMLTags::circuitButton    = "circuitButton";
const QString XMLTags::group            = "group";
const QString XMLTags::image            = "image";
const QString XMLTags::knob             = "knob";
const QString XMLTags::lcd              = "lcd";
const QString XMLTags::led              = "led";
const QString XMLTags::pushButton       = "pushButton";
const QString XMLTags::svgImage         = "svgImage";
const QString XMLTags::text             = "text";
const QString XMLTags::textLabel        = "textLabel";
const QString XMLTags::valueEdit        = "valueEdit";

//------------------------------------------------------
// Вложенный виджет для элемента group

const QString XMLTags::layout           = "layout";

//------------------------------------------------------
// Атрибуты для элементов                               // Где используется

const QString XMLTags::name             = "name";       // page, layout
const QString XMLTags::mode             = "mode";       // page
const QString XMLTags::title			= "title";      // page, layout
const QString XMLTags::showTitle        = "showTitle";  // lcd
const QString XMLTags::xSize			= "xSize";      // page, layout
const QString XMLTags::ySize			= "ySize";      // page, layout

//------------------------------------------------------
// Предопределенные значения атрибутов

const QString XMLTags::window           = "window"; // mode@page

// button classes

const QString XMLTags::buttonClass		= "buttonClass";
const QString XMLTags::ventBotton       = "ventButton";

//------------------------------------------------------
// Прочее..., пока не разобрано...

const QString XMLTags::axis             = "axis";
const QString XMLTags::bottom			= "bottom";
const QString XMLTags::channel			= "channel";
const QString XMLTags::connection		= "connection";
const QString XMLTags::driver			= "driver";
const QString XMLTags::enableIf			= "enableIf";
const QString XMLTags::format			= "format";
const QString XMLTags::item				= "item";
const QString XMLTags::left             = "left";
const QString XMLTags::log              = "log";
const QString XMLTags::maxValue			= "maxValue";
const QString XMLTags::menu             = "menu";
const QString XMLTags::minValue			= "minValue";
const QString XMLTags::offQuestion      = "offQuestion";
const QString XMLTags::onQuestion       = "onQuestion";
const QString XMLTags::page             = "page";
const QString XMLTags::plot             = "plot";
const QString XMLTags::right			= "right";
const QString XMLTags::signalID			= "signalID";
const QString XMLTags::signalValue		= "signalValue";
const QString XMLTags::step             = "step";
const QString XMLTags::style            = "style";
const QString XMLTags::textPosition     = "textPosition";
const QString XMLTags::top				= "top";
const QString XMLTags::unit             = "unit";
const QString XMLTags::value			= "value";
const QString XMLTags::x                = "x";
const QString XMLTags::y                = "y";

const QString ImageFormats::svg			= "svg";

//-----------------------------------------------------------------------------

QString
getLevelIndent(int level)
{
    QString result;

    for (int i = 0; i < level; ++i)
        result += QString("\t");

    return result;
}

//-----------------------------------------------------------------------------

QString
removeSpacesAndNewLines(const QString& str)
{
    QString result;

    for (int i = 0; i < str.size(); ++i)
    {
        QChar c = str.at(i);
        if ((!c.isSpace()) && (c != QChar('\n')))
            result += c;
    }
    return result;
}

//-----------------------------------------------------------------------------

QString
restoreNewLines(const QString& str)
{
    QString result = str;

    result.replace(QString("\\n"), QChar('\n'));
    return result;
}

//-----------------------------------------------------------------------------

EquipmentUiReader::EquipmentUiReader(EquipmentControlPanel* panel,
                                     Core::UdavSettings* settings,
                                     const udav_drivers::MetaSignals& metasignals,
                                     const QString& xml,
                                     elvactools::Logger* logger) :
    controlPanel_(panel),
    xml_(xml),
    metasignals_(metasignals),
    settings_(settings)
{
    ELVACTOOLS_LOGGER_SYSTEM_CONNECT(this, logger)
}


//-----------------------------------------------------------------------------

void
EquipmentUiReader::readCurcuitButton_(EquipmentControlPage* widget, QXmlStreamReader* xmlReader, int level)
{
    int curLevel = level + 1;
    int tagsLevel = level + 2;

    QString name = xmlReader->attributes().value(XMLTags::name).toString();

    emit debugMessage(getLevelIndent(level) + tr("Restoring button: %1").arg(name));

    // Читаем имя класса кнопки мнемосхемы
    QString itemClass = xmlReader->attributes().value(XMLTags::buttonClass).toString();
    emit debugMessage(getLevelIndent(curLevel) + tr("Item class is %1").arg(itemClass));

    QString onQuestion, offQuestion;

    bool onQuestionOn = false, offQuestionOn = false;

    int x = 0, y = 0, xSize = 0, ySize = 0;
    quint8 connection = 0, textPos = 0;

    udav_drivers::SignalID signalID = 0;

    // Читаем параметры кнопки мнемосхемы
    xmlReader->readNext();
    while (!xmlReader->atEnd())
    {
        if (xmlReader->isEndElement())
        {
            xmlReader->readNext();
            break;
        }
        if (xmlReader->isStartElement())
        {
            QStringRef tagName = xmlReader->name();
            if (tagName == XMLTags::x)
            {
                x = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("x=%1").arg(x));
            }
            else if (tagName == XMLTags::y)
            {
                y = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("y=%1").arg(y));
            }
            else if (tagName == XMLTags::xSize)
            {
                xSize = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("xSize=%1").arg(xSize));
            }
            else if (tagName == XMLTags::ySize)
            {
                ySize = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("ySize=%1").arg(ySize));
            }
            else if (tagName == XMLTags::connection)
            {
                QString conStr = Core::readTextFieldFromXML(xmlReader);
                if (conStr == XMLTags::bottom)
                {
                    emit debugMessage(getLevelIndent(tagsLevel) + tr("Connection: bottom"));
                    connection |= CircuitPushButtonUiCell::Position::Bottom;
                }
                else if (conStr == XMLTags::right)
                {
                    emit debugMessage(getLevelIndent(tagsLevel) + tr("Connection: right"));
                    connection |= CircuitPushButtonUiCell::Position::Right;
                }
                else if (conStr == XMLTags::top)
                {
                    emit debugMessage(getLevelIndent(tagsLevel) + tr("Connection: top"));
                    connection |= CircuitPushButtonUiCell::Position::Top;
                }
                else if (conStr == XMLTags::left)
                {
                    emit debugMessage(getLevelIndent(tagsLevel) + tr("Connection: left"));
                    connection |= CircuitPushButtonUiCell::Position::Left;
                }
            }
            else if (tagName == XMLTags::textPosition)
            {
                QString textPosStr = Core::readTextFieldFromXML(xmlReader);
                if (textPosStr == XMLTags::bottom)
                {
                    emit debugMessage(getLevelIndent(tagsLevel) + tr("Text position: bottom"));
                    textPos |= CircuitPushButtonUiCell::Position::Bottom;
                }
                else if (textPosStr == XMLTags::right)
                {
                    emit debugMessage(getLevelIndent(tagsLevel) + tr("Text position: right"));
                    textPos |= CircuitPushButtonUiCell::Position::Right;
                }
                else if (textPosStr == XMLTags::top)
                {
                    emit debugMessage(getLevelIndent(tagsLevel) + tr("Text position: top"));
                    textPos |= CircuitPushButtonUiCell::Position::Top;
                }
                else if (textPosStr == XMLTags::left)
                {
                    emit debugMessage(getLevelIndent(tagsLevel) + tr("Text position: left"));
                    textPos |= CircuitPushButtonUiCell::Position::Left;
                }
            }
            else if (tagName == XMLTags::signalID)
            {
                signalID = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + tr("SignalID=%1").arg(signalID));
            }
            else if (tagName == XMLTags::onQuestion)
            {
                onQuestion = xmlReader->attributes().value(XMLTags::text).toString();
                onQuestion = Core::getLocaleString(onQuestion);
                onQuestionOn = Core::readBoolFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("onQuestion=%1; question is '%2'").arg(onQuestionOn).arg(onQuestion));
            }
            else if (tagName == XMLTags::offQuestion)
            {
                offQuestion = xmlReader->attributes().value(XMLTags::text).toString();
                offQuestion = Core::getLocaleString(offQuestion);
                offQuestionOn = Core::readBoolFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("offQuestion=%1; question is '%2'").arg(offQuestionOn).arg(offQuestion));
            }
            else
                Core::skipUnknownElement(xmlReader);
        }
        else xmlReader->readNext();
    }

    if (textPos & CircuitPushButtonUiCell::Position::Top
            || textPos & CircuitPushButtonUiCell::Position::Bottom)
    {
        // Текст сверху или снизу. В этом случае кнопка должна быть 1(ш)*2(в)
        // Если это не так, выдаем предупреждение
        if ((ySize / xSize) != 2.0)
            emit infoMessage(getLevelIndent(curLevel) + tr("For button with text position top or bottom recommended size 1w*2h"));
    }
    else if (textPos & CircuitPushButtonUiCell::Position::Left
             || textPos & CircuitPushButtonUiCell::Position::Right)
    {
        // Текст сверху или снизу. В этом случае кнопка должна быть 2(ш)*1(в)
        // Если это не так, выдаем предупреждение
        if ((xSize / ySize) != 2.0)
            emit infoMessage(getLevelIndent(curLevel) + tr("For button with text position left or right recommended size 2w*1h"));
    }

    // Создаем интерфейсный элемент кнопки мнемосхемы
    CircuitPushButtonUiCell* btn = 0;
    if (itemClass == CircuitButtonClassNames::Leak)
        btn = new LeakButtonUiCell(widget);
    else if (itemClass == CircuitButtonClassNames::Pipe)
        btn = new PipeButtonUiCell(widget);
    else if (itemClass == CircuitButtonClassNames::Pump)
        btn = new PumpButtonUiCell(widget);
    else if (itemClass == CircuitButtonClassNames::TurboPump)
        btn = new TurboPumpButton(widget);
    else if (itemClass == CircuitButtonClassNames::VacuumChamber)
        btn = new VacuumChamberButton(widget);
    else if (itemClass == CircuitButtonClassNames::Sensor)
        btn = new VacuumSensorButtonUiCell(widget);
    else if (itemClass == CircuitButtonClassNames::Vent)
        btn = new VentButtonUiCell(widget);
    else
        emit debugMessage(getLevelIndent(curLevel) + tr("Cannot create %1 item. Wrong item class").arg(itemClass));

    if (btn)
    {
        emit debugMessage(getLevelIndent(curLevel) + tr("Created %1 item. Adding it to widget...").arg(itemClass));

        btn->setObjectName(name);
        btn->setText(name);

        btn->setConnection(connection);
        btn->setTextPos(textPos);

        btn->setGridGeometry(x, y, xSize, ySize, widget->gridStep_);
        btn->setVisible(true);

        btn->setSignalId(signalID);

        btn->setOnQuestionOn(onQuestionOn);
        btn->setOnQuestion(onQuestion);

        btn->setOffQuestionOn(offQuestionOn);
        btn->setOffQuestion(offQuestion);

        btn->setClassName(XMLTags::circuitButton);

        widget->uiItems_.append(btn);
        widget->update();

        emit debugMessage(getLevelIndent(curLevel) + tr("Item added to widget"));

        controlPanel_->ui2signal_[btn] = signalID;
        controlPanel_->signal2ui_[signalID] = btn;

        QObject::connect(btn, SIGNAL(controlSignalChanged(Drivers::SignalID, QVariant)), controlPanel_, SIGNAL(controlSignalChanged(Drivers::SignalID, QVariant)));

        emit debugMessage(getLevelIndent(level) + tr("Restored button: %1").arg(name));
    }
    else
    {
        emit debugMessage(getLevelIndent(level) + tr("Cannot allocate button"));
    }
}

//-----------------------------------------------------------------------------

void
EquipmentUiReader::readGroup_(EquipmentControlPage *parent, QXmlStreamReader *xmlReader, int level)
{
    int tagsLevel = level + 1;

    QString name = xmlReader->attributes().value(XMLTags::name).toString();
    QString title = xmlReader->attributes().value(XMLTags::title).toString();
    title = Core::getLocaleString(title);

    emit debugMessage(getLevelIndent(level) + tr("Restoring group '%1' with title '%2'").arg(name).arg(title));

    int x = 0, y = 0, xSize = 0, ySize = 0;

    // Создаем интерфейсный элемент
    GroupUiCell* groupUiCell = new GroupUiCell(parent);
    groupUiCell->setTitle(title);

    // Читаем параметры
    xmlReader->readNext();
    while (!xmlReader->atEnd())
    {
        if (xmlReader->isEndElement())
        {
            xmlReader->readNext();
            break;
        }
        if (xmlReader->isStartElement())
        {
            QStringRef tagName = xmlReader->name();
            if (tagName == XMLTags::x)
            {
                x = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("x=%1").arg(x));
            }
            else if (tagName == XMLTags::y)
            {
                y = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("y=%1").arg(y));
            }
            else if (tagName == XMLTags::xSize)
            {
                xSize = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("xSize=%1").arg(xSize));
            }
            else if (tagName == XMLTags::ySize)
            {
                ySize = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("ySize=%1").arg(ySize));
            }
            else if (tagName == XMLTags::layout)
            {
                EquipmentControlPage* widget = readLayout_(xmlReader, false, level + 1);

                groupUiCell->setLayout_(widget);
            }
            else
                Core::skipUnknownElement(xmlReader);
        }
        else xmlReader->readNext();
    }

    groupUiCell->setGridGeometry(x, y, xSize, ySize, parent->gridStep_);
    groupUiCell->setVisible(true);

    parent->uiItems_.append(groupUiCell);
    parent->update();

    emit debugMessage(getLevelIndent(level) + tr("Restored group '%1' with title '%2'").arg(name).arg(title));
}

//-----------------------------------------------------------------------------

void
EquipmentUiReader::readImage_(EquipmentControlPage* widget, QXmlStreamReader* xmlReader, int level)
{
    int x = 0, y = 0, xSize = 0, ySize = 0;
    udav_drivers::SignalID signalId = 0;
    QString format = QString("");

    QString name = xmlReader->attributes().value(XMLTags::name).toString();

    // Читаем параметры элемента отображения картинки
    xmlReader->readNext();
    while (!xmlReader->atEnd())
    {
        if (xmlReader->isEndElement())
        {
            xmlReader->readNext();
            break;
        }
        if (xmlReader->isStartElement())
        {
            QStringRef tagName = xmlReader->name();
            if (tagName == XMLTags::x)
                x = Core::readIntFieldFromXML(xmlReader);
            else if (tagName == XMLTags::y)
                y = Core::readIntFieldFromXML(xmlReader);
            else if (tagName == XMLTags::xSize)
                xSize = Core::readIntFieldFromXML(xmlReader);
            else if (tagName == XMLTags::ySize)
                ySize = Core::readIntFieldFromXML(xmlReader);
            else if (tagName == XMLTags::signalID)
                signalId = Core::readIntFieldFromXML(xmlReader);
            else if (tagName == XMLTags::format)
                format = Core::readTextFieldFromXML(xmlReader);
            else
                Core::skipUnknownElement(xmlReader);
        }
        else xmlReader->readNext();
    }

    if (format == ImageFormats::svg)
    {
        // Создаем интерфейсный элемент отображения SVG картинки
        SvgImageUiItem* svgImageUiItem = new SvgImageUiItem(widget);
        svgImageUiItem->setGridGeometry(x, y, xSize, ySize, widget->gridStep_);
        svgImageUiItem->setVisible(true);
        svgImageUiItem->setClassName(XMLTags::svgImage);

        widget->uiItems_.append(svgImageUiItem);
        widget->update();

        controlPanel_->ui2signal_[svgImageUiItem] = signalId;
        controlPanel_->signal2ui_[signalId] = svgImageUiItem;
    }
    else
    {
        // Создаем интерфейсный элемент отображения растровой картинки
        ImageUiItem* imageUiItem = new ImageUiItem(widget);
        imageUiItem->setGridGeometry(x, y, xSize, ySize, widget->gridStep_);
        imageUiItem->setVisible(true);
        imageUiItem->setClassName(XMLTags::image);

        widget->uiItems_.append(imageUiItem);
        widget->update();

        controlPanel_->ui2signal_[imageUiItem] = signalId;
        controlPanel_->signal2ui_[signalId] = imageUiItem;
    }
}


//-----------------------------------------------------------------------------

void
EquipmentUiReader::readKnob_(EquipmentControlPage* widget, QXmlStreamReader* xmlReader, int level)
{
    int x = 0, y = 0, xSize = 0, ySize = 0;
    double minValue = 0.0, maxValue = 0.0, step = 0.0;
    udav_drivers::SignalID signalId = 0;

    QString name = xmlReader->attributes().value(XMLTags::name).toString();
    QString title = xmlReader->attributes().value(XMLTags::title).toString();
    title = Core::getLocaleString(title);

    bool enablerOk = false;
    udav_drivers::SignalID enablerId = 0;
    int enablerValue = 0;

    xmlReader->readNext();
    while (!xmlReader->atEnd())
    {
        if (xmlReader->isEndElement())
        {
            xmlReader->readNext();
            break;
        }
        if (xmlReader->isStartElement())
        {
            QStringRef tagName = xmlReader->name();
            if (tagName == XMLTags::x)
                x = Core::readIntFieldFromXML(xmlReader);
            else if (tagName == XMLTags::y)
                y = Core::readIntFieldFromXML(xmlReader);
            else if (tagName == XMLTags::xSize)
                xSize = Core::readIntFieldFromXML(xmlReader);
            else if (tagName == XMLTags::ySize)
                ySize = Core::readIntFieldFromXML(xmlReader);
            else if (tagName == XMLTags::maxValue)
                maxValue = Core::readDoubleFieldFromXML(xmlReader);
            else if (tagName == XMLTags::minValue)
                minValue = Core::readDoubleFieldFromXML(xmlReader);
            else if (tagName == XMLTags::step)
                step = Core::readDoubleFieldFromXML(xmlReader);
            else if (tagName == XMLTags::signalID)
                signalId = Core::readIntFieldFromXML(xmlReader);
            else if (tagName == XMLTags::enableIf)
            {

                qDebug() << "enableIf!";

                enablerOk = true;
                enablerId = xmlReader->attributes().value(XMLTags::signalID).toString().toInt(&enablerOk);
                enablerValue = xmlReader->attributes().value(XMLTags::signalValue).toString().toInt(&enablerOk);

                xmlReader->readElementText();
                if (xmlReader->isEndElement()) xmlReader->readNext();
            }
            else
                Core::skipUnknownElement(xmlReader);
        }
        else xmlReader->readNext();
    }

    KnobUiItem* knobUiItem = new KnobUiItem(title, widget);
    knobUiItem->setRange(minValue, maxValue, step);
    knobUiItem->setGridGeometry(x, y, xSize, ySize, widget->gridStep_);
    knobUiItem->setVisible(true);
    knobUiItem->setClassName(XMLTags::knob);

    widget->uiItems_.append(knobUiItem);
    widget->update();

    controlPanel_->ui2signal_[knobUiItem] = signalId;
    controlPanel_->signal2ui_[signalId] = knobUiItem;

    if (enablerOk)
    {
        qDebug() << "enableOk! signalID: " << enablerId << ", ";
        UiEnabler enabler(enablerValue, knobUiItem);

        controlPanel_->uiEnbaler[enablerId] = enabler;
    }

    QObject::connect(knobUiItem, SIGNAL(valueChanged(KnobUiItem*,double)), controlPanel_, SLOT(knobValueChanged(KnobUiItem*,double)));
}

//-----------------------------------------------------------------------------

EquipmentControlPage*
EquipmentUiReader::readLayout_(QXmlStreamReader *xmlReader, bool isPage, int level)
{
    int curLevel = level + 1;

    QString type;
    if (isPage)
        type = "page";
    else
        type = "layout";

    EquipmentControlPage* layout = new EquipmentControlPage(settings_, controlPanel_);

    QString name = xmlReader->attributes().value(XMLTags::name).toString();
    QString title = xmlReader->attributes().value(XMLTags::title).toString();
    title = Core::getLocaleString(title);

    emit debugMessage(getLevelIndent(level) + tr("Restoring %3 '%1' with title '%2'").arg(name).arg(title).arg(type));
    layout->setObjectName(name);
    layout->setWindowTitle(title);

    QString xSizeStr = xmlReader->attributes().value(XMLTags::xSize).toString();
    QString ySizeStr = xmlReader->attributes().value(XMLTags::ySize).toString();

    if (isPage)
    {
        QString mode = xmlReader->attributes().value(XMLTags::mode).toString();
        if (mode == XMLTags::window)
        {
            emit debugMessage(getLevelIndent(curLevel) + tr("Mode: Display page in separate window"));
            layout->windowMode_ = true;
        }
        else
        {
            emit debugMessage(getLevelIndent(curLevel) + tr("Mode: Display in control panel"));
            layout->windowMode_ = false;
        }
    }
    else
        layout->windowMode_ = false;

    bool ok = true;
    layout->xSize_ = xSizeStr.toInt(&ok);
    layout->ySize_ = ySizeStr.toInt(&ok);

    xmlReader->readNext();
    while (!xmlReader->atEnd())
    {
        if (xmlReader->isEndElement())
        {
            xmlReader->readNext();
            break;
        }
        if (xmlReader->isStartElement())
        {
            QString nextItemName(xmlReader->name().toString());
            emit debugMessage(getLevelIndent(curLevel) + tr("Next ui item is '%1'").arg(nextItemName));

            if (xmlReader->name() == XMLTags::circuitButton)
                readCurcuitButton_(layout, xmlReader, curLevel + 1);
            else if (xmlReader->name() == XMLTags::group)
                readGroup_(layout, xmlReader, curLevel + 1);
            else if (xmlReader->name() == XMLTags::image)
                readImage_(layout, xmlReader, curLevel + 1);
            else if (xmlReader->name() == XMLTags::knob)
                readKnob_(layout, xmlReader, curLevel + 1);
            else if (xmlReader->name() == XMLTags::lcd)
                readLcd_(layout, xmlReader, curLevel + 1);
            else if (xmlReader->name() == XMLTags::led)
                readLed_(layout, xmlReader, curLevel + 1);
            else if (xmlReader->name() == XMLTags::pushButton)
                readPushButton_(layout, xmlReader, curLevel + 1);
            else if (xmlReader->name() == XMLTags::textLabel)
                readTextLabel_(layout, xmlReader, curLevel + 1);
            else if (xmlReader->name() == XMLTags::valueEdit)
                readValueEdit_(layout, xmlReader, curLevel + 1);
            else if (xmlReader->name() == XMLTags::text) //! @todo: Проверить список доступных элементов
                readTextMessage_(layout, xmlReader, curLevel + 1);
            else
                Core::skipUnknownElement(xmlReader);
        }
        else xmlReader->readNext();
    }
    if (xmlReader->isEndElement()) xmlReader->readNext();

    emit debugMessage(getLevelIndent(level) + tr("Restored %3 '%1' with title '%2'").arg(name).arg(title).arg(type));

    return layout;
}

//-----------------------------------------------------------------------------

void
EquipmentUiReader::readLcd_(EquipmentControlPage* widget, QXmlStreamReader* xmlReader, int level)
{
    int x = 0, y = 0, xSize = 0, ySize = 0;
    udav_drivers::SignalID signalId = 0;

    QString name = xmlReader->attributes().value(XMLTags::name).toString();
    QString title = xmlReader->attributes().value(XMLTags::title).toString();
    title = Core::getLocaleString(title);

    emit debugMessage(getLevelIndent(level) + tr("Restoring LCD: %1; with title: %2").arg(name).arg(title));

    QString style;
    QString unit;
    bool showTitle = true;
    bool unitPresent = false;

    xmlReader->readNext();
    while (!xmlReader->atEnd())
    {
        if (xmlReader->isEndElement())
        {
            xmlReader->readNext();
            break;
        }
        if (xmlReader->isStartElement())
        {
            QStringRef tagName = xmlReader->name();
            if (tagName == XMLTags::x)
            {
                x = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(level + 1) + QString("x=%1").arg(x));
            }
            else if (tagName == XMLTags::y)
            {
                y = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(level + 1) + QString("y=%1").arg(x));
            }
            else if (tagName == XMLTags::xSize)
            {
                xSize = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(level + 1) + QString("xSize=%1").arg(xSize));
            }
            else if (tagName == XMLTags::ySize)
            {
                ySize = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(level + 1) + QString("ySize=%1").arg(ySize));
            }
            else if (tagName == XMLTags::showTitle)
            {
                showTitle = Core::readBoolFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(level + 1) + QString("showTitle=%1").arg(showTitle));
            }
            else if (tagName == XMLTags::style)
            {
                style = Core::readTextFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(level + 1) + QString("style=%1").arg(removeSpacesAndNewLines(style)));
            }
            else if (tagName == XMLTags::unit)
            {
                unitPresent = true;
                unit = Core::readTextFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(level + 1) + QString("unit=%1").arg(unit));
            }
            else if (tagName == XMLTags::signalID)
            {
                signalId = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(level + 1) + QString("signalID=%1").arg(signalId));
            }
            else
                Core::skipUnknownElement(xmlReader);
        }
        else xmlReader->readNext();
    }

    if (unitPresent)
        unit = Core::getLocaleString(unit);
    else
        unit = QString();

    LcdUiCell* lcdUiItem = new LcdUiCell(widget, title, showTitle, unit, style);
    lcdUiItem->setClassName(XMLTags::lcd);

    lcdUiItem->setGridGeometry(x, y, xSize, ySize, widget->gridStep_);
    lcdUiItem->setVisible(true);

    widget->uiItems_.append(lcdUiItem);
    widget->update();

    controlPanel_->ui2signal_[lcdUiItem] = signalId;
    controlPanel_->signal2ui_[signalId] = lcdUiItem;

    emit debugMessage(getLevelIndent(level) + tr("Restored LCD: %1; with title: %2").arg(name).arg(title));
}

//-----------------------------------------------------------------------------

void
EquipmentUiReader::readLed_(EquipmentControlPage* widget, QXmlStreamReader* xmlReader, int level)
{
    int x = 0, y = 0, xSize = 0, ySize = 0;
    udav_drivers::SignalID signalId = 0;

    QString name = xmlReader->attributes().value(XMLTags::name).toString();
    QString title = xmlReader->attributes().value(XMLTags::title).toString();
    title = Core::getLocaleString(title);

    xmlReader->readNext();
    while (!xmlReader->atEnd())
    {
        if (xmlReader->isEndElement())
        {
            xmlReader->readNext();
            break;
        }
        if (xmlReader->isStartElement())
        {
            QStringRef tagName = xmlReader->name();
            if (tagName == XMLTags::x)
                x = Core::readIntFieldFromXML(xmlReader);
            else if (tagName == XMLTags::y)
                y = Core::readIntFieldFromXML(xmlReader);
            else if (tagName == XMLTags::xSize)
                xSize = Core::readIntFieldFromXML(xmlReader);
            else if (tagName == XMLTags::ySize)
                ySize = Core::readIntFieldFromXML(xmlReader);
            else if (tagName == XMLTags::signalID)
                signalId = Core::readIntFieldFromXML(xmlReader);
            else
                Core::skipUnknownElement(xmlReader);
        }
        else xmlReader->readNext();
    }

    LedUiItem* ledUiItem = new LedUiItem(widget);
//    ledUiItem->setRange(minValue, maxValue, step);
    ledUiItem->setGridGeometry(x, y, xSize, ySize, widget->gridStep_);
    ledUiItem->setVisible(true);
    ledUiItem->setClassName(XMLTags::led);

    widget->uiItems_.append(ledUiItem);
    widget->update();

    controlPanel_->ui2signal_[ledUiItem] = signalId;
    controlPanel_->signal2ui_[signalId] = ledUiItem;
}

//-----------------------------------------------------------------------------

void
EquipmentUiReader::readPage_(QXmlStreamReader* xmlReader, int level)
{
    EquipmentControlPage* page = readLayout_(xmlReader, true, level);
    if (!page->windowMode_)
    {
        controlPanel_->addPage_(page);
        emit debugMessage(getLevelIndent(level) + tr("Page will be shown in control panel"));
    }
    else
    {
        controlPanel_->addWindow_(page);
        emit debugMessage(getLevelIndent(level) + tr("Page will be shown in separate window"));
    }
}

//-----------------------------------------------------------------------------

void
EquipmentUiReader::readPushButton_(EquipmentControlPage* widget, QXmlStreamReader* xmlReader, int level)
{
    int curLevel = level;
    int tagsLevel = level + 1;

    QString name = xmlReader->attributes().value(XMLTags::name).toString();

    QString title = xmlReader->attributes().value(XMLTags::title).toString();
    title = Core::getLocaleString(title);

    emit debugMessage(getLevelIndent(level) + tr("Restoring push button '%1' with title '%2'").arg(name).arg(title));

    QString onQuestion, offQuestion;

    bool onQuestionOn = false, offQuestionOn = false;

    int x = 0, y = 0, xSize = 0, ySize = 0;
    udav_drivers::SignalID signalId = 0;

    xmlReader->readNext();
    while (!xmlReader->atEnd())
    {
        if (xmlReader->isEndElement())
        {
            xmlReader->readNext();
            break;
        }
        if (xmlReader->isStartElement())
        {
            QStringRef tagName = xmlReader->name();
            if (tagName == XMLTags::x)
            {
                x = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("x=%1").arg(x));
            }
            else if (tagName == XMLTags::y)
            {
                y = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("y=%1").arg(y));
            }
            else if (tagName == XMLTags::xSize)
            {
                xSize = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("xSize=%1").arg(xSize));
            }
            else if (tagName == XMLTags::ySize)
            {
                ySize = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("ySize=%1").arg(ySize));
            }
            else if (tagName == XMLTags::signalID)
            {
                signalId = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("signalId=%1").arg(signalId));
            }
            else if (tagName == XMLTags::onQuestion)
            {
                onQuestion = xmlReader->attributes().value(XMLTags::text).toString();
                onQuestion = Core::getLocaleString(onQuestion);
                onQuestionOn = Core::readBoolFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("onQuestion=%1; question is '%2'").arg(onQuestionOn).arg(onQuestion));
            }
            else if (tagName == XMLTags::offQuestion)
            {
                offQuestion = xmlReader->attributes().value(XMLTags::text).toString();
                offQuestion = Core::getLocaleString(offQuestion);
                offQuestionOn = Core::readBoolFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("offQuestion=%1; question is '%2'").arg(offQuestionOn).arg(offQuestion));
            }
            else
                Core::skipUnknownElement(xmlReader);
        }
        else xmlReader->readNext();
    }

    PushButtonUiCell* btn = new PushButtonUiCell(widget);

    if (btn)
    {
        emit debugMessage(getLevelIndent(curLevel) + tr("Push button created. Adding it to widget..."));

        btn->setObjectName(name);
        btn->setText(title);

        btn->setGridGeometry(x, y, xSize, ySize, widget->gridStep_);
        btn->setVisible(true);

        btn->setSignalId(signalId);

        btn->setOnQuestionOn(onQuestionOn);
        btn->setOnQuestion(onQuestion);

        btn->setOffQuestionOn(offQuestionOn);
        btn->setOffQuestion(offQuestion);

        btn->setClassName(XMLTags::pushButton);

        widget->uiItems_.append(btn);
        widget->update();

        controlPanel_->ui2signal_[btn] = signalId;
        controlPanel_->signal2ui_[signalId] = btn;

        emit debugMessage(getLevelIndent(curLevel) + tr("Push button added to widget"));

        QObject::connect(btn, SIGNAL(controlSignalChanged(Drivers::SignalID, QVariant)), controlPanel_, SIGNAL(controlSignalChanged(Drivers::SignalID, QVariant)));

        emit debugMessage(getLevelIndent(level) + tr("Restored button: %1").arg(name));
    }
    else
    {
        emit debugMessage(getLevelIndent(level) + tr("Cannot allocate push button"));
    }
}

//-----------------------------------------------------------------------------

void
EquipmentUiReader::readTextLabel_(EquipmentControlPage *parent, QXmlStreamReader *xmlReader, int level)
{
    int tagsLevel = level + 1;

    QString name = xmlReader->attributes().value(XMLTags::name).toString();

    QString title = xmlReader->attributes().value(XMLTags::title).toString();
    title = Core::getLocaleString(title);
    title = restoreNewLines(title);

    emit debugMessage(getLevelIndent(level) + tr("Restoring text label '%1' with title '%2'").arg(name).arg(title));

    int x = 0, y = 0, xSize = 0, ySize = 0;

    xmlReader->readNext();
    while (!xmlReader->atEnd())
    {
        if (xmlReader->isEndElement())
        {
            xmlReader->readNext();
            break;
        }
        if (xmlReader->isStartElement())
        {
            QStringRef tagName = xmlReader->name();
            if (tagName == XMLTags::x)
            {
                x = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("x=%1").arg(x));
            }
            else if (tagName == XMLTags::y)
            {
                y = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("y=%1").arg(y));
            }
            else if (tagName == XMLTags::xSize)
            {
                xSize = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("xSize=%1").arg(xSize));
            }
            else if (tagName == XMLTags::ySize)
            {
                ySize = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("ySize=%1").arg(ySize));
            }
            else
                Core::skipUnknownElement(xmlReader);
        }
        else xmlReader->readNext();
    }

    TextMessageUiItem* textLabelUiItem = new TextMessageUiItem(parent);
    if (textLabelUiItem)
    {
        emit debugMessage(getLevelIndent(level) + tr("Text label created. Adding it to widget..."));

        textLabelUiItem->setText(title);
        textLabelUiItem->setGridGeometry(x, y, xSize, ySize, parent->gridStep_);
        textLabelUiItem->setVisible(true);
        textLabelUiItem->setClassName(XMLTags::text);

        parent->uiItems_.append(textLabelUiItem);
        parent->update();

        emit debugMessage(getLevelIndent(level) + tr("Text label added to widget"));
        emit debugMessage(getLevelIndent(level) + tr("Restored text label: %1").arg(name));
    }
    else
    {
        emit debugMessage(getLevelIndent(level) + tr("Cannot allocate text label"));
    }

}

//-----------------------------------------------------------------------------

void EquipmentUiReader::readTextMessage_(EquipmentControlPage* widget, QXmlStreamReader* xmlReader, int level)
{
//    QString name = xmlReader->attributes().value(XMLTags::name).toString();

//    int x = 0, y = 0, xSize = 0, ySize = 0;
//    Drivers::SignalID channel = 0;

////	QMenu* menu = 0;

//    // Читаем параметры кнопки
//    xmlReader->readNext();
//    while (!xmlReader->atEnd())
//    {
//        if (xmlReader->isEndElement())
//        {
//            xmlReader->readNext();
//            break;
//        }
//        if (xmlReader->isStartElement())
//        {
//            QStringRef tagName = xmlReader->name();
//            if (tagName == XMLTags::x)
//                x = Core::readIntFieldFromXML(xmlReader);
//            else if (tagName == XMLTags::y)
//                y = Core::readIntFieldFromXML(xmlReader);
//            else if (tagName == XMLTags::xSize)
//                xSize = Core::readIntFieldFromXML(xmlReader);
//            else if (tagName == XMLTags::ySize)
//                ySize = Core::readIntFieldFromXML(xmlReader);
//            else if (tagName == XMLTags::channel)
//                channel = Core::readIntFieldFromXML(xmlReader);
//            else
//                Core::skipUnknownElement(xmlReader);
//        }
//        else xmlReader->readNext();
//    }

//    qDebug() << "Creating text message!";

//    // Создаем интерфейсный элемент
//    TextMessageUiItem* textMessageUiItem = new TextMessageUiItem(widget);
//    textMessageUiItem->setText();
//    textMessageUiItem->setGridGeometry(x, y, xSize, ySize, widget->gridStep_);
//    textMessageUiItem->setVisible(true);
//    textMessageUiItem->setClassName(XMLTags::text);

//    widget->uiItems_.append(textMessageUiItem);
//    widget->update();

//    widget_->label2channel_[textMessageUiItem] = channel;
//    widget_->channel2label_[channel] = textMessageUiItem;
//    qDebug() << textMessageUiItem << ", channel:" << channel;

//	QObject::connect(textMessageUiItem, SIGNAL(toggled(PushButtonUiItem*,bool)), widget_, SLOT(pushButtonToggled(PushButtonUiItem*,bool)));
    //	QObject::connect(textMessageUiItem, SIGNAL(toggled(PushButtonUiItem*,qint16)), widget_, SLOT(pushButtonToggled(PushButtonUiItem*,qint16)));
}

//-----------------------------------------------------------------------------

void
EquipmentUiReader::readValueEdit_(EquipmentControlPage *parent, QXmlStreamReader *xmlReader, int level)
{
    int tagsLevel = level + 1;

    int x = 0, y = 0, xSize = 0, ySize = 0;
    udav_drivers::SignalID signalId = 0;

    QString name = xmlReader->attributes().value(XMLTags::name).toString();
    QString title = xmlReader->attributes().value(XMLTags::title).toString();
    title = Core::getLocaleString(title);

    emit debugMessage(getLevelIndent(level) + tr("Restoring value edit '%1' with title '%2'").arg(name).arg(title));

    QString unit;
    bool unitPresent = false;

    xmlReader->readNext();
    while (!xmlReader->atEnd())
    {
        if (xmlReader->isEndElement())
        {
            xmlReader->readNext();
            break;
        }
        if (xmlReader->isStartElement())
        {
            QStringRef tagName = xmlReader->name();
            if (tagName == XMLTags::x)
            {
                x = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("x=%1").arg(x));
            }
            else if (tagName == XMLTags::y)
            {
                y = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("y=%1").arg(y));
            }
            else if (tagName == XMLTags::xSize)
            {
                xSize = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("xSize=%1").arg(xSize));
            }
            else if (tagName == XMLTags::ySize)
            {
                ySize = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("ySize=%1").arg(ySize));
            }
            else if (tagName == XMLTags::unit)
            {
                unitPresent = true;
                unit = Core::readTextFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("unit: %1").arg(unit));
            }
            else if (tagName == XMLTags::signalID)
            {
                signalId = Core::readIntFieldFromXML(xmlReader);
                emit debugMessage(getLevelIndent(tagsLevel) + QString("signalID=%1").arg(signalId));
            }
            else
                Core::skipUnknownElement(xmlReader);
        }
        else xmlReader->readNext();
    }
    emit debugMessage(getLevelIndent(level) + tr("Value edit readed and will be created"));

    if (unitPresent)
        unit = Core::getLocaleString(unit);
    else
        unit = QString();

    ValueEditUiCell* valueEditUiCell = new ValueEditUiCell(parent, title, unit);
    valueEditUiCell->setClassName(XMLTags::valueEdit);

    valueEditUiCell->setSignalId(signalId);

    emit debugMessage(getLevelIndent(level) + tr("Value range: [%1; %2]").
                       arg(metasignals_[signalId].minValue).
                       arg(metasignals_[signalId].maxValue));

    emit debugMessage(getLevelIndent(level) + tr("Default value: %1").
                      arg(metasignals_[signalId].defaultValue));

    valueEditUiCell->setRange(metasignals_[signalId].minValue, metasignals_[signalId].maxValue);
    valueEditUiCell->setValue(metasignals_[signalId].defaultValue);

    valueEditUiCell->setGridGeometry(x, y, xSize, ySize, parent->gridStep_);
    valueEditUiCell->setVisible(true);

    parent->uiItems_.append(valueEditUiCell);
    parent->update();

    controlPanel_->ui2signal_[valueEditUiCell] = signalId;
    controlPanel_->signal2ui_[signalId] = valueEditUiCell;

    QObject::connect(valueEditUiCell, SIGNAL(controlSignalChanged(Drivers::SignalID, QVariant)), controlPanel_, SIGNAL(controlSignalChanged(Drivers::SignalID, QVariant)));

    emit debugMessage(getLevelIndent(level) + tr("Restored value edit '%1' with title '%2'").arg(name).arg(title));
}

//-----------------------------------------------------------------------------

bool
EquipmentUiReader::restore()
{
    QXmlStreamReader* xmlReader = new QXmlStreamReader(xml_);

    xmlReader->readNext();
    while (!xmlReader->atEnd())
    {
        if (xmlReader->isStartElement())
        {
            if (xmlReader->name() == XMLTags::driver)
            {
                xmlReader->readNext();
                while (!xmlReader->atEnd())
                {
                    if (xmlReader->isEndElement())
                    {
                        xmlReader->readNext();
                        break;
                    }

                    if (xmlReader->isStartElement())
                    {
                        if (xmlReader->name().toString() == XMLTags::page)
                            readPage_(xmlReader, 0);
                        else
                            Core::skipUnknownElement(xmlReader);
                    }
                    else
                        xmlReader->readNext();
                }
            }
            else xmlReader->raiseError(QObject::tr("Not a driver ui description!"));
        }
        else xmlReader->readNext();
    }

    if (xmlReader->hasError())
    {
        emit criticalMessage(QObject::tr("Error: failed to parse description! %1").arg(qPrintable(xmlReader->errorString())));
        delete xmlReader;

        return false;
    }

    delete xmlReader;
    return true;

}

} //namespace Gui
