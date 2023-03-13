#include "gui.h"

namespace udav_gui
{

//-----------------------------------------------------------------------------

QIcon getConnectIcon()
{
    return QIcon(":/resource/icons/128x128/connected.png");
}

//-----------------------------------------------------------------------------

QIcon getDisconnectIcon()
{
    return QIcon(":/resource/icons/128x128/disconnected.png");
}

//-----------------------------------------------------------------------------

QIcon
getExitIcon()
{
    return QIcon(":/resource/icons/128x128/logout.png");
}

//-----------------------------------------------------------------------------

QIcon
getFullScreenIcon()
{
    return QIcon(":/resource/icons/128x128/fullscreen.png");
}

//-----------------------------------------------------------------------------

QIcon
getNewProtocolIcon()
{
    return QIcon(":/resource/icons/128x128/filenew.png");
}

//-----------------------------------------------------------------------------

QIcon
getReloadIcon()
{
    return QIcon(":/resource/icons/reload.png");
}

//-----------------------------------------------------------------------------

QIcon
getSaveProtocolIcon()
{
    return QIcon(":/resource/icons/128x128/filesave.png");
}

//-----------------------------------------------------------------------------

QIcon
getSettingsIcon()
{
    return QIcon(":/resource/icons/128x128/settings.png");
}

//-----------------------------------------------------------------------------

} // namespace udav_gui

//-----------------------------------------------------------------------------
// EOF
