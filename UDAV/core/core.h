#ifndef CORE_H
#define CORE_H

#include <QtGlobal>
#include <QMap>
#include <QDataStream>
#include <QString>

#include <QTranslator>

//! Главный модуль, ядро системы.
/**
  Содержит объявления констант и классов, составляющих основной функционал программы.
*/
namespace Core
{

//! Используемая версия потока для передачи данных.
const int StreamVersion = QDataStream::Qt_4_7;

//! Название программы клиента.
const QString ClientName = "UDAV client";

/**
 * @brief Имя исполняемого файла клиента.
 */
const QString ClientExeName = "udavc";

//! Название проекта.
const QString ProjectName = "udav";

//! Название программы сервера.
const QString ServerName = "UDAV server";

/**
 * @brief Имя исполняемого файла сервера
 */
const QString ServerExeName = "udavs";

//! Название компании-разработчика программ.
const QString CompanyName = "MELZ-INVEST";

//! Версия программы.
const QString RevisionNumber = "0.0.1";

enum PacketTypes
{
    REQUEST_CONNECTION = 0,
    CHECK_CONNECTION = REQUEST_CONNECTION + 1,

};

//! Допустимые типы пакетов, передаваемые между клиентом и сервером.
/**
  Диапазон значений констант, описывающих пакеты, 0...127.
*/
struct PacketType
{
    static const quint8 RequestConnection		= 0;	//!< Запрос на подключение
    static const quint8 CheckConnection			= 1;	//!< Проверка подключения
    static const quint8 ConnectionOK			= 2;	//!< Успешное соединение
    static const quint8 HandShake				= 3;	//!< "Рукопожатие" клинета и сервера
    static const quint8 RefuseConnection		= 4;	//!< Отказ в соединении
    static const quint8 DriversList				= 5;	//!< Список драйверов оборудования
    static const quint8 RegistrationRequest		= 6;	//!< Запрос на регистрацию нового пользователя
    static const quint8 RegistrationData		= 7;	//!< Регистрационные данные пользователя
    static const quint8 TicketRequest			= 8;	//!< Запрос билета на проведение эксперимента
    static const quint8 TicketRelease			= 9;	//!< Отказ от билета на проведение эксперимента
    static const quint8 Shedule					= 10;	//!< Запрос расписания загрузки оборудования
    static const quint8 TicketOK				= 11;	//!< Билет успешно проверен
    static const quint8 TicketRefuse			= 12;	//!< Отказ билету после проверки
    static const quint8 MetaSignals				= 13;	//!< Описание метасигналов драйвера
    static const quint8 Control					= 14;	//!< Пакет управляющих данных от клиента к серверу @todo Удалить, повторная константа
    static const quint8 View					= 15;	//!< Пакет данных наблюдения от сервера к клиенту @todo Удалить, повторная константа
    static const quint8 ConnectionType			= 16;	//!< Тип подключения
    static const quint8 SelectedDriver			= 17;	//!< Сообщение от клиента о выбранном драйвере
    static const quint8 UiDescription			= 18;	//!< Описание интерфейса пользователя
    static const quint8 ControlArray			= 19;	//!< Массив управляющих сигналов
    static const quint8 ViewArray				= 20;	//!< Массив сигналов наблюдения
    static const quint8 VideoFrame				= 22;	//!< Кадр из набора изображений для визуализации на клиенте
    static const quint8 File					= 24;	//!< Файл
    static const quint8 FileRequest				= 25;	//!< Запрос на передачу файла
    static const quint8 DriverSettings			= 26;	//!< Настройки драйвера оборудования
    static const quint8 SignalValue				= 30;	//!< Значение единичного сигнала (не пакет)
    static const quint8 TextMessage				= 31;	//!< Текстовое сообщение
    static const quint8 CommandOk               = 32;   //!< Управляющая команда успешно выполнена
    static const quint8 CommandError            = 33;   //!< При выполнении управляющей команды произошла ошибка
    static const quint8 OpenMessageWindow       = 34;   //!< Команда клиенту открыть окно для вывода сообщений
    static const quint8 CloseMessageWindow      = 35;   //!< Команда клиенту закрыть окно для вывода сообщения
    static const quint8 TextMessage2Window      = 36;   //!< Команда клиенту вывести сообщение в окно для вывода сообщения
    static const quint8 ReloadInterface         = 37;   //!< Команда от клиента серверу на обновление интерфейса пользователя

    static const quint8 Empty					= 127;	//!< Пустой пакет

    static const quint8 ConnectionEstablsihed    = 255;  //!< Соединение успешно установлено
};

//! Допустимые типы подключений, между клиентом и сервером.
/**
  Диапазон констант, описывающих типы подключения, 128...129.
  Константы задаются от базовой величины, равной PacketType::Empty.
*/
struct ConnectionType
{
    static const quint8 WithTicket			= PacketType::Empty + 1;		//!< Подключение с билетом
    static const quint8 WithoutTicket		= PacketType::Empty + 2;		//!< Подключение без билета
    static const quint8 DriversListRequest  = PacketType::Empty + 3;        //!< Запрос списка драйверов оборудования

};

struct MessageType
{
    static const quint16 Info = 1001;
    static const quint16 Debug = 1002;
    static const quint16 Critical = 1003;
};

//! Карта соответствия значений констант типов пакетов и их символических имен.
/**
  Может быть использована для вывода значений констант в лог-файл в формате понятном пользователю.
  Инициализируется вызовом функции initNetworkPacketNames().
*/
extern QMap<quint8, QString> packetNames;

extern QTranslator translator;

void initTranslator();

/**
  Инициализировать карту соответствия значений констант типов пакетов и их символических имен packetNames
*/
void initNetworkPacketNames();

//! Универсальная карта соответствия для хранения данных любых типов, с обращением к ним по именованым полям.
typedef QMap<QString, QVariant> UniversalNamedMap;

void messageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg);

}

#endif // CORE_H
