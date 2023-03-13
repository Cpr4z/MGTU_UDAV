#include "core.h"
#include "elvactools/logger.h"

#include <cmath>

namespace Core
{

const quint8 PacketType::RequestConnection;
const quint8 PacketType::CheckConnection;
const quint8 PacketType::ConnectionOK;
const quint8 PacketType::HandShake;
const quint8 PacketType::RefuseConnection;
const quint8 PacketType::DriversList;
const quint8 PacketType::RegistrationRequest;
const quint8 PacketType::RegistrationData;
const quint8 PacketType::TicketRequest;
const quint8 PacketType::TicketRelease;
const quint8 PacketType::Shedule;
const quint8 PacketType::TicketOK;
const quint8 PacketType::TicketRefuse;
const quint8 PacketType::MetaSignals;
const quint8 PacketType::Control;
const quint8 PacketType::View;
const quint8 PacketType::ConnectionType;
const quint8 PacketType::SelectedDriver;
const quint8 PacketType::UiDescription;
const quint8 PacketType::ControlArray;
const quint8 PacketType::ViewArray;
const quint8 PacketType::VideoFrame;
const quint8 PacketType::File;
const quint8 PacketType::FileRequest;
const quint8 PacketType::DriverSettings;
const quint8 PacketType::SignalValue;
const quint8 PacketType::TextMessage;
const quint8 PacketType::CommandOk;
const quint8 PacketType::CommandError;
const quint8 PacketType::OpenMessageWindow;
const quint8 PacketType::CloseMessageWindow;
const quint8 PacketType::TextMessage2Window;
const quint8 PacketType::ReloadInterface;

const quint8 PacketType::Empty;

const quint8 PacketType::ConnectionEstablsihed;

const quint8 ConnectionType::WithTicket;
const quint8 ConnectionType::WithoutTicket;
const quint8 ConnectionType::DriversListRequest;

QMap<quint8, QString> packetNames;

QTranslator translator;

//-----------------------------------------------------------------------------

void
initTranslator()
{
    translator.load("udav_core_" + QLocale::system().name(), ":/");
}

//-----------------------------------------------------------------------------

void
initNetworkPacketNames()
{
   packetNames[PacketType::RequestConnection]		= "RequestConnection";
   packetNames[PacketType::CheckConnection]			= "CheckConnection";
   packetNames[PacketType::ConnectionOK]			= "ConnectionOK";
   packetNames[PacketType::HandShake]				= "HandShake";
   packetNames[PacketType::RefuseConnection]		= "RefuseConnection";
   packetNames[PacketType::DriversList]				= "DriversList";
   packetNames[PacketType::RegistrationRequest]		= "RegistrationRequest";
   packetNames[PacketType::RegistrationData]		= "RegistrationData";
   packetNames[PacketType::TicketRequest]			= "TicketRequest";
   packetNames[PacketType::TicketRelease]			= "TicketRelease";
   packetNames[PacketType::Shedule]					= "Shedule";
   packetNames[PacketType::TicketOK]				= "TicketOK";
   packetNames[PacketType::TicketRefuse]			= "TicketRefuse";
   packetNames[PacketType::MetaSignals]				= "MetaSignals";
   packetNames[PacketType::Control]					= "Control";
   packetNames[PacketType::View]					= "View";
   packetNames[PacketType::ConnectionType]			= "ConnectionType";
   packetNames[PacketType::SelectedDriver]			= "SelectedDriver";
   packetNames[PacketType::UiDescription]			= "UiDescription";
   packetNames[PacketType::ControlArray]			= "ControlArray";
   packetNames[PacketType::ViewArray]				= "ViewArray";
   packetNames[PacketType::VideoFrame]				= "VideoFrame";
   packetNames[PacketType::File]					= "File";
   packetNames[PacketType::FileRequest]				= "FileRequest";
   packetNames[PacketType::DriverSettings]			= "DriverSettings";
   packetNames[PacketType::SignalValue]             = "SignalValue";
   packetNames[PacketType::TextMessage]				= "TextMessage";
   packetNames[PacketType::CommandOk]               = "CommandOk";
   packetNames[PacketType::CommandError]			= "CommandError";
   packetNames[PacketType::OpenMessageWindow]		= "OpenMessageWindow";
   packetNames[PacketType::CloseMessageWindow]		= "CloseMessageWindow";
   packetNames[PacketType::TextMessage2Window]      = "TextMessage2Window";
   packetNames[PacketType::ReloadInterface]         = "ReloadInterface";

   packetNames[PacketType::Empty]					= "Empty";

   packetNames[PacketType::ConnectionEstablsihed]    = "Connection establihed";

   packetNames[ConnectionType::WithTicket]			= "WithTicket";
   packetNames[ConnectionType::WithoutTicket]		= "WithoutTicket";
}

} // namespace Core




