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

#include "main.h"
#include "main_window.h"

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QGroupBox>
#include <QPushButton>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QSettings>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QGridLayout* registersLayout = new QGridLayout;

    createConnectionSettingsGroupBox_();

    createCoilsGroupBox_();
    createDiscreteInputsGroupBox_();
    createInputRegistersGroupBox_();
    createHoldingRegistersGroupBox_();

    registersLayout->addWidget(discreteInputsGroupBox_, 0, 0);
    registersLayout->addWidget(inputRegistersGroupBox_, 0, 1);
    registersLayout->addWidget(coilsGroupBox_, 1, 0);
    registersLayout->addWidget(holdingRegistersGroupBox_, 1, 1);

    createUserDefinedFunctionGroupBox_();

    mainLayout->addWidget(connectionSettingsGroupBox_);
    mainLayout->addLayout(registersLayout);
    mainLayout->addWidget(userDefinedFunctionGroupBox_);
    mainLayout->addStretch();
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(mainLayout);

    setWindowTitle(ApplicationName);

    QString portName = serialPortComboBox_->currentText();

    QSerialPort::BaudRate baudRate = (QSerialPort::BaudRate)baudRateComboBox_->itemData(baudRateComboBox_->currentIndex()).toInt();
    QSerialPort::DataBits dataBits = (QSerialPort::DataBits)dataBitsComboBox_->itemData(dataBitsComboBox_->currentIndex()).toInt();
    QSerialPort::StopBits stopBits = (QSerialPort::StopBits)stopBitsComboBox_->itemData(stopBitsComboBox_->currentIndex()).toInt();
    QSerialPort::Parity parity = (QSerialPort::Parity)parityComboBox_->itemData(parityComboBox_->currentIndex()).toInt();

    mbClient_ = new modbus4qt::RtuClient(portName, baudRate, dataBits, stopBits, parity, this);
    mbClient_->setUnitID(unitId_->value());
    mbClient_->setReadTimeOut(5000);
    mbClient_->openPort();

    connect(mbClient_, SIGNAL(errorMessage(QString)), this, SLOT(errorMessage(QString)));

    connect(serialPortComboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(changePortName(int)));
    connect(baudRateComboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(changeBaudRate(int)));
    connect(dataBitsComboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDataBits(int)));
    connect(stopBitsComboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(changeStopBits(int)));
    connect(parityComboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(changeParity(int)));
    connect(unitId_, SIGNAL(valueChanged(int)), this, SLOT(setUnitId(int)));

    settings_ = new QSettings(this);
    restoreSettings_();
}

//-----------------------------------------------------------------------------

MainWindow::~MainWindow()
{
}

//-----------------------------------------------------------------------------

void
MainWindow::changePortName(int /*i*/)
{
    QString portName = serialPortComboBox_->currentText();;
    mbClient_->setPortName(portName);
}

//-----------------------------------------------------------------------------

void
MainWindow::changeBaudRate(int i)
{
    QSerialPort::BaudRate baudRate = (QSerialPort::BaudRate)baudRateComboBox_->itemData(i).toInt();
    mbClient_->setBaudRate(baudRate);
}

//-----------------------------------------------------------------------------

void
MainWindow::changeDataBits(int i)
{
    QSerialPort::DataBits dataBits = (QSerialPort::DataBits)dataBitsComboBox_->itemData(i).toInt();
    mbClient_->setDataBits(dataBits);
}

//-----------------------------------------------------------------------------

void
MainWindow::changeStopBits(int i)
{
    QSerialPort::StopBits stopBits = (QSerialPort::StopBits)stopBitsComboBox_->itemData(i).toInt();
    mbClient_->setStopBits(stopBits);
}

//-----------------------------------------------------------------------------

void
MainWindow::changeParity(int i)
{
    QSerialPort::Parity parity = (QSerialPort::Parity)parityComboBox_->itemData(i).toInt();
    mbClient_->setParity(parity);
}

//-----------------------------------------------------------------------------

void
MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings_();
    event->accept();
}

//-----------------------------------------------------------------------------

void
MainWindow::createCoilsGroupBox_() // r/w
{
    coilsGroupBox_ = new QGroupBox(tr("Coils"));

    QLabel* startLabel = new QLabel(tr("Start:"));
    coilStart_ = new QSpinBox();
    coilStart_->setRange(0, 65535);

    QLabel* qtyLabel = new QLabel(tr("Quantity:"));
    coilQty_ = new QSpinBox();
    coilQty_->setRange(1, 2000);

    QLabel* valueLabel = new QLabel(tr("Value:"));
    coilValue_ = new QSpinBox();
    coilValue_->setRange(0, 1);

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(startLabel, 0, 0);
    gridLayout->addWidget(coilStart_, 0, 1);
    gridLayout->addWidget(qtyLabel, 1, 0);
    gridLayout->addWidget(coilQty_, 1, 1);
    gridLayout->addWidget(valueLabel, 2, 0);
    gridLayout->addWidget(coilValue_, 2, 1);

    QPushButton* readBotton = new QPushButton("Read");
    connect(readBotton, SIGNAL(clicked()), this, SLOT(readCoils()));

    QPushButton* writeBotton = new QPushButton("Write");
    connect(writeBotton, SIGNAL(clicked()), this, SLOT(writeCoils()));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(readBotton);
    buttonLayout->addWidget(writeBotton);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(gridLayout);
    layout->addLayout(buttonLayout);

    coilsGroupBox_->setLayout(layout);
}

//-----------------------------------------------------------------------------

void
MainWindow::createDiscreteInputsGroupBox_()
{
    discreteInputsGroupBox_ = new QGroupBox(tr("Discrete inputs"));

    QLabel* startLabel = new QLabel(tr("Start:"));
    dInStart_ = new QSpinBox();
    dInStart_->setRange(0, 65535);

    QLabel* qtyLabel = new QLabel(tr("Quantity:"));
    dInQty_ = new QSpinBox();
    dInQty_->setRange(1, 2000);

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(startLabel, 0, 0);
    gridLayout->addWidget(dInStart_, 0, 1);
    gridLayout->addWidget(qtyLabel, 1, 0);
    gridLayout->addWidget(dInQty_, 1, 1);

    QPushButton* readBotton = new QPushButton("Read");
    connect(readBotton, SIGNAL(clicked()), this, SLOT(readDiscreteInputs()));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(readBotton);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(gridLayout);
    layout->addStretch();
    layout->addLayout(buttonLayout);
    discreteInputsGroupBox_->setLayout(layout);
}

//-----------------------------------------------------------------------------

void
MainWindow::createInputRegistersGroupBox_()
{
    inputRegistersGroupBox_ = new QGroupBox(tr("Input registers"));

    QLabel* startLabel = new QLabel(tr("Start:"));
    inRegStart_ = new QSpinBox();
    inRegStart_->setRange(0, 65535);

    QLabel* qtyLabel = new QLabel(tr("Quantity:"));
    inRegQty_ = new QSpinBox();
    inRegQty_->setRange(1, 125);

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(startLabel, 0, 0);
    gridLayout->addWidget(inRegStart_, 0, 1);
    gridLayout->addWidget(qtyLabel, 1, 0);
    gridLayout->addWidget(inRegQty_, 1, 1);

    QPushButton* readBotton = new QPushButton("Read");
    connect(readBotton, SIGNAL(clicked()), this, SLOT(readInputRegistres()));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(readBotton);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(gridLayout);
    layout->addStretch();
    layout->addLayout(buttonLayout);

    inputRegistersGroupBox_->setLayout(layout);
}

//-----------------------------------------------------------------------------

void
MainWindow::createHoldingRegistersGroupBox_() // r/w
{
    holdingRegistersGroupBox_ = new QGroupBox(tr("Holding registers"));

    QLabel* startLabel = new QLabel(tr("Start:"));
    holdRegStart_ = new QSpinBox();
    holdRegStart_->setRange(0, 65535);

    QLabel* qtyLabel = new QLabel(tr("Quantity:"));
    holdRegQty_ = new QSpinBox();
    holdRegQty_->setRange(1, 125);

    QLabel* valueLabel = new QLabel(tr("Value:"));
    holdRegValue_ = new QSpinBox();
    holdRegValue_->setRange(0, 65535);

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(startLabel, 0, 0);
    gridLayout->addWidget(holdRegStart_, 0, 1);
    gridLayout->addWidget(qtyLabel, 1, 0);
    gridLayout->addWidget(holdRegQty_, 1, 1);
    gridLayout->addWidget(valueLabel, 2, 0);
    gridLayout->addWidget(holdRegValue_, 2, 1);

    QPushButton* readButton = new QPushButton("Read");
    connect(readButton, SIGNAL(clicked()), this, SLOT(readHoldingRegisters()));

    QPushButton* writeButton = new QPushButton("Write");
    connect(writeButton, SIGNAL(clicked()), this, SLOT(writeHoldingRegisters()));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(readButton);
    buttonLayout->addWidget(writeButton);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(gridLayout);
    layout->addLayout(buttonLayout);

    holdingRegistersGroupBox_->setLayout(layout);
}

//-----------------------------------------------------------------------------

void
MainWindow::createConnectionSettingsGroupBox_()
{
    connectionSettingsGroupBox_ = new QGroupBox(tr("Connection settings"));
    QGridLayout *layout = new QGridLayout;

    QLabel *serialPortLabel = new QLabel(tr("Serial port:"));
    serialPortComboBox_ = new QComboBox();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        serialPortComboBox_->addItem(info.portName());
    serialPortComboBox_->setCurrentIndex(0);

    QLabel* unitIdLabel = new QLabel(tr("Unit ID:"));
    unitId_ = new QSpinBox();
    unitId_->setRange(1, 247);
    unitId_->setValue(1);

    QLabel* baudRateLabel = new QLabel(tr("Baud rate:"));

    baudRateComboBox_ = new QComboBox();
    baudRateComboBox_->addItem(QString("1200"), QSerialPort::Baud1200);
    baudRateComboBox_->addItem(QString("2400"), QSerialPort::Baud2400);
    baudRateComboBox_->addItem(QString("4800"), QSerialPort::Baud4800);
    baudRateComboBox_->addItem(QString("9600"), QSerialPort::Baud9600);
    baudRateComboBox_->addItem(QString("19200"), QSerialPort::Baud19200);
    baudRateComboBox_->addItem(QString("38400"), QSerialPort::Baud38400);
    baudRateComboBox_->addItem(QString("57600"), QSerialPort::Baud57600);
    baudRateComboBox_->addItem(QString("115200"), QSerialPort::Baud115200);
    baudRateComboBox_->setCurrentIndex(3);

    QLabel* dataBitsLabel = new QLabel(tr("Data bits:"));

    dataBitsComboBox_ = new QComboBox();
    dataBitsComboBox_->addItem(QString("7"), QSerialPort::Data7);
    dataBitsComboBox_->addItem(QString("8"), QSerialPort::Data8);
    dataBitsComboBox_->setCurrentIndex(1);

    QLabel* stopBitsLabel = new QLabel(tr("Stop bits:"));

    stopBitsComboBox_ = new QComboBox();
    stopBitsComboBox_->addItem(QString("1"), QSerialPort::OneStop);
    stopBitsComboBox_->addItem(QString("1.5"), QSerialPort::OneAndHalfStop);
    stopBitsComboBox_->addItem(QString("2"), QSerialPort::TwoStop);

    QLabel* parityLabel = new QLabel(tr("Parity:"));

    parityComboBox_ = new QComboBox();
    parityComboBox_->addItem(QString("None"), QSerialPort::NoParity);
    parityComboBox_->addItem(QString("Even"), QSerialPort::EvenParity);
    parityComboBox_->addItem(QString("Odd"), QSerialPort::OddParity);

    layout->addWidget(serialPortLabel, 0, 0);
    layout->addWidget(serialPortComboBox_, 0, 1);

    layout->addWidget(unitIdLabel, 1, 0);
    layout->addWidget(unitId_, 1, 1);

    layout->addWidget(baudRateLabel, 2, 0);
    layout->addWidget(baudRateComboBox_, 2, 1);

    layout->addWidget(dataBitsLabel, 3, 0);
    layout->addWidget(dataBitsComboBox_, 3, 1);

    layout->addWidget(stopBitsLabel, 4, 0);
    layout->addWidget(stopBitsComboBox_, 4, 1);

    layout->addWidget(parityLabel, 5, 0);
    layout->addWidget(parityComboBox_, 5, 1);

    connectionSettingsGroupBox_->setLayout(layout);
}

//-----------------------------------------------------------------------------

void
MainWindow::createUserDefinedFunctionGroupBox_()
{
    userDefinedFunctionGroupBox_ = new QGroupBox(tr("User defined function"));

    QLabel* functionCodeLabel = new QLabel(tr("Function code:"));
    functionCode_ = new QSpinBox();
    functionCode_->setRange(65, 70);

    QLabel* subFunctionCodeLabel = new QLabel(tr("Subfunction code:"));
    subFunctionCode_ = new QSpinBox;
    subFunctionCode_->setRange(0, 255);

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->addWidget(functionCodeLabel, 0, 0);
    gridLayout->addWidget(functionCode_, 0, 1);
    gridLayout->addWidget(subFunctionCodeLabel, 1, 0);
    gridLayout->addWidget(subFunctionCode_, 1, 1);

    QPushButton* sendButton = new QPushButton(tr("Send"));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(sendUserDefinedFunction()));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(sendButton);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(gridLayout);
    layout->addLayout(buttonLayout);

    userDefinedFunctionGroupBox_->setLayout(layout);
}


//-----------------------------------------------------------------------------

void
MainWindow::errorMessage(const QString& msg)
{
    QMessageBox errorBox(this);
    errorBox.setWindowTitle(tr("Error"));
    errorBox.setIcon(QMessageBox::Critical);
    errorBox.setText(msg);
    errorBox.setStandardButtons(QMessageBox::Ok);
    errorBox.exec();
}

//-----------------------------------------serialPortComboBox_------------------------------------

void
MainWindow::infoMessage(const QString& msg)
{
    QMessageBox errorBox(this);
    errorBox.setWindowTitle(tr("Information"));
    errorBox.setIcon(QMessageBox::Information);
    errorBox.setText(msg);
    errorBox.setStandardButtons(QMessageBox::Ok);
    errorBox.exec();
}

//-----------------------------------------------------------------------------

void
MainWindow::readCoils()
{
    QVector<bool> data(coilQty_->value());

    bool result = mbClient_->readCoils(coilStart_->value(), coilQty_->value(), data);
    if (!result)
    {
        errorMessage(tr("Cannot read coil(s)!"));
        return;
    }

    QString msgText = tr("Coil # : Value\n");
    int curReg = coilStart_->value();
    for (int i = 0; i < data.size(); ++i)
    {
        msgText += QString("  %1 : %2\n").arg(curReg, 7).arg(data[i], 7);
        curReg += 1;
    }

    QMessageBox resultBox(this);
    resultBox.setWindowTitle(tr("Readed values"));
    resultBox.setText(msgText);
    resultBox.setStandardButtons(QMessageBox::Ok);
    resultBox.exec();
}

//-----------------------------------------------------------------------------

void
MainWindow::readDiscreteInputs()
{
    QVector<bool> data(dInQty_->value());

    bool result = mbClient_->readDescreteInputs(dInStart_->value(), dInQty_->value(), data);
    if (!result)
    {
        errorMessage(tr("Cannot read discrete input(s)!"));
        return;
    }

    QString msgText = tr("Reg # : Value\n");
    int curReg = dInStart_->value();
    for (int i = 0; i < data.size(); ++i)
    {
        msgText += QString("  %1 : %2\n").arg(curReg, 7).arg(data[i], 7);
        curReg += 1;
    }

    QMessageBox resultBox(this);
    resultBox.setWindowTitle(tr("Readed values"));
    resultBox.setText(msgText);
    resultBox.setStandardButtons(QMessageBox::Ok);
    resultBox.exec();
}

//-----------------------------------------------------------------------------

void MainWindow::readInputRegistres()
{
    QVector<quint16> data(inRegQty_->value());

    bool result = mbClient_->readInputRegisters(inRegStart_->value(), inRegQty_->value(), data);
    if (!result)
    {
        errorMessage(tr("Cannot read input register(s)!"));
        return;
    }

    QString msgText = tr("Reg # : Value (DEC) : Value (HEX)\n");
    int curReg = inRegStart_->value();
    for (int i = 0; i < data.size(); ++i)
    {
        msgText += QString("%1 :%2 :%3\n").arg(curReg, 5).arg(data[i], 12).arg(data[i], 12, 16);
        curReg += 1;
    }

    QFont font;
    font.setFamily("Monospace");

    QMessageBox resultBox(this);
    resultBox.setFont(font);
    resultBox.setWindowTitle(tr("Readed values"));
    resultBox.setText(msgText);
    resultBox.setStandardButtons(QMessageBox::Ok);
    resultBox.exec();
}

//-----------------------------------------------------------------------------

void MainWindow::readHoldingRegisters()
{
    QVector<quint16> data(holdRegQty_->value());

    bool result = mbClient_->readHoldingRegisters(holdRegStart_->value(), holdRegQty_->value(), data);
    if (!result)
    {
        errorMessage(tr("Cannot read holding register(s)!"));
        return;
    }

    QString msgText = tr("Reg # : Value (DEC) : Value (HEX)\n");
    int curReg = holdRegStart_->value();
    for (int i = 0; i < data.size(); ++i)
    {
        msgText += QString("%1 :%2 :%3\n").arg(curReg, 5).arg(data[i], 12).arg(data[i], 12, 16);
        curReg += 1;
    }

    QFont font;
    font.setFamily("Monospace");

    QMessageBox resultBox(this);
    resultBox.setFont(font);
    resultBox.setWindowTitle(tr("Readed values"));
    resultBox.setText(msgText);
    resultBox.setStandardButtons(QMessageBox::Ok);
    resultBox.exec();
}

//-----------------------------------------------------------------------------

void
MainWindow::sendUserDefinedFunction()
{
    QVector<quint8> request, reply;
    int numBytes = 10;

    bool result = mbClient_->userDefinedFunction(quint8(functionCode_->value()), quint8(subFunctionCode_->value()), request, reply);
    if (!result)
    {
        errorMessage(tr("Cannot perfom user defined function!"));
        return;
    }

    QString msgText = tr("Reply byte # : Value (DEC) : Value (HEX)\n");
    for (int i = 0; i < reply.size() && i < numBytes; ++i)
    {
        msgText += QString("%1 :%2 :%3\n").arg(i, 5).arg(reply[i], 12).arg(reply[i], 12, 16);
    }

    QFont font;
    font.setFamily("Monospace");

    QMessageBox resultBox(this);
    resultBox.setFont(font);
    resultBox.setWindowTitle(tr("First %1 byte(s) in reply").arg(numBytes));
    resultBox.setText(msgText);
    resultBox.setStandardButtons(QMessageBox::Ok);
    resultBox.exec();
}

//-----------------------------------------------------------------------------

void
MainWindow::restoreSettings_()
{
    bool isConfigured = settings_->value("global/isConfigured").value<bool>();

    if (isConfigured)
    {
        QRect geom = settings_->value("window/geometry").value<QRect>();
        setGeometry(geom);

        settings_->beginGroup("connection");
        serialPortComboBox_->setCurrentIndex(serialPortComboBox_->findText(settings_->value("port").value<QString>()));
        unitId_->setValue(settings_->value("unitId").value<int>());
        baudRateComboBox_->setCurrentIndex(baudRateComboBox_->findText(settings_->value("baudRate").value<QString>()));
        dataBitsComboBox_->setCurrentIndex(dataBitsComboBox_->findText(settings_->value("dataBits").value<QString>()));
        stopBitsComboBox_->setCurrentIndex(stopBitsComboBox_->findText(settings_->value("stopBits").value<QString>()));
        parityComboBox_->setCurrentIndex(parityComboBox_->findText(settings_->value("parity").value<QString>()));
        settings_->endGroup();
    }
}

//-----------------------------------------------------------------------------

void
MainWindow::saveSettings_()
{
    settings_->beginGroup("global");
    settings_->setValue("isConfigured", true);
    settings_->endGroup();

    settings_->beginGroup("window");
    settings_->setValue("geometry",     geometry());
    settings_->endGroup();

    settings_->beginGroup("connection");
    settings_->setValue("port",         serialPortComboBox_->currentText());
    settings_->setValue("unitId",       unitId_->value());
    settings_->setValue("baudRate",     baudRateComboBox_->currentText());
    settings_->setValue("dataBits",     dataBitsComboBox_->currentText());
    settings_->setValue("stopBits",     stopBitsComboBox_->currentText());
    settings_->setValue("parity",       parityComboBox_->currentText());
    settings_->endGroup();
}

//-----------------------------------------------------------------------------

void
MainWindow::setUnitId(int id)
{
    mbClient_->setUnitID(id);
}

//-----------------------------------------------------------------------------

void
MainWindow::writeCoils()
{
    bool result = false;

    int qty = coilQty_->value();

    if (qty > 1)
    {
        QVector<bool> data(coilQty_->value());
        data.fill(coilValue_->value() == 1);

        result = mbClient_->writeCoils(coilStart_->value(), data);
    }
    else
    {
        result = mbClient_->writeSingleCoil(coilStart_->value(), coilValue_->value() == 1);
    }

    if (!result)
        errorMessage(tr("Cannot write coil(s)!"));
    else
        infoMessage(tr("Data writed to PLC!"));
}

//-----------------------------------------------------------------------------

void
MainWindow::writeHoldingRegisters()
{
    bool result = false;
    int qty = holdRegQty_->value();

    if (qty > 1)
    {
        QVector<quint16> data(holdRegQty_->value());
        data.fill(holdRegValue_->value());

        result = mbClient_->writeHoldingRegisters(holdRegStart_->value(), data);
        if (!result)
            errorMessage(tr("Cannot write holding register(s)!"));
        else
            infoMessage(tr("Data writed to PLC!"));
    }
    else
    {
        result = mbClient_->writeSingleRegister(holdRegStart_->value(), holdRegValue_->value());
        if (!result)
            errorMessage(tr("Cannot write holding register(s)!"));
        else
            infoMessage(tr("Data writed to PLC!"));
    }

}

