#include "equipment_control_panel.h"

#include "equipment_control_page.h"

#include "equipment_config_widget.h"
#include "equipment_debug_widget.h"
#include "equipment_help_widget.h"
#include "equipment_ui_reader.h"

#include "elvactools/ui/grid_tool_button_widget.h"
#include "elvactools/ui/postioned_widget.h"

//#include "client.h"
#include "elvactools/utils.h"

#include "cells/abstract_ui_cell.h"
#include "cells/image_ui_item.h"
#include "cells/svg_image_ui_item.h"

#include "cells/push_button_ui_cell.h"
#include "cells/knob_ui_item.h"
#include "cells/led_ui_item.h"
#include "cells/lcd_ui_cell.h"
#include "cells/value_edit_ui_cell.h"

#include "equipment_ui_reader.h"

#include "core/udav_settings.h"

#include <QtGui>

#include <QAction>

#include <QTabWidget>

#include <QTextBrowser>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include "gui.h"

#include "driver/driver_impl.h"

const int PagesButtonWidth = 100;
const int PagesButtonSpace = 2;

namespace udav_gui
{

//-----------------------------------------------------------------------------

EquipmentControlPanel::EquipmentControlPanel(udav_drivers::DriverImpl* driverImpl,
                                             Core::UdavSettings* settings_,
                                             elvactools::Logger* logger,
                                             QWidget *parent) :
    QWidget(parent),
    driverImpl_(driverImpl),
    settings_(settings_),
    logger_(logger)
{
    ELVACTOOLS_LOGGER_SYSTEM_CONNECT(this, logger)

    //-----------------------------------------------------------------------
    // Create browser for messages going from driver

    messagesBrowser_ = new QTextBrowser();
    messagesBrowser_->setMaximumHeight(75);

    //------------------------------------------
    // Connect signals to catch any messages from equipment

    connect(driverImpl_, SIGNAL(criticalMessage(QString)), this, SLOT(displayCriticalMessage(QString)));
    connect(driverImpl_, SIGNAL(infoMessage(QString)), this, SLOT(displayInfoMessage(QString)));

    //-----------------------------------------------------------------------
    // Create central area for equipment ui pages

    controlPages_ = new QTabWidget(this);
    controlPages_->setTabPosition(QTabWidget::South);
    controlPages_->setTabShape(QTabWidget::Triangular);

    //-----------------------------------------------------------------------
    // Prepare right buttons grid widget for control panel actions

    const bool showText = true;

    pagesButtonsWidget_ = new GridToolButtonWidget(PagesButtonWidth, PagesButtonSpace, showText);
    pagesButtonsWidget_->setObjectName("EquipmentControlPanel::pagesButtonsWidget_");

    connect(pagesButtonsWidget_, SIGNAL(actionTriggered(int)), this, SLOT(changePage_(int)));

    QAction* newProtocolAction = new QAction(tr("New protocol"), this);
    newProtocolAction->setIcon(udav_gui::getNewProtocolIcon());
    newProtocolAction->setEnabled(true);
    pagesButtonsWidget_->addAction(START_NEW_PROTOCOL, newProtocolAction);

    QAction* saveProtocolAction = new QAction(tr("Save protocol"), this);
    saveProtocolAction->setIcon(QIcon(":/resource/gui/icons/tools.png"));
    saveProtocolAction->setEnabled(true);
    pagesButtonsWidget_->addAction(SAVE_PROTOCOL, saveProtocolAction);

    QAction* disconnectAction = new QAction(tr("Disconnect"), this);
    disconnectAction->setIcon(QIcon(":/resource/gui/icons/tools.png"));
    disconnectAction->setEnabled(true);
    pagesButtonsWidget_->addAction(DISCONNECT, disconnectAction);

    QAction* shutdownAction = new QAction(tr("Shutdown"), this);
    shutdownAction->setIcon(QIcon(":/resource/gui/icons/tools.png"));
    shutdownAction->setEnabled(true);
    pagesButtonsWidget_->addAction(SHUTDOWN, shutdownAction);

    QAction* reloadUiAction = new QAction(tr("Reload UI"), this);
    reloadUiAction->setIcon(QIcon(":/resource/gui/icons/tools.png"));
    reloadUiAction->setEnabled(true);
    pagesButtonsWidget_->addAction(RELOAD_UI, reloadUiAction);

    //-----------------------------------------------------------------------
    // Layout items

    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->addWidget(controlPages_);
    controlLayout->addWidget(pagesButtonsWidget_);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(messagesBrowser_, 1);

    setLayout(mainLayout);

    //-----------------------------------------------------------------------
    // Restore pages ui reading it from driver

    restoreUi(driverImpl_->metaSignals(), driverImpl_->userInterface());

    //connect(driverImpl_, SIGNAL(debugMessage(QString)), this, SLOT(displayDebugMessage(QString)));

//    //>> Сигналы при установленном соединении (BOF)
//    connect(client_, SIGNAL(criticalMessageFromServer(QString)), this, SLOT(displayCriticalMessage(QString)));
//    connect(client_, SIGNAL(errorMessageFromServer(QString)), this, SLOT(displayErrorMessage(QString)));
//    //<< Сигналы при установленном соединении (EOF)

//    connect(client_, SIGNAL(fileRecieved(QString)), this, SLOT(fileRecieved(QString)));
//    connect(client_, SIGNAL(signalValueChanged(Drivers::SignalID, QVariant, QString)), this, SLOT(signalValueChanged(Drivers::SignalID, QVariant, QString)));

    // От панели управления к серверу. Надо сделать ревизию
//    connect(this, SIGNAL(controlSignalChanged(Drivers::SignalID,QVariant)), client, SLOT(controlSignalChanged(Drivers::SignalID,QVariant)));
//    connect(this, SIGNAL(requestFileFromServer(QString)), client_, SLOT(requestFileFromServer(QString)));
}

//-----------------------------------------------------------------------------

EquipmentControlPanel::~EquipmentControlPanel()
{
    removeWindows_();
}

//-----------------------------------------------------------------------------

void
EquipmentControlPanel::displayCriticalMessage(const QString &message)
{
    QMessageBox::critical(this, tr("Error"), message, QMessageBox::Ok);
}

//-----------------------------------------------------------------------------

void
EquipmentControlPanel::addPage_(EquipmentControlPage* page)
{
    controlPages_->addTab(page, page->windowTitle());
    controlPagesArray_.append(page);
}

//-----------------------------------------------------------------------------

void
EquipmentControlPanel::addWindow_(EquipmentControlPage *page)
{
    PostionedWidget* window = new PostionedWidget(page->objectName(), settings_);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->setWindowTitle(page->windowTitle());
    window->setMinimumSize(QSize(800, 600));

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(page);
    window->setLayout(layout);

    controlWindowsArray_.append(window);

    if (settings_->startInFullScreenMode())
    {
        window->showNormal();
    }
    else
    {
        window->showFullScreen();
    }

    window->show();
}

//-----------------------------------------------------------------------------

EquipmentControlPage*
EquipmentControlPanel::createControlPage()
{
    EquipmentControlPage* equipmentControlPage = new EquipmentControlPage(settings_, this);

    connect(equipmentControlPage, SIGNAL(displayErrorMessageInBrowser(QString)), this, SLOT(displayErrorMessageInBrowser(QString)));

    return equipmentControlPage;
}

//-----------------------------------------------------------------------------

void
EquipmentControlPanel::displayErrorMessageInBrowser(const QString &message)
{
    messagesBrowser_->setFontWeight(QFont::Bold);
    messagesBrowser_->setTextColor(QColor("red"));

    messagesBrowser_->append(message);
}

//-----------------------------------------------------------------------------

void
EquipmentControlPanel::displayErrorMessage(const QString &message)
{
    messagesBrowser_->setFontWeight(QFont::Bold);
    messagesBrowser_->setTextColor(QColor("red"));

    messagesBrowser_->append(message);

//    QMessageBox::warning(this, tr("Error"), message, QMessageBox::Ok);
}

//-----------------------------------------------------------------------------

void
EquipmentControlPanel::displayInfoMessage(const QString &message)
{
//    QMessageBox::information(this, tr("Information"), message, QMessageBox::Ok);

    messagesBrowser_->setFontWeight(QFont::Normal);
    messagesBrowser_->setTextColor(QColor("black"));

    messagesBrowser_->append(message);
}

//-----------------------------------------------------------------------------

void
EquipmentControlPanel::fileRecieved(const QString& fname)
{
    emit infoMessage(tr("File %1 recieved!").arg(fname));

    // Надо проверить мы ли заказывали прием файла
    // Если мы, то надо выполнить то, ради чего его заказывали
    if (fileDispatcher_.fileInQueue(fname))
    {
        quint8 fileType = fileDispatcher_.getFileType(fname);

        if (!fileType) return;

        quint16 fileIndex = fileDispatcher_.getFileIndex(fname);
        if (fileType == Core::IncomingFilesDispatcher::FileTypes::Image)
        {
            // fileIndex в этом случае соответствует номеру сигнала
            if (fileIndex == udav_drivers::CS_Begin || fileIndex == udav_drivers::VS_Begin) return; // Нулевые сигналы игнорируем

            // Надо просто сообщить интерфейсному элементу, привязанному к сигналу,
            // что для него получена картинка, и передать ему ее имя
            //! @todo Этот код повторяется с кодом в методе обработки сигналов. Надо бы унифицировать, вынеся его в отдельный метод
            //! @todo При медленной работе сети и большом размере картинок есть шанс, что изображение будет отставать от значения сигнала

            QString tempPath = Core::getTempPath();
            QString fullName = Core::buildFullFileName(fname, tempPath);

            // Ищем по номеру сигнала элемент управления
            // Если сигнал с таким номером не найден, значит он не привязан ни к одному элементу управления. Его просто игнорируем.
            AbstractUiCell* ui = (AbstractUiCell*)(signal2ui_[fileIndex]);
            if (!ui) return;

            QString className = ui->getClassName();
            if (className == XMLTags::image)
            {
                ImageUiItem* imageUiItem = dynamic_cast<ImageUiItem*>(ui);
                imageUiItem->setFileName(fullName);
            }
            else if (className == XMLTags::svgImage)
            {
                SvgImageUiItem* svgImageUiItem = dynamic_cast<SvgImageUiItem*>(ui);
                svgImageUiItem->setFileName(fullName);
            }
        }
        fileDispatcher_.removeFile(fname);
    }
}

//-----------------------------------------------------------------------------

void
EquipmentControlPanel::changePage_(int i)
{
    controlPages_->setCurrentIndex(i);
}

//-----------------------------------------------------------------------------

void
EquipmentControlPanel::signalValueChanged(udav_drivers::SignalID signalId, const QVariant& value, const QString& message)
{
    if (signalId == udav_drivers::CS_Begin || signalId == udav_drivers::VS_Begin) return; // Нулевые сигналы игнорируем

    udav_drivers::MetaSignal::ValueType valueType; // = client_->getMetaSignals()[signalId].valueType;

    // Устанавливаем доступность элементов, зависимых от значения сигнала
    QList<UiEnabler> enablers = uiEnbaler.values(signalId);
    UiEnabler item;
    foreach(item, enablers)
    {
        if (item.ui)
        {
            bool ok = true;
            int val = value.toInt(&ok);
            if (ok)
            {
                if (item.signalValue == val)
                    item.ui->setEnabled(true);
                else
                    item.ui->setEnabled(false);
            }
        }
    }

    // Ищем по номеру сигнала элемент управления
    // Если сигнал с таким номером не найден, значит он не привязан ни к одному элементу управления. Его просто игнорируем.
    AbstractUiCell* ui = (AbstractUiCell*)(signal2ui_[signalId]);
    if (!ui)
    {
        emit infoMessage(tr("Signal with id=%1 is not connected to interface. Skipping...").arg(signalId));
        return;
    }

    QString className = ui->getClassName();

    emit debugMessage(tr("Updating view: SignalID: %1").arg(signalId));
    emit debugMessage(tr("Ui class name: %1").arg(className));

    if (className == XMLTags::image)
    {
        ImageUiItem* imageUiItem = dynamic_cast<ImageUiItem*>(ui);

        // Для изображения тип значения может быть только int - номер картинки.
        // Все остальные типы игнорируем.
        if (valueType != udav_drivers::MetaSignal::Integer) return;

        bool ok = true;
        int signalValue = value.toInt(&ok);

        // В случае неуспешного преобразования значения, игнорируем полученные данные
        if (!ok) return;

        // Формируем имя файла изображения и устанавливаем его элементу отображения
        QString imgFileName = "signal-%1-%2";
        imgFileName += ".jpg";

        imgFileName = imgFileName.arg(signalId, 3).arg(signalValue, 3, 10, QLatin1Char('0'));

        QString tempPath = Core::getTempPath();
        // Проверяем на доступность пути для хранения файла мнемосхемы
        // Если папки нет, она будет созадана
        if (!Core::checkFilePath(imgFileName, tempPath)) return;
        QString fullName = Core::buildFullFileName(imgFileName, tempPath);

//        QDir dir;
//        if (settings_->cacheFiles())
//        {
//            if (!dir.exists(fullName))
//            {
//                emit infoMessage(tr("No image file %1 found in cache. Requesting it from server...").arg(imgFileName));
//                fileDispatcher_.addFile(imgFileName, Core::IncomingFilesDispatcher::FileTypes::Image, signalId);
//                emit requestFileFromServer(imgFileName);
//                return;
//            }
//            imageUiItem->setFileName(fullName);
//        }
//        else
//        {
//            fileDispatcher_.addFile(imgFileName, Core::IncomingFilesDispatcher::FileTypes::Image, signalId);
//            emit requestFileFromServer(imgFileName);
//            return;
//        }
    }
    else if (className == XMLTags::svgImage)
    {
        SvgImageUiItem* svgImageUiItem = dynamic_cast<SvgImageUiItem*>(ui);

        // Через метаданные определяем тип значений. Для изображения он может быть только int - номер картинки.
        // Все остальные типы игнорируем.
        if (valueType != udav_drivers::MetaSignal::Integer) return;

        bool ok = true;
        int signalValue = value.toInt(&ok);

        // В случае неуспешного преобразования значения, игнорируем полученные данные
        if (!ok) return;

        // Формируем имя файла изображения и устанавливаем его элементу отображения
        QString imgFileName = "drivers/ui/signal-%1-%2";
        imgFileName += ".svg";

        imgFileName = imgFileName.arg(signalId, 3).arg(signalValue, 4, 16, QLatin1Char('0'));

        QString tempPath = Core::getTempPath();

        // Проверяем на доступность пути для хранения файла мнемосхемы
        // Если папки нет, она будет созадана
        if (!Core::checkFilePath(imgFileName, tempPath)) return;

        QString fullName = Core::buildFullFileName(imgFileName, tempPath);

        QDir dir;
//        if (clientSettings->cacheFiles())
//        {
//            if (!dir.exists(fullName))
//            {
//                emit infoMessage(tr("No image file %1 found in cache. Requesting it from server...").arg(imgFileName));

//                fileDispatcher_.addFile(imgFileName, Core::IncomingFilesDispatcher::FileTypes::Image, signalId);
//                emit requestFileFromServer(imgFileName);

//                return;
//            }
//            svgImageUiItem->setFileName(fullName);
//        }
//        else
//        {
//            fileDispatcher_.addFile(imgFileName, Core::IncomingFilesDispatcher::FileTypes::Image, signalId);
//            emit requestFileFromServer(imgFileName);

//            return;
//        }
    }
    else if (className == XMLTags::led)
    {
        LedUiItem* ledUiItem = dynamic_cast<LedUiItem*>(ui);

        // Через метаданные определяем тип значений. Для LED он может быть только bool. true - зеленый, false - красный.
        // Все остальные типы игнорируем.
        if (valueType != udav_drivers::MetaSignal::Boolean) return;

        bool signalValue = value.toBool();
        ledUiItem->setValue(signalValue);
    }
    else if (className == XMLTags::lcd)
    {
        LcdUiCell* lcdUiItem = dynamic_cast<LcdUiCell*>(ui);

        // Через метаданные определяем тип значений. Для LCD он может быть только int или real.
        // Все остальные типы игнорируем.
        if (valueType != udav_drivers::MetaSignal::Integer &&  valueType != udav_drivers::MetaSignal::Real) return;

        bool ok = true;
        double signalValue = value.toDouble(&ok);
        if (!ok) return;

        lcdUiItem->setValue(signalValue);
    }
    else if (className == XMLTags::pushButton || className == XMLTags::circuitButton)
    {
        PushButtonUiCell* pushButtonUiItem = dynamic_cast<PushButtonUiCell*>(ui);

        // Через метаданные определяем тип значений. Для pushButton он может быть только bool
        // Все остальные типы игнорируем.
        udav_drivers::MetaSignal::ValueType valueType; // = client_->getMetaSignals()[signalId].valueType;
        if (valueType != udav_drivers::MetaSignal::Boolean) return;

        bool signalValue = value.toBool();

        pushButtonUiItem->setChecked(signalValue);
    }
    else if (className == XMLTags::valueEdit)
    {
        ValueEditUiCell* valueEditUiItem = dynamic_cast<ValueEditUiCell*>(ui);
        // Через метаданные определяем тип значений. Для valueEdit он может быть только real или int
        // Все остальные типы игнорируем.
        udav_drivers::MetaSignal::ValueType valueType; // = client_->getMetaSignals()[signalId].valueType;

        if (valueType != udav_drivers::MetaSignal::Integer && valueType != udav_drivers::MetaSignal::Real)
            return;

        bool ok = true;
        double signalValue = value.toDouble(&ok);
        if (!ok)
            return;

        valueEditUiItem->setValue(signalValue);
    }

    if (!message.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), message, QMessageBox::Ok);
    }
}

//-----------------------------------------------------------------------------

void
EquipmentControlPanel::removePages_()
{
    if (!controlPagesArray_.size()) return;

    emit infoMessage(tr("Removing user %1 interface pages from control panel...").arg(controlPagesArray_.size()));

    for (int i = 0; i < controlPagesArray_.size(); ++i)
    {
        // Кнопки страниц управления оборудованием находятся в начале,
        // поэтому просто удаляем необходимое количество кнопок от начала
        pagesButtonsWidget_->removeAction(0);

        // Удаляем команду на переключение страницы
        delete pagesActionsArray_[i];

        // Удаляем страницу
        controlPages_->removeTab(i); //controlPagesArray_[i]);
    }

    pagesActionsArray_.clear();
    controlPagesArray_.clear();
}

//-----------------------------------------------------------------------------

void
EquipmentControlPanel::removeWindows_()
{
    if (!controlWindowsArray_.size()) return;

    for (int i = 0; i < controlWindowsArray_.size(); ++i)
    {
        controlWindowsArray_[i]->close();

        delete controlWindowsArray_[i];
    }

    controlWindowsArray_.clear();
}

//-----------------------------------------------------------------------------

bool
EquipmentControlPanel::restoreUi(const udav_drivers::MetaSignals& metaSignals, const QString &xml)
{
    signal2ui_.clear();
    ui2signal_.clear();

    removePages_();
    removeWindows_();

    emit infoMessage(tr("Restoring user interface recieved from driver..."));

    EquipmentUiReader reader(this, settings_, metaSignals, xml, logger_);

    bool result = reader.restore();

    if (!result)
    {
        QMessageBox::critical(this, "ClientUserFriendlyName",
                              tr("An error occurred during interface restoring!\n"
                                 "See more information in log file"),
                              QMessageBox::Ok);
    }

    emit infoMessage(tr("Total restored %1 page(s)").arg(controlPagesArray_.size()));

    return result;
}

//-----------------------------------------------------------------------------

} //namespace Gui
