/*****************************************************************************
 * modbus4qt Library
 * Author: Leonid Kolesnik, l.kolesnik@m-i.ru
 * Copyright (C) 2012-2015
 * http://mt11.net.ru
 *****************************************************************************/

/*****************************************************************************
* This file is part of modbus4qt.
*
* modbus4qt is free software: you can redistribute it and/or modify it under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option) any
* later version.
*
* modbus4qt is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
* A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with
* modbus4qt.
*
* If not, see <https://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <modbus4qt/tcp_server.h>

class QGroupBox;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTableView;
class QTableWidget;

class CoilsTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    public slots:

        void about();

        void addCoils();

    private: // fields

        QAction* exitAction_;
        QAction* aboutAction_;
        QAction* aboutQtAction_;

        QMenu* fileMenu_;
        QMenu* helpMenu_;

        QLineEdit* serverAddress_;
        QSpinBox* serverPort_;
        QPushButton* listenButton_;

        //QTableView* coilsTableView_;
        CoilsTableModel* coilsTableModel_;

        QTableWidget* discreteInputsTable_;
        QTableWidget* inputRegistersTable_;
        QTableWidget* holdingRegistersTable_;

        modbus4qt::ServerInternalData* serverInternalData_;

    private: // methods

        void createActions_();

        void createMenus_();

        QGroupBox* createServerGroupBox_();

        QGroupBox* createCoilsGroupBox_();
        QGroupBox* createDiscreteInputsGroupBox_();
        QGroupBox* createInputRegistersGroupBox_();
        QGroupBox* createHoldingRegistersGroupBox_();
};

#endif // MAINWINDOW_H
