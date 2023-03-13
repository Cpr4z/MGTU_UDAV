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

#include <QWidget>

#include <modbus4qt/rtu_client.h>

class QGroupBox;
class QLineEdit;
class QSpinBox;
class QComboBox;
class QSettings;

class MainWindow : public QWidget
{
    Q_OBJECT

    private:
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

        QGroupBox* connectionSettingsGroupBox_;
        QComboBox *serialPortComboBox_;
        QSpinBox* unitId_;
        QComboBox* baudRateComboBox_;
        QComboBox* dataBitsComboBox_;
        QComboBox* stopBitsComboBox_;
        QComboBox* parityComboBox_;

        QGroupBox* userDefinedFunctionGroupBox_;
        QSpinBox* functionCode_;
        QSpinBox* subFunctionCode_;

        QSettings* settings_;

        modbus4qt::RtuClient* mbClient_;

        void createCoilsGroupBox_();
        void createDiscreteInputsGroupBox_();
        void createInputRegistersGroupBox_();
        void createHoldingRegistersGroupBox_();
        void createUserDefinedFunctionGroupBox_();

        void createConnectionSettingsGroupBox_();

        void restoreSettings_();

        void saveSettings_();

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public slots:
        void changePortName(int i);
        void changeBaudRate(int i);
        void changeDataBits(int i);
        void changeStopBits(int i);
        void changeParity(int i);

        void closeEvent(QCloseEvent *event);

        void errorMessage(const QString& msg);
        void infoMessage(const QString& msg);

        void readCoils();
        void readDiscreteInputs();
        void readInputRegistres();
        void readHoldingRegisters();

        void sendUserDefinedFunction();

        void setUnitId(int id);

        void writeCoils();
        void writeHoldingRegisters();
};

#endif // MAIN_WINDOW_H
