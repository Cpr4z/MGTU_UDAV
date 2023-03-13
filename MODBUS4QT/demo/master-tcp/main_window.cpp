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

#include "main_window.h"
#include "main.h"

#include <QtGui>

#include <QAction>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* centralWidget = new QWidget();

    QVBoxLayout* mainLayout = new QVBoxLayout;
    QGridLayout* registersLayout = new QGridLayout;

    createCoilsGroupBox_();
    createDiscreteInputsGroupBox_();
    createInputRegistersGroupBox_();
    createHoldingRegistersGroupBox_();

    registersLayout->addWidget(discreteInputsGroupBox_, 0, 0);
    registersLayout->addWidget(inputRegistersGroupBox_, 0, 1);
    registersLayout->addWidget(coilsGroupBox_, 1, 0);
    registersLayout->addWidget(holdingRegistersGroupBox_, 1, 1);

    createServerGroupBox_();

    mainLayout->addWidget(serverGroupBox_);
    mainLayout->addLayout(registersLayout);
    mainLayout->addStretch();
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    centralWidget->setLayout(mainLayout);

    setCentralWidget(centralWidget);

    createActions_();
    createMenus_();
    statusBar();
    setWindowTitle(ApplicationName);

    tcpClient_ = new modbus4qt::TcpClient(this);
    tcpClient_->setAutoConnect();

    connect(tcpClient_, SIGNAL(errorMessage(QString)), this, SLOT(errorMessage(QString)));
}

//-----------------------------------------------------------------------------

MainWindow::~MainWindow()
{

}

//-----------------------------------------------------------------------------

void
MainWindow::about()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("About"));
    msgBox.setText(ApplicationName);
    msgBox.setInformativeText(QString("Copyright \nhttp://www.") + OrganizationDomain);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

//-----------------------------------------------------------------------------

void
MainWindow::createActions_()
{
    aboutAction_ = new QAction(tr("&About..."), this);
    aboutAction_->setStatusTip(tr("Show this program's About box"));
    connect(aboutAction_, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction_ = new QAction(tr("About &Qt..."), this);
    aboutQtAction_->setShortcut(tr("Ctrl+Shift+F1"));
    aboutQtAction_->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction_, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    exitAction_ = new QAction(tr("E&xit"), this);
    exitAction_->setShortcut(tr("Ctrl+Q"));
    exitAction_->setStatusTip(tr("Exit the application"));
    connect(exitAction_, SIGNAL(triggered()), this, SLOT(close()));
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
    coilQty_->setRange(0, 65535);

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
    dInQty_->setRange(0, 65535);

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
    inRegQty_->setRange(0, 65535);

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
    holdRegQty_->setRange(0, 65535);

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

    QPushButton* readBotton = new QPushButton("Read");
    connect(readBotton, SIGNAL(clicked()), this, SLOT(readHoldingRegisters()));

    QPushButton* writeBotton = new QPushButton("Write");
    connect(writeBotton, SIGNAL(clicked()), this, SLOT(writeHoldingRegisters()));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(readBotton);
    buttonLayout->addWidget(writeBotton);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(gridLayout);
    layout->addLayout(buttonLayout);

    holdingRegistersGroupBox_->setLayout(layout);

}

//-----------------------------------------------------------------------------

void
MainWindow::createMenus_()
{
    fileMenu_ = menuBar()->addMenu(tr("&File"));
    fileMenu_->addAction(exitAction_);

    helpMenu_ = menuBar()->addMenu(tr("Help"));
    helpMenu_->addAction(aboutAction_);
    helpMenu_->addAction(aboutQtAction_);
}

//-----------------------------------------------------------------------------

void
MainWindow::createServerGroupBox_()
{
    serverGroupBox_ = new QGroupBox(tr("MODBUS TCP Server"));
    QGridLayout *layout = new QGridLayout;

    QLabel* serverAddressLabel = new QLabel(tr("Server address:"));
    serverAddress_ = new QLineEdit("127.0.0.1");

    layout->addWidget(serverAddressLabel, 0, 0);
    layout->addWidget(serverAddress_, 0, 1);

    QLabel* serverPortLabel = new QLabel(tr("Server port:"));
    serverPort_ = new QSpinBox();
    serverPort_->setRange(1, 65535);
    serverPort_->setValue(502);

    layout->addWidget(serverPortLabel, 1, 0);
    layout->addWidget(serverPort_, 1, 1);

    serverGroupBox_->setLayout(layout);
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

//-----------------------------------------------------------------------------

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
    tcpClient_->setServerAddress(QHostAddress(serverAddress_->text()));
    tcpClient_->setServerPort(serverPort_->value());

    QVector<bool> data(coilQty_->value());

    bool result = tcpClient_->readCoils(coilStart_->value(), coilQty_->value(), data);
    if (!result) errorMessage(tr("Cannot read coil(s)!"));

    QString msgText = tr("Reg # : Value\n");
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

void MainWindow::readDiscreteInputs()
{
    tcpClient_->setServerAddress(QHostAddress(serverAddress_->text()));
    tcpClient_->setServerPort(serverPort_->value());

    QVector<bool> data(dInQty_->value());

    bool result = tcpClient_->readDescreteInputs(dInStart_->value(), dInQty_->value(), data);
    if (!result) errorMessage(tr("Cannot read discrete input(s)!"));

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
    tcpClient_->setServerAddress(QHostAddress(serverAddress_->text()));
    tcpClient_->setServerPort(serverPort_->value());

    QVector<quint16> data(inRegQty_->value());

    bool result = tcpClient_->readInputRegisters(inRegStart_->value(), inRegQty_->value(), data);
    if (!result) errorMessage(tr("Cannot read input register(s)!"));

    QString msgText = tr("Reg # : Value\n");
    int curReg = inRegStart_->value();
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

void MainWindow::readHoldingRegisters()
{
    tcpClient_->setServerAddress(QHostAddress(serverAddress_->text()));
    tcpClient_->setServerPort(serverPort_->value());

    QVector<quint16> data(holdRegQty_->value());

    bool result = tcpClient_->readHoldingRegisters(holdRegStart_->value(), holdRegQty_->value(), data);
    if (!result)
    {
        errorMessage(tr("Cannot read holding register(s)!"));
    }

    QString msgText = tr("Reg # : Value\n");
    int curReg = holdRegStart_->value();
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
MainWindow::writeCoils()
{
    tcpClient_->setServerAddress(QHostAddress(serverAddress_->text()));
    tcpClient_->setServerPort(serverPort_->value());

    QVector<bool> data(coilQty_->value());
    data.fill(coilValue_->value() == 1);

    bool result = tcpClient_->writeCoils(coilStart_->value(), data);
    if (!result)
        errorMessage(tr("Cannot write holding register(s)!"));
    else
        infoMessage(tr("Data writed to PLC!"));
}

//-----------------------------------------------------------------------------

void
MainWindow::writeHoldingRegisters()
{
    tcpClient_->setServerAddress(QHostAddress(serverAddress_->text()));
    tcpClient_->setServerPort(serverPort_->value());

    QVector<quint16> data(holdRegQty_->value());
    data.fill(holdRegValue_->value());

    bool result = tcpClient_->writeHoldingRegisters(holdRegStart_->value(), data);
    if (!result)
        errorMessage(tr("Cannot write holding register(s)!"));
    else
        infoMessage(tr("Data writed to PLC!"));
}


