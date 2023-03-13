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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

#include <modbus4qt/tcp_client.h>

class QGroupBox;
class QLineEdit;
class QSpinBox;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public slots:

        void about();

        void errorMessage(const QString& msg);
        void infoMessage(const QString& msg);

        void readCoils();
        void readDiscreteInputs();
        void readInputRegistres();
        void readHoldingRegisters();

        void writeCoils();
        void writeHoldingRegisters();

    private: // fields

        QAction* exitAction_;
        QAction* aboutAction_;
        QAction* aboutQtAction_;

        QMenu* fileMenu_;
        QMenu* helpMenu_;

        QSpinBox* coilStart_;
        QSpinBox* coilQty_;
        QSpinBox* coilValue_;

        QSpinBox* dInStart_;
        QSpinBox* dInQty_;

        QSpinBox* inRegStart_;
        QSpinBox* inRegQty_;

        QSpinBox* holdRegStart_;
        QSpinBox* holdRegQty_;
        QSpinBox* holdRegValue_;

        QGroupBox* coilsGroupBox_;
        QGroupBox* discreteInputsGroupBox_;
        QGroupBox* inputRegistersGroupBox_;
        QGroupBox* holdingRegistersGroupBox_;

        QGroupBox* serverGroupBox_;

        QLineEdit* serverAddress_;

        QSpinBox* serverPort_;

        modbus4qt::TcpClient* tcpClient_;

    private: // methods

        void createActions_();

        void createCoilsGroupBox_();

        void createDiscreteInputsGroupBox_();

        void createInputRegistersGroupBox_();

        void createHoldingRegistersGroupBox_();

        void createMenus_();

        void createServerGroupBox_();
};

#endif // MAIN_WINDOW_H
