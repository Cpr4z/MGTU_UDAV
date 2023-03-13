#ifndef UDAV_GUI_UDAVMAINWINDOWCREATOR_H
#define UDAV_GUI_UDAVMAINWINDOWCREATOR_H

#include <QObject>

class QAction;
class QPlainTextEdit;

#include "elvactools/logger.h"

//-----------------------------------------------------------------------------

namespace Core
{

class UdavSettings;

}

//-----------------------------------------------------------------------------

namespace udav_gui
{

class UdavMainWindow;

/**
 * @brief The UdavMainWindowCreator class
 */
class UdavMainWindowCreator : public QObject
{
    Q_OBJECT
    ELVACTOOLS_LOGGER_SYSTEM_DECLARE

    public:

        /**
         * @brief Default constructor
         * @param settings - application settings
         * @param logger - application logger
         */
        UdavMainWindowCreator(Core::UdavSettings* settings, elvactools::Logger* logger);

        /**
         * @brief Create main window and return pointer to it
         * @return Pointer to main window created.
         */
        UdavMainWindow* createMainWindow();

    private:

        /**
         * @brief Main window to be created and returned
         */
        UdavMainWindow* mainWindow_;

        /**
         * @brief Application settings
         */
        Core::UdavSettings* settings_;

        /**
         * @brief Application logger
         */
        elvactools::Logger* logger_;

        /**
         * @brief Log viewer
         *
         * We need to save it since it'll be createed at start to catch all possible messages.
         */
        QPlainTextEdit* logMessagesView_;

        //---------------------------------------------
        // Actions (bof)

        // We need internal fields for actions since we use then in different places

        /**
         * @brief About action
         */
        QAction* aboutAction_;

        /**
         * @brief About Qt action
         */
        QAction* aboutQtAction_;

        /**
         * @brief Connect to equipment action
         */
        QAction* connectToServerAction_;

        /**
         * @brief Disconnect from equipment action
         */
        QAction* disconnectFromServerAction_;

        /**
         * @brief Reload ui action
         */
        QAction* reloadIfaceAction_;

        /**
         * @brief Edit settings action
         */
        QAction* editSettingsAction_;

        /**
         * @brief Exit action
         */
        QAction* exitAction_;

        /**
         * @brief Switch full screen action
         */
        QAction* fullScreenAction_;

        /**
         * @brief Start new protocol action
         */
        QAction* newProtocolAction_;

        /**
         * @brief Save protocol action
         */
        QAction* saveProtocolAction_;

        /**
         * @brief Switch to program panel action
         */
        QAction* swicth2ProgramControlPanelAction_;

        // Actions (eof)
        //---------------------------------------------

        /**
         * @brief Create actions
         */
        void createActions_();

        /**
         * @brief Create menus
         */
        void createMenus_();

        /**
        * @brief Create central widget
        */
        void createCentralWidget_();
};

} // namespace udav_gui

//-----------------------------------------------------------------------------
// EOF

#endif // UDAV_GUI_UDAVMAINWINDOWCREATOR_H
